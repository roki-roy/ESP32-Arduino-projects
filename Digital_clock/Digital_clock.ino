/*
# ESP32 Smart Digital Clock & Room Environment Monitor

## Project Overview

This project is an ESP32-based smart clock and room environment monitoring system. It uses a 0.96-inch OLED display to show real-time information and a TTP223 touch sensor to switch between different display modes.

The ESP32 connects to Wi-Fi and automatically synchronizes the current Bangladesh time using an NTP time server. A DHT11 sensor is also integrated to measure the room temperature and humidity.

## Main Features

- **Real-time Digital Clock**
  - 12-hour time format
  - AM/PM indication
  - Seconds display
  - Automatic time synchronization through Wi-Fi

- **Date & Day Display**
  - Current date
  - Month and year
  - Day of the week
  - Bangladesh (BD) time indication

- **Analog Clock**
  - Clock face with 12-hour marks
  - Hour, minute, and second hands
  - Smooth hour-hand positioning based on the current minute

- **Temperature & Humidity Monitoring**
  - DHT11 sensor
  - Temperature displayed in °C
  - Relative humidity displayed in %
  - Sensor readings updated every 2 seconds

- **Touch-Based Mode Switching**
  - TTP223 touch sensor
  - Each touch changes the display mode
  - Four available modes:
    1. Digital Clock
    2. Date + Day
    3. Analog Clock
    4. Temperature + Humidity

- **Time-Based Pixel-Art Icons**
  - Morning icon: 5:00 AM–11:59 AM
  - Afternoon icon: 12:00 PM–3:59 PM
  - Evening icon: 4:00 PM–6:59 PM
  - Night icon: 7:00 PM–4:59 AM

## Pin Configuration

| Component  | ESP32 Pin |
 -------------------------
| OLED SDA   | GPIO 21   |
| OLED SCL   | GPIO 22   |
| TTP223 OUT | GPIO 4    |
| DHT11 DATA | GPIO 15   |
--------------------------

The OLED uses I²C communication at address **0x3C**.

The project demonstrates practical use of ESP32, I²C OLED communication, Wi-Fi/NTP synchronization, sensor interfacing, touch input and embedded graphical programming.
*/

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"
#include <math.h>

#include <DHT.h>


// =================================================
//                  OLED
// =================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// =================================================
//               TTP223 TOUCH
// =================================================

#define TOUCH_PIN 4

int lastTouchState = LOW;


// =================================================
//                 DHT11
// =================================================

#define DHT_PIN 15
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

float temperature = 0.0;
float humidity = 0.0;

unsigned long lastDHTRead = 0;

const unsigned long DHT_INTERVAL = 2000;


// =================================================
//                 WIFI
// =================================================

const char* ssid = "ECE Tech Lab";
const char* password = "79987769";


// =================================================
//               BANGLADESH TIME
// =================================================

const char* ntpServer = "pool.ntp.org";

const long gmtOffset_sec = 6 * 60 * 60;
const int daylightOffset_sec = 0;


// =================================================
//               SCREEN MODE
// =================================================

// 0 = Digital Clock
// 1 = Date + Day
// 2 = Analog Clock
// 3 = Temperature + Humidity

int screenMode = 0;


// =================================================
//                  SETUP
// =================================================

void setup() {

  Serial.begin(115200);


  // ===============================================
  //             TTP223
  // ===============================================

  pinMode(TOUCH_PIN, INPUT);


  // ===============================================
  //               OLED
  // ===============================================

  Wire.begin(21, 22);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {

    Serial.println("OLED not found!");

    while (1);
  }


  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);


  // ===============================================
  //               DHT11
  // ===============================================

  dht.begin();

  Serial.println("DHT11 started");


  // ===============================================
  // START SCREEN
  // ===============================================

  display.setTextSize(2);

  display.setCursor(30, 20);

  display.println("CLOCK");

  display.display();

  delay(1500);


  // ===============================================
  // WIFI CONNECTING
  // ===============================================

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(20, 25);

  display.println("Connecting WiFi...");

  display.display();


  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");


  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }


  Serial.println();

  Serial.println("WiFi Connected!");

  Serial.print("IP: ");

  Serial.println(WiFi.localIP());


  // ===============================================
  // NTP
  // ===============================================

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer
  );


  // ===============================================
  // WAIT FOR TIME
  // ===============================================

  struct tm timeInfo;

  while (!getLocalTime(&timeInfo)) {

    delay(500);

    Serial.println("Waiting for time...");
  }


  Serial.println("Time synchronized!");


  // ===============================================
  // FIRST DHT READING
  // ===============================================

  readDHT();
}


