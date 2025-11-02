/*
  Project: espTinyGPSPlus
  Description:
    This program interfaces an ESP32 with a GPS module using the TinyGPS++ library.
    It reads real-time GPS data such as latitude, longitude, altitude, speed, and UTC time,
    and displays them on the serial monitor once valid satellite data is received.

  Hardware Connections:
    - GPS Module TX → ESP32 RX2 (GPIO16)
    - GPS Module RX → ESP32 TX2 (GPIO17)
    - VCC → 3.3V
    - GND → GND

  Library Requirements:
    - TinyGPSPlus library by Mikal Hart

  Functional Overview:
    - Initializes UART communication between ESP32 and GPS module.
    - Continuously parses NMEA data from the GPS receiver.
    - Displays valid location, speed, altitude, and time information.
    - Handles cases where GPS signal is not yet available.

  Baud Rate:
    - GPS Communication: 9600 bps (default)
    - Serial Monitor: 115200 bps

  Author: Peter Njuguna
*/


#include <TinyGPSPlus.h>

// GPS pins
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

// Create GPS object
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Waiting for GPS signal...");
}

void loop() {
  // Feed GPS data to TinyGPS++
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // If GPS has valid data, print it
  if (gps.location.isValid()) {
    Serial.print("Latitude:  "); Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: "); Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Location: Not Available");
  }

  if (gps.altitude.isValid()) {
    Serial.print("Altitude:  "); Serial.print(gps.altitude.meters());
    Serial.println(" m");
  }

  if (gps.speed.isValid()) {
    Serial.print("Speed:     "); Serial.print(gps.speed.kmph());
    Serial.println(" km/h");
  }

  if (gps.time.isValid()) {
    Serial.print("Time (UTC): ");
    Serial.print(gps.time.hour()); Serial.print(":");
    Serial.print(gps.time.minute()); Serial.print(":");
    Serial.println(gps.time.second());
  }

  Serial.println("-------------------------------");
  delay(1000);
}
