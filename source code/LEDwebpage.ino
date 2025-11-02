#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid     = "njuguna";
const char* password = "398#Pita";

WebServer server(80);

const int ledPin = 17;   
bool ledState = false;
bool blinkMode = false;
int brightness = 255;  // 0–255

// Generate HTML page
String getHTML() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;text-align:center;} button{padding:15px;font-size:18px;margin:5px;} input{width:80%;}</style>";
  html += "<script>function updateBrightness(val){fetch('/brightness?val='+val);}</script>";
  html += "</head><body>";
  html += "<h1>ESP32 LED Control</h1>";

  html += ledState ? "<p>LED is <b>ON</b></p>" : "<p>LED is <b>OFF</b></p>";
  if (blinkMode) html += "<p><b>BLINK MODE ACTIVE</b></p>";

  html += "<a href='/on'><button style='background:green;color:white;'>ON</button></a>";
  html += "<a href='/off'><button style='background:red;color:white;'>OFF</button></a>";
  html += "<a href='/blink'><button style='background:blue;color:white;'>BLINK</button></a>";

  html += "<p>Brightness</p>";
  html += "<input type='range' min='0' max='255' value='" + String(brightness) + "' oninput='updateBrightness(this.value)'>";
  
  html += "</body></html>";
  return html;
}

void handleRoot() { server.send(200, "text/html", getHTML()); }

void handleOn() {
  blinkMode = false;
  ledState = true;
  analogWrite(ledPin, brightness);
  server.send(200, "text/html", getHTML());
}

void handleOff() {
  blinkMode = false;
  ledState = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html", getHTML());
}

void handleBlink() {
  blinkMode = !blinkMode;  // toggle blink
  server.send(200, "text/html", getHTML());
}

void handleBrightness() {
  if (server.hasArg("val")) {
    brightness = server.arg("val").toInt();
    if (ledState && !blinkMode) {
      analogWrite(ledPin, brightness);
    }
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/blink", handleBlink);
  server.on("/brightness", handleBrightness);

  server.begin();
}

void loop() {
  server.handleClient();

  if (blinkMode) {
    analogWrite(ledPin, brightness);  // LED ON
    delay(500);
    analogWrite(ledPin, 0);           // LED OFF
    delay(500);
  }
}

