/*
  Project: DHTPlusEPS
  Description:
    This program interfaces the DHT11 temperature and humidity sensor with an ESP32.
    It reads ambient humidity and temperature (in both Celsius and Fahrenheit),
    displays the results in the serial monitor, and reports errors if the sensor
    fails to provide valid readings.

  Hardware Connections:
    - DHT11 DATA pin → GPIO17 (changeable via DHTPIN define)
    - VCC → 3.3V
    - GND → GND

  Library Requirements:
    - DHT sensor library by Adafruit
    - Adafruit Unified Sensor library

  Functional Overview:
    - Initializes DHT11 sensor on startup.
    - Continuously reads humidity and temperature values every 2 seconds.
    - Displays results via the serial monitor.
    - Handles NaN (invalid) readings gracefully.

  Author: Peter Njuguna
*/


#include "DHT.h"

#define DHTPIN 17       // GPIO17 connected to DHT11 DATA pin
#define DHTTYPE DHT11   // Define sensor type DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 test starting...");
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius
  float temperatureF = dht.readTemperature(true); // Fahrenheit

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");
  Serial.print(temperatureF);
  Serial.println(" *F");

  delay(2000); // Wait 2 seconds between readings
}
