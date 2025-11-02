/*
 *  LoRa Encrypted Sequential Sender
 *  --------------------------------
 *  This program acts as a data bridge between a Raspberry Pi and a LoRa transceiver.
 *  It reads binary data from the Raspberry Pi via UART (Serial1), then transmits 
 *  the data sequentially over LoRa.
 *
 *  Purpose:
 *    - To transmit encrypted or binary data chunks (e.g., image segments, telemetry)
 *      received from a Raspberry Pi or similar processor.
 *    - Designed for real-time data forwarding in distributed satellite or IoT systems.
 *
 *  Features:
 *    - Uses UART (Serial1) to interface with a Raspberry Pi.
 *    - Sends data sequentially via LoRa at 868 MHz.
 *    - Works efficiently for small-to-medium payloads (<200 bytes).
 *
 *  System Integration Example:
 *    Raspberry Pi  →  ESP32 (this code)  →  LoRa  →  Remote Receiver (e.g., LoRa_FileReceiver)
 *
 *  Notes:
 *    - Adjust `LORA_BAND` to your regional frequency (e.g., 915E6 for North America).
 *    - Data integrity or encryption is handled externally (e.g., by the Raspberry Pi).
 *    - Ideal for relaying pre-encrypted, framed, or image data sequences.
 *
 *  Recommended File Name: LoRa_EncryptedSequentialSender.ino
 *
 *  Author: Peter Njuguna
 *  Date: October 2025
 */

#include <SPI.h>
#include <LoRa.h>

// LoRa SPI pin configuration
#define SCK   36
#define MISO  37
#define MOSI  35
#define NSS   10
#define RST   9
#define DIO0  8

// UART pins for Raspberry Pi link
#define RXD1 16   // ESP32 RX  ←  Pi TX
#define TXD1 17   // ESP32 TX  →  Pi RX

// LoRa frequency band (adjust for your region)
#define LORA_BAND 868E6

void setup() {
  // Initialize Serial for debugging and Serial1 for UART communication with Raspberry Pi
  Serial.begin(115200);    
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);

  Serial.println("Initializing LoRa transmitter...");

  // Initialize LoRa module
  SPI.begin(SCK, MISO, MOSI, NSS);
  LoRa.setPins(NSS, RST, DIO0);

  // Attempt to start LoRa
  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("❌ LoRa init failed. Check wiring and frequency settings.");
    while (1);
  }

  Serial.println("✅ LoRa transmitter ready.");
}

void loop() {
  // Check if data is available from the Raspberry Pi
  if (Serial1.available()) {
    uint8_t buffer[200];                     // Temporary data buffer
    int len = Serial1.readBytes(buffer, sizeof(buffer));

    if (len > 0) {
      // Debugging output
      Serial.print("Forwarding ");
      Serial.print(len);
      Serial.println(" bytes via LoRa...");

      // Send received data chunk over LoRa
      LoRa.beginPacket();
      LoRa.write(buffer, len);
      LoRa.endPacket();
    }
  }
}
