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

// Initialize Variables
byte ledStatus = LOW;
int doorStatus = 0;
int lastDoorStatus = doorStatus;

Servo autoLock;

void setup() {
  // Initialize Sensors, Servos and LED's
  pinMode(LED_PIN, OUTPUT);
  pinMode(REED_SENSOR, INPUT_PULLUP);
  autoLock.write(0);

  // Connect to WiFi
  pinMode(LED_PIN, OUTPUT);
  pinMode(REED_SENSOR, INPUT_PULLUP);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    /* Blink the LED */
    digitalWrite(LED_PIN, ledStatus);
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
  }
  ledStatus = HIGH;
  digitalWrite(LED_PIN, ledStatus);

  // Login to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Set last status to current status and update current status
  lastDoorStatus = doorStatus;
  doorStatus = reedSensor();

  // See if the statuses are different (e.g.) the door has changed
  if (lastDoorStatus != doorStatus) {
    Firebase.setInt("doorStatus", doorStatus);
    setServo(doorStatus); // Send DoorStatus to Servo to Change Position
  }
  delay(500);
}

int reedSensor() {
  return digitalRead(REED_SENSOR);
}

void setServo(int doorStatus) {
  int pos = doorStatus * 90; // Set final position to door status
  autoLock.write(pos);
}
