#include <ESP8266WiFi.h>


#define TRIGGER_PIN D1
#define ECHO_PIN   D2

 
const char* ssid     ="realme X2";
const char* password ="modi@1904";
const char* host = "192.168.43.38";


WiFiClient client;

String url;

long duration, inches;
unsigned long timeout;

void setup() {

  Serial.begin(9600);
  delay(10);

  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);

// We start by connecting to a WiFi network

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.mode (WIFI_STA); 
WiFi.begin(ssid, password);

while (WiFi.status()!= WL_CONNECTED){
  delay(500);
  Serial.println("WiFi Not connected");
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}

void ultrasonic () {
  digitalWrite(TRIGGER_PIN, LOW); // Added this line 
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH); 
  inches = microsecondsToInches(duration);
  Serial.print(inches);
  Serial.println(" inches");
}

void loop() {
  Serial.print("distance of ");
  ultrasonic();

    Serial.print("connected "); 
    Serial.println (host);

// Use WiFiClient class to create TCP connections

  WiFiClient client;
  const int httpPort = 3306;
  if (!client.connect (host, httpPort)) {
  //Serial.println("connection faild");
  return;
  }

  // This will send the request to the server
  client.print(String("GET http://localhost/ES/connect.php")+ 
    ("&inches=") + inches + 
    "HTTP/1.1\r\n" + 
    "Host: " + host + "\r\n" + 
    "Connection: close\r\n\r\n");
    
    unsigned long timeout= millis();
    while (client.available() == 0) {
    if (millis() - timeout >1000) {
    Serial.println(">>> client Timeout !"); 
    client.stop();
    return;

    }
  }


// Read all the lines of the reply from server and print them to Serial 
while (client.available()) { 
  String line = client.readStringUntil('\r');
  Serial.print(line);

}

      Serial.println();
      Serial.println("closing connection");
}
long microsecondsToInches(long microseconds)
{
  return microseconds /74 /2;
}
