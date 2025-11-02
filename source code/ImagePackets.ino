// ESP32_TX.ino
// Transmitter: reads from USB serial (PC), sends FILE_START then DATA packets with ACK & retry over LoRa.

#include <SPI.h>
#include <LoRa.h>

#define LORA_SCK   36
#define LORA_MISO  37
#define LORA_MOSI  35
#define LORA_SS    10
#define LORA_RST   9
#define LORA_DIO0  8

// Packet types
#define PT_FILE_START 0x01
#define PT_DATA       0x02
#define PT_ACK        0x03
#define PT_FILE_END   0x04

const long LORA_FREQ = 868E6; // change to 915E6 if needed
const int CHUNK_SIZE = 200;
const int MAX_RETRIES = 5;
const unsigned long ACK_TIMEOUT = 1500; // ms

uint32_t seq = 0;

void sendLoRaPacket(uint8_t *buf, int len) {
  LoRa.beginPacket();
  LoRa.write(buf, len);
  LoRa.endPacket();
}

bool waitForAck(uint32_t expectSeq) {
  unsigned long start = millis();
  while (millis() - start < ACK_TIMEOUT) {
    int ps = LoRa.parsePacket();
    if (ps > 0) {
      uint8_t t = LoRa.read();
      if (t == PT_ACK) {
        uint32_t rseq = 0;
        for (int i = 0; i < 4; ++i) {
          rseq = (rseq << 8) | (uint8_t)LoRa.read();
        }
        if (rseq == expectSeq) return true;
      }
    }
  }
  return false;
}

void sendFileStart(uint64_t filesize) {
  // Build FILE_START packet: [PT_FILE_START][8-byte filesize big-endian]
  uint8_t buf[1 + 8];
  buf[0] = PT_FILE_START;
  for (int i = 0; i < 8; ++i) {
    buf[1 + i] = (filesize >> (8 * (7 - i))) & 0xFF;
  }

  for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
    sendLoRaPacket(buf, sizeof(buf));
    // wait for ACK seq 0 as handshake
    if (waitForAck(0)) return;
  }
  Serial.println("Failed to get FILE_START ACK");
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1) delay(1000);
  }
  Serial.println("ESP32-TX LoRa ready");
}

void loop() {
  // Wait for start marker from PC
  if (!Serial.available()) {
    delay(10);
    return;
  }

  // Expect PC to send "<START>" then 8 bytes filesize, then chunked data (len byte + payload)
  String marker = Serial.readStringUntil('\n'); // avoid blocking on binary; sender will send a newline after START
  marker.trim();
  if (marker != "<START>") {
    // ignore any other text lines
    Serial.println("Waiting for <START>");
    return;
  }

  // Read 8 bytes filesize from serial (raw)
  while (Serial.available() < 8) delay(1);
  uint64_t filesize = 0;
  for (int i = 0; i < 8; ++i) {
    int b = Serial.read();
    filesize = (filesize << 8) | (uint8_t)b;
  }
  Serial.print("File size: "); Serial.println((unsigned long)filesize);

  // Send FILE_START and wait for handshake ACK (seq=0)
  sendFileStart(filesize);
  Serial.println("FILE_START sent, proceeding to send data...");

  // send data chunks
  seq = 1; // start data seq at 1 (0 reserved for handshake)
  uint64_t bytesSent = 0;
  while (bytesSent < filesize) {
    // wait for length byte
    while (!Serial.available()) delay(1);
    int len = Serial.read(); // 1 byte length
    if (len <= 0) continue;
    uint8_t payload[CHUNK_SIZE];
    int got = 0;
    while (got < len) {
      int r = Serial.readBytes(payload + got, len - got);
      if (r > 0) got += r;
    }

    // Build DATA packet: [PT_DATA][4-byte seq big-endian][2-byte len][payload]
    uint8_t pkt[1 + 4 + 2 + CHUNK_SIZE];
    int idx = 0;
    pkt[idx++] = PT_DATA;
    pkt[idx++] = (seq >> 24) & 0xFF;
    pkt[idx++] = (seq >> 16) & 0xFF;
    pkt[idx++] = (seq >> 8) & 0xFF;
    pkt[idx++] = (seq) & 0xFF;
    pkt[idx++] = (len >> 8) & 0xFF;
    pkt[idx++] = (len) & 0xFF;
    memcpy(pkt + idx, payload, len);
    int pktlen = idx + len;

    bool acked = false;
    for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
      sendLoRaPacket(pkt, pktlen);
      // wait for ACK(seq)
      if (waitForAck(seq)) {
        acked = true;
        break;
      }
      // else retry
    }

    if (!acked) {
      Serial.print("Failed to get ACK for seq "); Serial.println(seq);
      // Optionally abort entire transfer
      Serial.println("ABORT");
      break;
    }

    bytesSent += len;
    seq++;
    Serial.print("Sent seq "); Serial.print(seq - 1); Serial.print("  bytesSent: "); Serial.println(bytesSent);
  }

  // send FILE_END
  uint8_t endbuf[1 + 8];
  endbuf[0] = PT_FILE_END;
  for (int i = 0; i < 8; ++i) endbuf[1 + i] = (filesize >> (8 * (7-i))) & 0xFF;
  for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
    sendLoRaPacket(endbuf, sizeof(endbuf));
    if (waitForAck(0xFFFFFFFFUL)) { // use 0xFFFFFFFF as FILE_END ack code
      Serial.println("File transfer complete, got FILE_END ACK");
      break;
    }
  }

  Serial.println("Finished transfer loop");
  // back to waiting for next transfer
}