// =================================================
// LOOP
// =================================================

void loop() {

  // ===============================================
  // CHECK TOUCH
  // ===============================================

  int touchState = digitalRead(TOUCH_PIN);


  if (
    touchState == HIGH &&
    lastTouchState == LOW
  ) {

    // =============================================
    // CHANGE MODE
    // =============================================

    screenMode++;

    if (screenMode > 3) {

      screenMode = 0;
    }


    Serial.print("Screen changed to: ");

    Serial.println(screenMode);


    // =============================================
    // DEBOUNCE
    // =============================================

    delay(250);
  }


  lastTouchState = touchState;


  // ===============================================
  // READ DHT11
  // ===============================================

  if (
    millis() - lastDHTRead >= DHT_INTERVAL
  ) {

    readDHT();
  }


  // ===============================================
  // GET CURRENT TIME
  // ===============================================

  struct tm timeInfo;

  if (!getLocalTime(&timeInfo)) {

    Serial.println("Time error");

    delay(1000);

    return;
  }


  // ===============================================
  // DIGITAL CLOCK
  // ===============================================

  if (screenMode == 0) {

    showClock(timeInfo);
  }


  // ===============================================
  // DATE SCREEN
  // ===============================================

  else if (screenMode == 1) {

    showDate(timeInfo);
  }


  // ===============================================
  // ANALOG CLOCK
  // ===============================================

  else if (screenMode == 2) {

    showAnalogClock(timeInfo);
  }


  // ===============================================
  // TEMPERATURE + HUMIDITY
  // ===============================================

  else if (screenMode == 3) {

    showTemperatureHumidity();
  }


  delay(100);
}


// =================================================
//              READ DHT11
// =================================================

void readDHT() {

  lastDHTRead = millis();

  float newHumidity = dht.readHumidity();

  float newTemperature = dht.readTemperature();


  // ===============================================
  // CHECK SENSOR ERROR
  // ===============================================

  if (
    isnan(newHumidity) ||
    isnan(newTemperature)
  ) {

    Serial.println("DHT11 reading failed!");

    return;
  }


  humidity = newHumidity;

  temperature = newTemperature;


  // ===============================================
  // SERIAL MONITOR
  // ===============================================

  Serial.print("Temperature: ");

  Serial.print(temperature);

  Serial.println(" C");


  Serial.print("Humidity: ");

  Serial.print(humidity);

  Serial.println(" %");
}


// =================================================
// MAIN DIGITAL CLOCK
// =================================================

void showClock(struct tm timeInfo) {

  // ===============================================
  // 12-HOUR TIME
  // ===============================================

  char hourMinute[6];

  strftime(
    hourMinute,
    sizeof(hourMinute),
    "%I:%M",
    &timeInfo
  );


  // ===============================================
  // SECOND
  // ===============================================

  char second[3];

  strftime(
    second,
    sizeof(second),
    "%S",
    &timeInfo
  );


  // ===============================================
  // AM / PM
  // ===============================================

  char ampm[3];

  strftime(
    ampm,
    sizeof(ampm),
    "%p",
    &timeInfo
  );


  // ===============================================
  // CLEAR SCREEN
  // ===============================================

  display.clearDisplay();


  // ===============================================
  // MAIN TIME
  // ===============================================

  display.setTextSize(3);

  display.setCursor(4, 4);

  display.print(hourMinute);


  // ===============================================
  // SECONDS
  // SMALL
  // ===============================================

  display.setTextSize(1);

  display.setCursor(105, 12);

  display.print(second);


  // ===============================================
  // AM / PM
  // LARGE
  // ===============================================

  display.setTextSize(2);

  display.setCursor(38, 38);

  display.print(ampm);


  // ===============================================
  // TIME BASED PIXEL EMOJI
  // ===============================================

  showTimeEmoji(
    timeInfo.tm_hour
  );


  // ===============================================
  // DISPLAY
  // ===============================================

  display.display();
}


