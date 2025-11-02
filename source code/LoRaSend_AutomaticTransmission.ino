#include <SPI.h>
#include <LoRa.h>

// LoRa SPI pin configuration
#define SCK   36
#define MISO  37
#define MOSI  35
#define NSS   10
#define RST   9
#define DIO0  8

String incomingLine = "";
bool sendingFile = false;
String currentFile = "";
int packetCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

    SPI.begin(SCK, MISO, MOSI, NSS);
    LoRa.setPins(NSS, RST, DIO0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Sender Ready ✅");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    incomingLine += c;

    // Detect start of file marker
    if (incomingLine.endsWith("]\n")) {
      if (incomingLine.startsWith("[START_FILE:")) {
        sendingFile = true;
        currentFile = incomingLine;
        Serial.println("Start signal received");
        LoRa.beginPacket();
        LoRa.print(incomingLine);
        LoRa.endPacket();
      } 
      else if (incomingLine.startsWith("[END_FILE")) {
        sendingFile = false;
        Serial.println("End signal received");
        LoRa.beginPacket();
        LoRa.print(incomingLine);
        LoRa.endPacket();
      }
      incomingLine = "";
    }

    // Send binary data if inside file
    if (sendingFile && Serial.availableForWrite() == 0) {
      LoRa.beginPacket();
      LoRa.write((uint8_t*)&c, 1);
      LoRa.endPacket();
    }
  }
}
