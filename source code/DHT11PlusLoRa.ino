/*
 *  DHT11_LoRa_Transmitter.ino
 *  ------------------------------------------------------------
 *  Description:
 *    Reads temperature and humidity data from a DHT11 sensor,
 *    transmits it via LoRa, and waits for an acknowledgment (ACK)
 *    from the receiver node. Retransmits if no ACK is received.
 *
 *  Features:
 *    ✅ Reads real-time environmental data from DHT11
 *    ✅ Sends sensor readings via LoRa (868 MHz default)
 *    ✅ Retransmission mechanism with up to MAX_RETRIES attempts
 *    ✅ Waits for specific ACK message format ("ACK <counter>")
 *
 *  Hardware Connections:
 *    DHT11 Sensor:
 *       - Data → GPIO17
 *       - VCC  → 3.3V
 *       - GND  → GND
 *
 *    LoRa Module:
 *       - SCK   → GPIO36
 *       - MISO  → GPIO37
 *       - MOSI  → GPIO35
 *       - NSS   → GPIO10
 *       - RST   → GPIO9
 *       - DIO0  → GPIO8
 *
 *  Message Format:
 *     "T=<temperature>C, H=<humidity>%, ID=<counter>"
 *
 *  Notes:
 *    - Adjust LoRa frequency (868E6 or 915E6) based on your region.
 *    - Ensure the receiver sends back an ACK in the format:
 *         "ACK <counter>"
 *    - Ideal for sensor network nodes that require reliable delivery.
 *
 *  Author: Peter Njuguna
 *  Date: October 2025
 */

#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"

// ==== DHT11 SETUP ====
#define DHTPIN 17       // GPIO17 for DHT11 data pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ==== LoRa Pins ====
#define SCK   36
#define MISO  37
#define MOSI  35
#define NSS   10
#define RST   9
#define DIO0  8

const int MAX_RETRIES = 3;   // Max retransmission attempts
const int TIMEOUT = 2000;    // 2-second wait for ACK
int counter = 1;             // Packet ID counter

void setup() {
  Serial.begin(115200);
  while (!Serial);

  dht.begin();  // Initialize DHT11 sensor

  // Initialize LoRa transceiver
  SPI.begin(SCK, MISO, MOSI, NSS);
  LoRa.setPins(NSS, RST, DIO0);

  if (!LoRa.begin(868E6)) {  // Use 915E6 for your regional LoRa frequency
    Serial.println("❌ Starting LoRa failed! Check wiring or module.");
    while (1);
  }

  Serial.println("✅ LoRa Transmitter (DHT11 with Retransmission) Ready");
}

void loop() {
  // Read DHT11 sensor values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check for read failure
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("❌ Failed to read from DHT11 sensor!");
    delay(2000);
    return;
  }

  // Create LoRa message
  String message = "T=" + String(temperature, 1) + "C, H=" + String(humidity, 1) + "%, ID=" + String(counter);
  bool success = false;

  // Attempt to send with retries
  for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
    Serial.print("📤 Sending: "); Serial.print(message);
    Serial.print(" (Attempt "); Serial.print(attempt); Serial.println(")");

    // Send packet
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();

    // Wait for ACK
    unsigned long start = millis();
    while (millis() - start < TIMEOUT) {
      int packetSize = LoRa.parsePacket();
      if (packetSize) {
        String ack = "";
        while (LoRa.available()) {
          ack += (char)LoRa.read();
        }

        // Verify correct ACK message
        if (ack == "ACK " + String(counter)) {
          Serial.println("✅ ACK received");
          success = true;
          break;
        }
      }
    }

    if (success) break;  // Stop retrying if ACK received
    else Serial.println("⚠️ No ACK → Retrying...");
  }

  if (!success) {
    Serial.println("🚫 Transmission failed after retries");
  }

  counter++;        // Increment message ID
  delay(5000);      // Send every 5 seconds
}