// =================================================
// TIME BASED PIXEL-ART EMOJI
// =================================================

void showTimeEmoji(int hour) {

  // ===============================================
  // MORNING
  // 5:00 AM - 11:59 AM
  // ===============================================

  if (
    hour >= 5 &&
    hour < 12
  ) {

    drawMorningEmoji();
  }


  // ===============================================
  // AFTERNOON
  // 12:00 PM - 3:59 PM
  // ===============================================

  else if (
    hour >= 12 &&
    hour < 16
  ) {

    drawAfternoonEmoji();
  }


  // ===============================================
  // EVENING
  // 4:00 PM - 6:59 PM
  // ===============================================

  else if (
    hour >= 16 &&
    hour < 19
  ) {

    drawEveningEmoji();
  }


  // ===============================================
  // NIGHT
  // 7:00 PM - 4:59 AM
  // ===============================================

  else {

    drawNightEmoji();
  }
}


// =================================================
// MORNING PIXEL-ART
// =================================================

void drawMorningEmoji() {

  int x = 103;
  int y = 50;


  // Sun

  display.fillCircle(
    x,
    y - 4,
    6,
    SSD1306_WHITE
  );


  // Horizon

  display.drawLine(
    x - 12,
    y + 5,
    x + 12,
    y + 5,
    SSD1306_WHITE
  );


  // Rays

  display.drawLine(
    x,
    y - 13,
    x,
    y - 10,
    SSD1306_WHITE
  );

  display.drawLine(
    x - 9,
    y - 11,
    x - 7,
    y - 9,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 9,
    y - 11,
    x + 7,
    y - 9,
    SSD1306_WHITE
  );

  display.drawLine(
    x - 13,
    y - 4,
    x - 10,
    y - 4,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 10,
    y - 4,
    x + 13,
    y - 4,
    SSD1306_WHITE
  );


  // Small rising lines

  display.drawLine(
    x - 10,
    y + 1,
    x - 6,
    y - 3,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 10,
    y + 1,
    x + 6,
    y - 3,
    SSD1306_WHITE
  );
}


// =================================================
// AFTERNOON PIXEL-ART
// =================================================

void drawAfternoonEmoji() {

  int x = 103;
  int y = 50;


  // Main sun

  display.fillCircle(
    x,
    y - 5,
    7,
    SSD1306_WHITE
  );


  // Rays

  display.drawLine(
    x,
    y - 16,
    x,
    y - 12,
    SSD1306_WHITE
  );

  display.drawLine(
    x,
    y + 2,
    x,
    y + 6,
    SSD1306_WHITE
  );

  display.drawLine(
    x - 11,
    y - 5,
    x - 15,
    y - 5,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 11,
    y - 5,
    x + 15,
    y - 5,
    SSD1306_WHITE
  );


  // Diagonal rays

  display.drawLine(
    x - 9,
    y - 14,
    x - 7,
    y - 11,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 9,
    y - 14,
    x + 7,
    y - 11,
    SSD1306_WHITE
  );

  display.drawLine(
    x - 9,
    y + 4,
    x - 7,
    y + 1,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 9,
    y + 4,
    x + 7,
    y + 1,
    SSD1306_WHITE
  );
}


// =================================================
// EVENING PIXEL-ART
// =================================================

