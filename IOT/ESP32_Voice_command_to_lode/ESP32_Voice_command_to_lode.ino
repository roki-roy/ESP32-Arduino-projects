/*
===================================================================================================================================
## Arduino IDE — ESP32 Voice Control System

The Arduino IDE is used to program the **ESP32 DevKit** as the main hardware controller and local HTTP server.

### Main Functions

* Connects the ESP32 to the local Wi-Fi network.
* Starts an HTTP server on port 80.
* Displays the ESP32 IP address on a **16×2 I2C LCD**.
* Controls a **4-channel active-LOW relay module**.
* Currently uses:

  * Relay 1 — Light — GPIO 26
  * Relay 2 — AC — GPIO 27
* Receives HTTP commands from the external webpage.
* Processes relay commands and switches the connected loads ON/OFF.
* Provides CORS headers so the VS Code webpage can communicate with the ESP32.

### Supported HTTP Commands

* `/relay1/on` → Light ON
* `/relay1/off` → Light OFF
* `/relay2/on` → AC ON
* `/relay2/off` → AC OFF
* `/all/on` → All connected loads ON
* `/all/off` → All connected loads OFF

### Important

The Arduino IDE code **does not contain the webpage HTML, CSS, or JavaScript anymore**. The ESP32 only handles Wi-Fi communication, HTTP requests, and relay control.
============================================================================================================================================================================
workflow:

**  Voice Command → VS Code Webpage → JavaScript → Wi-Fi → ESP32 → Relay → Electrical Load   **
*/

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "ECE Tech Lab";
const char* password = "79987769";

WiFiServer server(80);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==============================
// RELAY PINS
// ==============================

const int Relay_1 = 26;   // Light
const int Relay_2 = 27;   // AC


// ==============================
// SETUP
// ==============================

void setup() {

  Serial.begin(9600);

  // ==============================
  // RELAY SETUP
  // ==============================

  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);

  // Active LOW relay
  // LOW  = ON
  // HIGH = OFF

  digitalWrite(Relay_1, HIGH);
  digitalWrite(Relay_2, HIGH);


  // ==============================
  //         LCD SETUP
  // ==============================

  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Voice Control");

  lcd.setCursor(0, 1);
  lcd.print("   System       ");

  delay(3000);

  lcd.clear();


  // ==============================
  //       WIFI CONNECT
  // ==============================

  WiFi.begin(ssid, password);

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());


  // ==============================
  //      SHOW IP ON LCD
  // ==============================

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ESP32 IP:");

  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());


  // ==============================
  // START SERVER
  // ==============================

  server.begin();

  Serial.println("Web Server Started");
}


// ==============================
//         logic  LOOP
// ==============================

void loop() {

  WiFiClient client = server.available();

  if (!client) {
    return;
  }


  // ==============================
  //     WAIT FOR REQUEST
  // ==============================

  while (client.connected() && !client.available()) {
    delay(1);
  }


  // ==============================
  //       READ REQUEST
  // ==============================

  String request = client.readStringUntil('\r');

  Serial.println(request);


  // ==============================
  //       LIGHT ON
  // ==============================

  if (request.indexOf("GET /relay1/on") >= 0) {

    digitalWrite(Relay_1, LOW);

    Serial.println("Light ON");
  }


  // ==============================
  //         LIGHT OFF
  // ==============================

  if (request.indexOf("GET /relay1/off") >= 0) {

    digitalWrite(Relay_1, HIGH);

    Serial.println("Light OFF");
  }


  // ==============================
  //          AC ON
  // ==============================

  if (request.indexOf("GET /relay2/on") >= 0) {

    digitalWrite(Relay_2, LOW);

    Serial.println("AC ON");
  }


  // ==============================
  //         AC OFF
  // ==============================

  if (request.indexOf("GET /relay2/off") >= 0) {

    digitalWrite(Relay_2, HIGH);

    Serial.println("AC OFF");
  }


  // ==============================
  //          ALL ON
  // ==============================

  if (request.indexOf("GET /all/on") >= 0) {

    digitalWrite(Relay_1, LOW);
    digitalWrite(Relay_2, LOW);

    Serial.println("ALL ON");
  }


  // ==============================
  //          ALL OFF
  // ==============================

  if (request.indexOf("GET /all/off") >= 0) {

    digitalWrite(Relay_1, HIGH);
    digitalWrite(Relay_2, HIGH);

    Serial.println("ALL OFF");
  }


  // ==============================
  //        HTTP RESPONSE
  // ==============================

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: GET, OPTIONS");
  client.println("Access-Control-Allow-Headers: Content-Type");
  client.println("Connection: close");
  client.println();


  delay(1);

  client.stop();
}