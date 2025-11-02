/*
 *  LoRa File Receiver
 *  ------------------
 *  This program listens for incoming LoRa packets and reconstructs files transmitted
 *  in chunks from a sender device. It handles the start, data, and end markers for 
 *  a file transmission and logs progress via Serial Monitor.
 *
 *  Features:
 *   - Detects and processes <START:filename>, <SEQ:n>, and <END:filename> packets.
 *   - Tracks received chunks and displays file transfer progress.
 *   - Designed for reliable, chunked file reception over LoRa.
 *
 *  Suggested Use:
 *   - Use alongside a corresponding LoRa sender that formats packets with START/SEQ/END tags.
 *   - Modify to save received chunks to an SD card if persistent storage is required.
 *
 *  Recommended File Name: LoRa_FileReceiver.ino
 *
 *  Author: Peter Njuguna
 *  Date: October 2025
 */

#include <SPI.h>
#include <LoRa.h>

// LoRa SPI pin configuration (adjust to your board wiring)
#define SCK   36
#define MISO  37
#define MOSI  35
#define NSS   10
#define RST   9
#define DIO0  8

bool receivingFile = false;     // Flag to track active file reception
String currentFilename = "";    // Name of the file being received
int lastSeq = -1;               // Last received sequence number

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n📡 Initializing LoRa Receiver...");
  SPI.begin(SCK, MISO, MOSI, NSS);
  LoRa.setPins(NSS, RST, DIO0);

  // Initialize LoRa at 868 MHz (change to 915 MHz for your region)
  if (!LoRa.begin(868E6)) {
    Serial.println("❌ Starting LoRa failed!");
    while (1);
  }

  Serial.println("✅ LoRa initialized. Listening for packets...");
  Serial.println("-------------------------------------------");
}

void loop() {
  // Check for incoming LoRa packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String packet = "";
    while (LoRa.available()) {
      packet += (char)LoRa.read();  // Reconstruct packet content
    }
    processPacket(packet);  // Analyze and process the received packet
  }
}

/*
 * processPacket()
 * ---------------
 * Interprets the content of a received LoRa packet.
 * Handles:
 *   <START:filename>  → Marks beginning of a new file.
 *   <SEQ:n>data       → A numbered data chunk.
 *   <END:filename>    → Marks end of file transmission.
 * Logs progress and validates structure.
 */
void processPacket(String packet) {
  // --- Detect start of file ---
  if (packet.startsWith("<START:")) {
    int startIndex = packet.indexOf(":") + 1;
    int endIndex = packet.indexOf(">");
    currentFilename = packet.substring(startIndex, endIndex);
    receivingFile = true;
    lastSeq = -1;

    Serial.printf("\n📥 Start receiving: %s\n", currentFilename.c_str());
    return;
  }

  // --- Detect data chunks ---
  if (packet.startsWith("<SEQ:") && receivingFile) {
    int endTag = packet.indexOf(">");
    if (endTag != -1) {
      int seqNum = packet.substring(5, endTag).toInt();
      packet.remove(0, endTag + 1); // remove header, keep only data

      lastSeq = seqNum;
      Serial.printf("📦 Chunk #%d received (%d bytes)\n", seqNum, packet.length());
      return;
    }
  }

  // --- Detect end of file ---
  if (packet.startsWith("<END:") && receivingFile) {
    Serial.printf("✅ File %s completed (%d chunks)\n", currentFilename.c_str(), lastSeq + 1);
    receivingFile = false;
    currentFilename = "";
    return;
  }

  // --- Unrecognized data ---
  Serial.printf("⚠️ Unknown packet: %s\n", packet.c_str());
}
