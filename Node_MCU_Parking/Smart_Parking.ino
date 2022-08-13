#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
#define TRIGGER_PIN D1
#define ECHO_PIN   D2
const char* ssid     ="POCO F1";
const char* password ="*******";
const char* host = "http://smartparking.epizy.com/";
WiFiClient client;
String url;
long duration, inches;
unsigned long timeout;
void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);
  // Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("POCO F1", "viraj@123");
}
float ultrasonic () {
  digitalWrite(TRIGGER_PIN, LOW); // Added this line 
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH); 
  inches = microsecondsToInches(duration);
  Serial.print(inches);
  Serial.println(" inches");
  return inches;
}
void loop() {
  Serial.print("distance of ");
  float in=float(ultrasonic());
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    Serial.println("WiFi connected");
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String buf;
  buf += F("http://smartparking.epizy.com/connect.php/?inches=");
  buf += String(in, 1);
    if (http.begin(client, buf)) {  // HTTP
      http.addHeader("Cookie", "__test=2f51372aa95e988aec42c6fc4eb7a8f1; expires=Thursday, September 14, 2023 at 1:26:44 PM; path=/");

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      Serial.println("WiFi Not connected");
    }
  }
  delay(1000);
}
long microsecondsToInches(long microseconds)
{
  return microseconds /74 /2;
}
