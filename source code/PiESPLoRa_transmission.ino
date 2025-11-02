/*
 *  Pi ↔ ESP32 ↔ LoRa Transmission with Acknowledgement
 *  ---------------------------------------------------
 *  This sketch enables bidirectional communication between a Raspberry Pi 
 *  and an ESP32 LoRa transmitter module. The ESP32 receives data from the Pi 
 *  via UART, then transmits it over LoRa and waits for an acknowledgment (ACK)
 *  from the receiver node.
 *
 *  Purpose:
 *    - Facilitate reliable data transmission from a Raspberry Pi to a LoRa network.
 *    - Ensure message integrity by confirming each transmission via ACK.
 *
 *  Features:
 *    ✅ UART bridge between Raspberry Pi (TX/RX) and ESP32 (Serial1)
 *    ✅ Sends messages over LoRa (868 MHz)
 *    ✅ Waits up to 2 seconds for an ACK response
 *    ✅ Prints transmission activity for monitoring via Serial Monitor
 *
 *  Typical System Setup:
 *      Raspberry Pi  →  ESP32 (this code)  →  LoRa Receiver (with ACK)
 *
 *  Pin Configuration:
 *    - LoRa Module:
 *         SCK  →  36
 *         MISO →  37
 *         MOSI →  35
 *         NSS  →  10
 *         RST  →  9
 *         DIO0 →  8
 *    - UART Link (ESP32 ↔ Raspberry Pi):
 *         RXD1 →  16 (ESP32 RX ← Pi TX)
 *         TXD1 →  17 (ESP32 TX → Pi RX)
 *
 *  Notes:
 *    - Adjust frequency (868E6 / 915E6) based on your regional LoRa band.
 *    - Add error-handling or retransmission logic for higher reliability.
 *    - This code pairs well with “LoRa_EncryptedSequentialSender.ino” or similar receivers.
 *
 *  Recommended File Name: Pi_ESP32_LoRa_Transmission.ino
 *  Author: Peter Njuguna
 *  Date: October 2025
 */

#include <SPI.h>
#include <LoRa.h>

// LoRa SPI pins
#define SCK   36
#define MISO  37
#define MOSI  35
#define NSS   10
#define RST   9
#define DIO0  8

// UART pins for Raspberry Pi link
#define RXD1 16   // ESP32 RX → Pi TX
#define TXD1 17   // ESP32 TX → Pi RX

void setup() {
  Serial.begin(115200);                       // Debugging output (USB)
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);  // UART link to Raspberry Pi

  // Initialize LoRa
  SPI.begin(SCK, MISO, MOSI, NSS);
  LoRa.setPins(NSS, RST, DIO0);

  if (!LoRa.begin(868E6)) {   // Use 868E6 or 915E6 depending on your region
    Serial.println("❌ Starting LoRa failed! Check wiring and module.");
    while (1);
  }
  Serial.println("✅ LoRa Transmitter Ready");
}

void loop() {
  // Step 1: Check if Raspberry Pi sent data
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    data.trim();

    if (data.length() > 0) {
      // Step 2: Display received data
      Serial.print("📨 Received from Pi: ");
      Serial.println(data);

      // Step 3: Forward data via LoRa
      Serial.print("📤 Forwarding via LoRa: ");
      Serial.println(data);

      LoRa.beginPacket();
      LoRa.print(data);
      LoRa.endPacket();

      // Step 4: Wait for ACK
      unsigned long start = millis();
      bool ackReceived = false;

      while (millis() - start < 2000) { // 2-second ACK timeout
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
          String incoming = LoRa.readString();
          if (incoming == "ACK") {
            Serial.println("✅ ACK received");
            ackReceived = true;
            break;
          }
        }
      }

      if (!ackReceived) {
        Serial.println("⚠️ No ACK received (timeout)");
      }
    }
  }
}
