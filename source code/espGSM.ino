#include <WiFi.h>
#include <WebServer.h>
#include <TinyGPSPlus.h>

// Wi-Fi credentials
const char* ssid = "njuguna";
const char* password = "398#Pita";

// Server
WebServer server(80);

// Pins
const int ledPin = 17;
const int buzzerPin = 8;

// States
bool ledState = false;
bool buzzerState = false;

// GPS setup
TinyGPSPlus gps;
HardwareSerial gpsSerial(1); // Use UART1
float latitude = 0.0, longitude = 0.0;

String getHTML() {
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body{font-family:Arial;text-align:center;} button{padding:15px 30px;font-size:20px;margin:10px;border:none;border-radius:8px;color:white;cursor:pointer;}";
  html += ".on{background:green;} .off{background:red;}";
  html += ".buzzerOff{background:gray;} .buzzerOn{background:yellow;color:black;font-weight:bold;animation: blink 1s infinite;} @keyframes blink {50% {background:red;color:white;}}";
  html += "</style></head><body>";

  html += "<h2>ESP32 LED, Buzzer & GPS</h2>";

  // LED section
  html += "<p>LED is <b>" + String(ledState ? "ON" : "OFF") + "</b></p>";
  html += "<a href='/toggleLED'><button class='" + String(ledState ? "on" : "off") + "'>";
  html += (ledState ? "Turn OFF LED" : "Turn ON LED") + String("</button></a>");

  // Buzzer section
  html += "<p>Buzzer is <b>" + String(buzzerState ? "ON ⚠️" : "OFF") + "</b></p>";
  html += "<a href='/toggleBuzzer'><button class='" + String(buzzerState ? "buzzerOn" : "buzzerOff") + "'>";
  html += (buzzerState ? "STOP BUZZER" : "START BUZZER") + String("</button></a>");

  // GPS section
  html += "<h3>GPS Location</h3>";
  if (latitude != 0.0 && longitude != 0.0) {
    html += "<p>Latitude: " + String(latitude, 6) + "<br>Longitude: " + String(longitude, 6) + "</p>";
    html += "<p><a href='https://maps.google.com/?q=" + String(latitude, 6) + "," + String(longitude, 6) + "' target='_blank'>Open in Google Maps</a></p>";
  } else {
    html += "<p><i>Waiting for GPS signal...</i></p>";
  }

  html += "</body></html>";
  return html;
}

void handleRoot() { server.send(200, "text/html", getHTML()); }
void handleToggleLED() { ledState = !ledState; digitalWrite(ledPin, ledState ? HIGH : LOW); handleRoot(); }
void handleToggleBuzzer() { buzzerState = !buzzerState; digitalWrite(buzzerPin, buzzerState ? HIGH : LOW); handleRoot(); }

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17 for GPS

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/toggleLED", handleToggleLED);
  server.on("/toggleBuzzer", handleToggleBuzzer);
  server.begin();
}

void loop() {
  server.handleClient();

  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      Serial.printf("Lat: %.6f, Lng: %.6f\n", latitude, longitude);
    }
  }
}