void drawEveningEmoji() {

  int x = 103;
  int y = 50;


  // Setting sun

  display.fillCircle(
    x,
    y - 1,
    7,
    SSD1306_WHITE
  );


  // Horizon

  display.drawLine(
    x - 14,
    y + 5,
    x + 14,
    y + 5,
    SSD1306_WHITE
  );


  // Sun rays

  display.drawLine(
    x,
    y - 12,
    x,
    y - 9,
    SSD1306_WHITE
  );

  display.drawLine(
    x - 10,
    y - 1,
    x - 13,
    y - 1,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 10,
    y - 1,
    x + 13,
    y - 1,
    SSD1306_WHITE
  );


  // Horizon reflection

  display.drawLine(
    x - 9,
    y + 9,
    x - 3,
    y + 9,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 3,
    y + 9,
    x + 9,
    y + 9,
    SSD1306_WHITE
  );

  display.drawLine(
    x - 6,
    y + 12,
    x + 6,
    y + 12,
    SSD1306_WHITE
  );
}


// =================================================
// NIGHT PIXEL-ART
// =================================================

void drawNightEmoji() {

  int x = 103;
  int y = 50;


  // Moon

  display.fillCircle(
    x,
    y - 5,
    10,
    SSD1306_WHITE
  );


  // Cut-out

  display.fillCircle(
    x + 5,
    y - 9,
    10,
    SSD1306_BLACK
  );


  // Stars

  display.fillRect(
    x - 17,
    y - 14,
    2,
    2,
    SSD1306_WHITE
  );

  display.fillRect(
    x - 14,
    y + 3,
    2,
    2,
    SSD1306_WHITE
  );

  display.fillRect(
    x + 14,
    y + 6,
    2,
    2,
    SSD1306_WHITE
  );

  display.drawPixel(
    x + 13,
    y - 15,
    SSD1306_WHITE
  );
}


// =================================================
// ANALOG CLOCK
// =================================================

void showAnalogClock(struct tm timeInfo) {

  // ===============================================
  // CLOCK CENTER
  // ===============================================

  int centerX = 64;
  int centerY = 32;

  int radius = 28;


  // ===============================================
  // GET CURRENT TIME
  // ===============================================

  int hour   = timeInfo.tm_hour;
  int minute = timeInfo.tm_min;
  int second = timeInfo.tm_sec;


  // ===============================================
  // CLEAR SCREEN
  // ===============================================

  display.clearDisplay();


  // ===============================================
  // OUTER CLOCK CIRCLE
  // ===============================================

  display.drawCircle(
    centerX,
    centerY,
    radius,
    SSD1306_WHITE
  );


  // ===============================================
  // HOUR MARKS
  // ===============================================

  for (int i = 0; i < 12; i++) {

    float angle =
      i * 30.0 - 90.0;

    float rad =
      angle * PI / 180.0;


    int x1 =
      centerX + cos(rad) * (radius - 4);

    int y1 =
      centerY + sin(rad) * (radius - 4);


    int x2 =
      centerX + cos(rad) * (radius - 1);

    int y2 =
      centerY + sin(rad) * (radius - 1);


    display.drawLine(
      x1,
      y1,
      x2,
      y2,
      SSD1306_WHITE
    );
  }


  // ===============================================
  // SECOND HAND
  // ===============================================

  float secondAngle =
    second * 6.0 - 90.0;

  float secondRad =
    secondAngle * PI / 180.0;


  int secondX =
    centerX + cos(secondRad) * 23;

  int secondY =
    centerY + sin(secondRad) * 23;


  display.drawLine(
    centerX,
    centerY,
    secondX,
    secondY,
    SSD1306_WHITE
  );


  // ===============================================
  // MINUTE HAND
  // ===============================================

  float minuteAngle =
    minute * 6.0 - 90.0;

  float minuteRad =
    minuteAngle * PI / 180.0;


  int minuteX =
    centerX + cos(minuteRad) * 19;

  int minuteY =
    centerY + sin(minuteRad) * 19;


  display.drawLine(
    centerX,
    centerY,
    minuteX,
    minuteY,
    SSD1306_WHITE
  );


  // ===============================================
  // HOUR HAND
  // ===============================================

  float hourAngle =
    (hour % 12) * 30.0
    + minute * 0.5
    - 90.0;

  float hourRad =
    hourAngle * PI / 180.0;


  int hourX =
    centerX + cos(hourRad) * 13;

  int hourY =
    centerY + sin(hourRad) * 13;


  display.drawLine(
    centerX,
    centerY,
    hourX,
    hourY,
    SSD1306_WHITE
  );


  // ===============================================
  // CENTER DOT
  // ===============================================

  display.fillCircle(
    centerX,
    centerY,
    2,
    SSD1306_WHITE
  );

  // Analog sign

  display.setTextSize(1);
  display.setCursor(3,1);
  display.print("Analog");

  // NEXT ICON

  display.setTextSize(1);
  display.setCursor(112, 55);
  display.print("->");

  // ===============================================
  // DISPLAY
  // ===============================================

  display.display();
}


