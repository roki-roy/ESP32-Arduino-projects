/* Project: Smart Car Parking Alert System
 ==================================================================
  Description:
  This project uses an ESP32 and ultrasonic sensor to measure the
  distance between a car and the parking area. LEDs, a buzzer, an
  LCD, and a servo motor provide visual and audio alerts based on
  the detected distance.

  Features:
  - Real-time distance measurement
  - GO and STOP parking alerts
  - Three-level LED indication
  - LCD distance and status display
  - Distance-based buzzer alerts
  - Servo-controlled parking barrier
  - Non-blocking buzzer timing using millis()
  =====================================================================
*/

#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);          // lcd Model
Servo myservo;

// ---------- Pins ----------
const int white_LED = 18;
const int Green_LED = 19;
const int Red_LED = 23;

const int Buzzer = 13;

// Ultrasonic pwm pin
const int trigPin = 5;
const int ECHO_PIN = 35;

// ---------- Buzzer timing ----------
unsigned long buzzerStartTime = 0;

bool buzzerState = false;
int buzzerPlayed = 0;

// ---------- Distance ----------
int distance = 0;


void setup() {

  myservo.attach(2);       // Servo Pin
  myservo.write(90);       // Empity Servo Position Declaration

  // led condition
  pinMode(white_LED, OUTPUT);
  pinMode(Green_LED, OUTPUT);
  pinMode(Red_LED, OUTPUT);

  pinMode(Buzzer, OUTPUT);    // Buzzer Condition

  // Ultraconic Pin Condition
  pinMode(trigPin, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Wire.begin(21, 22);     // lcd pin

  lcd.init();
  lcd.backlight();

  // Active Animation
  lcd.setCursor(0, 0);
  lcd.print("* System Ready *");

  lcd.setCursor(0, 1);
  lcd.print("Parking alert ON");
  delay(4000);
}


void loop() {

  // ---------- Ultrasonic Sensor ----------

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseInLong(ECHO_PIN, HIGH, 30000);

  distance = duration * 0.0343 / 2;


  // =================================================
  //             GO ZONE → 10 cm to 15 cm
  // =================================================

  if (distance <= 15 && distance > 9) {

    digitalWrite(white_LED, HIGH);
    digitalWrite(Green_LED, LOW);
    digitalWrite(Red_LED, LOW);

    myservo.write(90);

    lcd.setCursor(0, 0);
    lcd.print("Gap: ");
    lcd.print(distance);
    lcd.print(" cm          ");

    lcd.setCursor(0, 1);
    lcd.print("      GO        ");

    // Buzzer ON
    if (!buzzerState) {

      digitalWrite(Buzzer, HIGH);

      buzzerState = true;
      buzzerStartTime = millis();
    }

    // 200 ms after Buzzer OFF
    if (buzzerState &&
        millis() - buzzerStartTime >= 200) {

      digitalWrite(Buzzer, LOW);

      buzzerState = false;
    }
  }


  // =================================================
  // 2. SLOW ZONE → 4 cm to 9 cm
  // =================================================

  else if (distance <= 9 && distance > 3) {

    digitalWrite(Green_LED, HIGH);
    digitalWrite(white_LED, LOW);
    digitalWrite(Red_LED, LOW);

    myservo.write(90);

    lcd.setCursor(0, 0);
    lcd.print("Gap: ");
    lcd.print(distance);
    lcd.print(" cm          ");

    lcd.setCursor(0, 1);
    lcd.print("     SLOW       ");

    // Buzzer ON
    if (!buzzerState) {

      digitalWrite(Buzzer, HIGH);

      buzzerState = true;
      buzzerStartTime = millis();
    }

    // 100 ms পরে Buzzer OFF
    if (buzzerState &&
        millis() - buzzerStartTime >= 100) {

      digitalWrite(Buzzer, LOW);

      buzzerState = false;
    }
  }


  // =================================================
  //            STOP ZONE → 3 cm Below
  // =================================================

  else if (distance <= 3 && distance > 0) {

    digitalWrite(Red_LED, HIGH);
    digitalWrite(Green_LED, LOW);
    digitalWrite(white_LED, LOW);

    myservo.write(0);

    lcd.setCursor(0, 0);
    lcd.print("Gap: ");
    lcd.print(distance);
    lcd.print(" cm          ");

    lcd.setCursor(0, 1);
    lcd.print("    STOP       ");


    // STOP buzzer just once
    if (buzzerPlayed == 0) {

      digitalWrite(Buzzer, HIGH);

      buzzerStartTime = millis();

      buzzerState = true;

      buzzerPlayed = 1;
    }


    // 1 second পরে Buzzer OFF
    if (buzzerState &&
        millis() - buzzerStartTime >= 1000) {

      digitalWrite(Buzzer, LOW);

      buzzerState = false;
    }
  }


  // =================================================
  //       SAFE / NO CAR → 15 cm to out of area
  // =================================================

  else {

    digitalWrite(Red_LED, LOW);
    digitalWrite(Green_LED, LOW);
    digitalWrite(white_LED, LOW);

    digitalWrite(Buzzer, LOW);

    buzzerState = false;

    buzzerPlayed = 0;

    myservo.write(90);

    lcd.setCursor(0, 0);
    lcd.print("  Parking Area  ");

    lcd.setCursor(0, 1);
    lcd.print("* Only For Car *");
  }
  delay(50);
}
