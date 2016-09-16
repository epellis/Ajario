#include <ESP8266WiFi.h>  // Include ESP 8266 WiFi Library
#include <Phant.h>  // Include Sparkfun Phant Libary for data logging

/* WiFi Definitions */
const char WiFiSSID[] = "IllinoisNet_Guest";
const char WiFiPSK[] = "";

/* Pin Definitions */
const int LED_PIN = 5; // Onboard LED to indicate WiFi Status
const int DIGITAL_PIN = 12; // Digital pin measuring Reed Switch

/* Refresh Rate */
const int refreshRate = 200;  // Refresh rate of 200 ms

/* Phant keys */
const char PhantHost[] = "data.sparkfun.com";
const char PublicKey[] = "v0O96NjjxAcy3vMRLGmp";
const char PrivateKey[] = "aPdqZ955g7czPBgjeJAk";

/* Device name */
const String deviceID = "RM 038A, Ned Ellis";

/* General Variables */
int lastState = 0;  // Last status of the door

void setup() {
  initHardware();
  connectWiFi();
  digitalWrite(LED_PIN, HIGH);  // Turn on the green light to denote a successful startup
}

void loop() {
  if (lastState != digitalRead(DIGITAL_PIN)) {
    lastState = digitalRead(DIGITAL_PIN);
    postData();
  }
  delay(refreshRate);
}

int postData() {
  /* Turn on light to indicate post */
  digitalWrite(LED_PIN, HIGH);

  /* Declare an object named phant with the correct keys */
  Phant phant(PhantHost, PublicKey, PrivateKey);

  /* Create a string to express the state of the pin */
  String state;
  if (digitalRead(DIGITAL_PIN) == 0) {
    state = "Open";
  }
  else {
    state = "Closed";
  }

  /* Add the state of the door and the time to the data post */
  phant.add("state", state);
  phant.add("time", millis());

  /* Connect to data.sparkfun.com and post data */
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(PhantHost, httpPort)) 
  {
    // If we fail to connect, return 0.
    return 0;
  }
  // If we successfully connected, print our Phant post:
  client.print(phant.post());

  /* Turn off LED */
  digitalWrite(LED_PIN, LOW);

  return 1;
}

void connectWiFi()  // Credits to www.sparkfun.com
{
  byte ledStatus = LOW;

  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    
    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    delay(100);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
}

void initHardware() // Credits to www.sparkfun.com
{
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}
