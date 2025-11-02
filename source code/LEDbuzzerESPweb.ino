/*
  Project: WiFi_LED_Buzzer_Control
  Description:
    This ESP32 sketch creates a Wi-Fi–based web dashboard for remotely controlling
    an LED and a buzzer. The interface updates dynamically and provides real-time
    visual feedback using color-coded and animated buttons.

  Hardware Connections:
    - LED: GPIO17 → resistor → LED → GND
    - Buzzer: GPIO8 → buzzer (+), GND → buzzer (–)
    - ESP32 Wi-Fi SSID: "Your WiFi name"
    - Password: "WiFi password"

  Features:
    • Web interface hosted on ESP32 (port 80)
    • LED and buzzer ON/OFF toggle via HTTP routes
    • Dynamic HTML with inline CSS and animations
    • Real-time status reflection on web UI

  Author: Peter Njuguna
  License: MIT
*/


#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "********";// insert wifi name
const char* password = "########"; // put wifi password

WebServer server(80);

const int ledPin = 17;
const int buzzerPin = 8;

bool ledState = false;
bool buzzerState = false;

String getHTML() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;}";
  html += "button{padding:15px 30px;font-size:20px;margin:10px;border:none;border-radius:8px;color:white;cursor:pointer;}";
  html += ".on{background:green;} .off{background:red;}";
  html += ".buzzerOff{background:gray;}";
  html += ".buzzerOn{background:yellow;color:black;font-weight:bold;animation: blink 1s infinite;}";
  html += "@keyframes blink {50% {background:red;color:white;}}";
  html += "</style></head><body>";

  html += "<h2>ESP32 LED & Buzzer Control</h2>";

  // LED status + button
  html += "<p>LED is <b>" + String(ledState ? "ON" : "OFF") + "</b></p>";
  html += "<a href='/toggleLED'><button class='" + String(ledState ? "on" : "off") + "'>";
  html += (ledState ? "Turn OFF LED" : "Turn ON LED") + String("</button></a>");

  // Buzzer status + button
  html += "<p>Buzzer is <b>" + String(buzzerState ? "ON ⚠️" : "OFF") + "</b></p>";
  html += "<a href='/toggleBuzzer'><button class='" + String(buzzerState ? "buzzerOn" : "buzzerOff") + "'>";
  html += (buzzerState ? "STOP BUZZER" : "START BUZZER") + String("</button></a>");

  html += "</body></html>";
  return html;
}

void handleRoot() { server.send(200, "text/html", getHTML()); }

void handleToggleLED() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);
  handleRoot();
}

void handleToggleBuzzer() {
  buzzerState = !buzzerState;
  digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
  handleRoot();
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggleLED", handleToggleLED);
  server.on("/toggleBuzzer", handleToggleBuzzer);

  server.begin();
}

void loop() {
  server.handleClient();
}
