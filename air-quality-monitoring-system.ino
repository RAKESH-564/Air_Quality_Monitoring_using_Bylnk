#define BLYNK_TEMPLATE_ID "TMPL3QPyDFxvp"
#define BLYNK_TEMPLATE_NAME "AIR QUALITY MONITORING"
#define BLYNK_AUTH_TOKEN "PmAIYNE15HucF-ILsrak47aQbEUPlks3"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "raki";  // type your wifi name
char pass[] = "123456789";  // type your wifi password

BlynkTimer timer;

int gas = A0;
int sensorThreshold = 600; // Threshold for gas detection
int ledPin = D5; // Pin connected to the LED

#define DHTPIN 2 // Connect Out pin to D4 in NODE MCU
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int analogSensor = analogRead(gas);
  Blynk.virtualWrite(V2, analogSensor);
  Serial.print("Gas Value: ");
  Serial.println(analogSensor);

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);

  // Check if the gas value exceeds the threshold
  if (analogSensor > sensorThreshold) {
    digitalWrite(ledPin, HIGH); // Turn on the LED
    Blynk.logEvent("pollution_alert", "Bad Air");
    Serial.println("Bad Air detected!");
  } else {
    digitalWrite(ledPin, LOW); // Turn off the LED
    Serial.println("Air quality is good.");
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(gas, INPUT);
  pinMode(ledPin, OUTPUT); // Initialize LED pin as output

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  
  // Change the interval to 2000 milliseconds (2 seconds)
  timer.setInterval(2000L, sendSensor); // Set interval for sending data and printing
}

void loop()
{
  Blynk.run();
  timer.run();
}