// =================================================
// TEMPERATURE + HUMIDITY SCREEN
// =================================================

void showTemperatureHumidity() {

  display.clearDisplay();


  // ===============================================
  // TITLE
  // ===============================================

  display.setTextSize(1);

  display.setCursor(4, 2);

  display.print("ROOM ENVIRONMENT");


  // ===============================================
  // TEMPERATURE ICON / LABEL
  // ===============================================

  display.setTextSize(1);

  display.setCursor(8, 20);

  display.print("TEMP");


  // ===============================================
  // TEMPERATURE VALUE
  // ===============================================

  display.setTextSize(2);

  display.setCursor(38, 17);

  if (isnan(temperature)) {

    display.print("--");
  }
  else {

    display.print(temperature, 1);
  }

  display.print(" C");


  // ===============================================
  // HUMIDITY LABEL
  // ===============================================

  display.setTextSize(1);

  display.setCursor(8, 43);

  display.print("HUM");


  // ===============================================
  // HUMIDITY VALUE
  // ===============================================

  display.setTextSize(2);

  display.setCursor(38, 40);

  if (isnan(humidity)) {

    display.print("--");
  }
  else {

    display.print(humidity, 1);
  }

  display.print(" %");


  // ===============================================
  // DISPLAY
  // ===============================================

  display.display();
}


// =================================================
// DATE + DAY
// =================================================

void showDate(struct tm timeInfo) {

  // ===============================================
  // DAY NUMBER
  // ===============================================

  char dayNumber[3];

  strftime(
    dayNumber,
    sizeof(dayNumber),
    "%d",
    &timeInfo
  );


  // ===============================================
  // MONTH
  // ===============================================

  char monthString[10];

  strftime(
    monthString,
    sizeof(monthString),
    "%b",
    &timeInfo
  );


  // ===============================================
  // YEAR
  // ===============================================

  char yearString[5];

  strftime(
    yearString,
    sizeof(yearString),
    "%Y",
    &timeInfo
  );


  // ===============================================
  // DAY NAME
  // ===============================================

  char dayString[15];

  strftime(
    dayString,
    sizeof(dayString),
    "%A",
    &timeInfo
  );


  // ===============================================
  // CLEAR SCREEN
  // ===============================================

  display.clearDisplay();


  // ===============================================
  // TODAY
  // ===============================================

  display.setTextSize(1);

  display.setCursor(3, 1);

  display.println("TODAY");

  // country set BD

  display.setTextSize(1);

  display.setCursor(113, 1);

  display.println("BD");


  // ===============================================
  // DATE
  // ===============================================

  display.setTextSize(2);

  display.setCursor(25, 17);

  display.print(dayNumber);

  display.print(" ");

  display.print(monthString);


  // ===============================================
  // YEAR
  // ===============================================

  display.setTextSize(2);

  display.setCursor(35, 36);

  display.print(yearString);


  // ===============================================
  // DAY
  // ===============================================

  display.setTextSize(1);

  display.setCursor(3, 55);

  display.print(dayString);

  // next icon (->)

  display.setTextSize(1);

  display.setCursor(112, 55);

  display.print("->");

  // ===============================================
  // DISPLAY
  // ===============================================

  display.display();
}