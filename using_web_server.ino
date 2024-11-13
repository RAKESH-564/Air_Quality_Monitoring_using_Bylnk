#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "AirQualityAP";
const char *password = "123456789";

int gas = A0;
int sensorThreshold = 600;
int ledPin = D5;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(gas, INPUT);

  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  dht.begin();

  server.on("/", HTTP_GET, [](){
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int analogSensor = analogRead(gas);

    String html = "<html>";
    html += "<head><title>Air Quality Monitoring</title><style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }";
    html += "h1 { font-size: 48px; color: #2E8B57; }";
    html += ".container { display: flex; justify-content: center; align-items: center; }";
    html += ".circle { width: 200px; height: 200px; border-radius: 50%; border: 5px solid #ddd; display: flex; justify-content: center; align-items: center; margin: 20px; }";
    html += ".circle span { font-size: 24px; font-weight: bold; color: #333; }";
    html += "</style></head><body>";
    html += "<h1>Air Quality Monitoring and Alerting System</h1>";
    html += "<div class='container'>";
    html += "<div class='circle'><span>Temp: " + String(t) + " °C</span></div>";
    html += "<div class='circle'><span>Humidity: " + String(h) + " %</span></div>";
    html += "<div class='circle'><span>Gas: " + String(analogSensor) + "</span></div>";
    html += "</div>";

    if (analogSensor > sensorThreshold) {
      digitalWrite(ledPin, HIGH);
      html += "<h2 style='color: red;'>Warning: Bad Air Quality Detected!</h2>";
    } else {
      digitalWrite(ledPin, LOW);
      html += "<h2 style='color: green;'>Air Quality is Good</h2>";
    }

    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
