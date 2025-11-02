#include <SPI.h>
#include <LoRa.h>

#define SCK   36
#define MISO  37
#define MOSI  35
#define NSS   10
#define RST   9
#define DIO0  8

int counter = 1;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(NSS, RST, DIO0);

  if (!LoRa.begin(868E6)) {  // or 915E6 depending on your module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Transmitter with ACK");
}

void loop() {
  // Send a message
  String message = "Hello " + String(counter);
  Serial.print("Sending: "); Serial.println(message);

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  // Wait for ACK
  unsigned long start = millis();
  bool ackReceived = false;
  
  while (millis() - start < 2000) {  // 2s timeout
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String ack = "";
      while (LoRa.available()) {
        ack += (char)LoRa.read();
      }
      if (ack == "ACK " + String(counter)) {
        Serial.println("ACK received ✅");
        ackReceived = true;
        break;
      }
    }
  }

  if (!ackReceived) {
    Serial.println("No ACK ❌");
  }

  counter++;
  delay(3000);
}
