#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>

// Login data for WiFi and Firebase
#define FIREBASE_HOST "ajario-2b883.firebaseio.com"
#define FIREBASE_AUTH "uJyP8aE33JAbONiSSW6SaEZhvPB6wLLFsWqLLqIb"
#define WIFI_SSID "IllinoisNet_Guest"
#define WIFI_PASSWORD ""

// Input/Output Pin Locations
const int LED_PIN = 5; // Onboard LED, indicates WiFi Status
const int REED_SENSOR = 12; // Digial Input, Measures Reed Sensor
const int SERVO = 13; // Digital Output, controls servomotor to open lock


void setup() {
  // initialize all in/out connections
  pinMode(LED_PIN, OUTPUT);
  pinMode(REED_SENSOR, INPUT_PULLUP);
  pinMode(SERVO, OUTPUT);

  // Initialize LED status
  byte ledStatus = LOW;
  digitalWrite(LED_PIN, ledStatus);

  // Initialize Servo Position
  int pos = 0;
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);

    /* Blink the LED */
    digitalWrite(LED_PIN, ledStatus);
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
  }

  ledStatus = HIGH;
  digitalWrite(LED_PIN, ledStatus);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
  // set value
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}
