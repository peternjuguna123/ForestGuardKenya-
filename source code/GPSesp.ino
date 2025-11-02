/*********
  Project: GPSesp
  Description:
    This program interfaces an ESP32 with a GPS receiver module (e.g., NEO-6M)
    and outputs the raw NMEA sentences received from the GPS over UART.
    It is ideal for testing GPS serial communication and verifying proper wiring
    before parsing data using a library such as TinyGPS++.

  Hardware Connections:
    - GPS Module TX → ESP32 RX2 (GPIO16)
    - GPS Module RX → ESP32 TX2 (GPIO17)
    - VCC → 3.3V
    - GND → GND

  Baud Rate:
    - GPS Communication: 9600 bps (default for most NEO-6M modules)
    - Serial Monitor: 115200 bps

  Functional Overview:
    - Initializes Serial2 for UART communication with the GPS module.
    - Continuously reads and prints raw NMEA sentences to the Serial Monitor.
    - Useful for verifying GPS output before data parsing.

  Author: Peter Njuguna
  Based on example by Rui Santos & Sara Santos (Random Nerd Tutorials)
  License: MIT
  Complete instructions at https://RandomNerdTutorials.com/esp32-neo-6m-gps-module-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

 // Define the RX and TX pins for Serial 2
#define RXD2 16
#define TXD2 17

#define GPS_BAUD 9600

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

void setup(){
  // Serial Monitor
  Serial.begin(115200);
  
  // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");
}

void loop(){
  while (gpsSerial.available() > 0){
    // get the byte data from the GPS
    char gpsData = gpsSerial.read();
    Serial.print(gpsData);
  }
  delay(1000);
  Serial.println("-------------------------------");
}