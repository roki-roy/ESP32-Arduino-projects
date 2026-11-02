/*
====================================================
       ESP32 BLUETOOTH SMART RC CAR
====================================================

Controller       : ESP32 DevKit V1
Programming      : C++ (Arduino Framework)
Bluetooth Name   : RC_CAR
Motor Driver     : L298N
Obstacle Sensor  : HC-SR04

----------------------------------------------------
1. MOTOR DRIVER PIN CONFIGURATION
----------------------------------------------------

IN1 = GPIO 26
IN2 = GPIO 27
IN3 = GPIO 18
IN4 = GPIO 19

ENA = GPIO 25
ENB = GPIO 23

----------------------------------------------------
2. ULTRASONIC SENSOR
----------------------------------------------------

TRIG = GPIO 5
ECHO = GPIO 34

STOP_DISTANCE = 15 cm
Sensor Interval = 60 ms

Features:
- Obstacle Detection
- Forward Movement Protection
- Stop When No Echo
- Automatic Distance Measurement

----------------------------------------------------
3. BUZZER
----------------------------------------------------

BUZZER = GPIO 22

Features:
- Buzzer ON when obstacle is within 15 cm
- Buzzer OFF when obstacle is outside 15 cm

----------------------------------------------------
4. RGB LED
----------------------------------------------------

RED   = GPIO 13
GREEN = GPIO 14

Commands:
X = RGB LEDs ON
x = RGB LEDs OFF

----------------------------------------------------
5. FRONT AND BACK LED
----------------------------------------------------

FRONT LED = GPIO 4

BACK LED 1 = GPIO 32
BACK LED 2 = GPIO 33

Commands:
W = Front LED ON
w = Front LED OFF

U = Back LEDs ON
u = Back LEDs OFF

----------------------------------------------------
6. BLUETOOTH MOVEMENT COMMANDS
----------------------------------------------------

F = Forward
B = Backward
L = Turn Left
R = Turn Right
S = Stop

----------------------------------------------------
7. SPEED CONTROL
----------------------------------------------------

0 = PWM 40
1 = PWM 80
2 = PWM 100
3 = PWM 120
4 = PWM 140
5 = PWM 160
6 = PWM 180
7 = PWM 200
8 = PWM 230
9 = PWM 255

PWM Range = 0 - 255

----------------------------------------------------
8. SAFETY SYSTEM
----------------------------------------------------

Bluetooth Timeout = 500 ms

- Car stops if Bluetooth commands stop arriving.
- Car stops when an obstacle is detected within
  the safety distance during forward movement.
- Car stops if ultrasonic sensor returns No Echo.
- Initial motor state is STOP.

----------------------------------------------------
9. CURRENT FEATURES
----------------------------------------------------

[✓] Bluetooth RC Control
[✓] Forward / Backward Movement
[✓] Left / Right Turning
[✓] 0-9 Speed Control
[✓] Ultrasonic Obstacle Detection
[✓] Automatic Buzzer Alert
[✓] RGB LED Control
[✓] Front LED Control
[✓] Back LED Control
[✓] Bluetooth Safety Timeout

----------------------------------------------------
10. FUTURE IMPROVEMENTS
----------------------------------------------------

[ ] Active Motor Braking
[ ] Improved Obstacle Auto-Braking
[ ] Distance-Based Buzzer Beeping
[ ] RGB LED Status Indicators
[ ] Non-blocking Ultrasonic Measurement
[ ] Hold-to-Run Bluetooth Control
*/
#include "BluetoothSerial.h"

BluetoothSerial mybluetooth;

// ================================
// Motor Driver Pins
// ================================
const int IN1 = 26;
const int IN2 = 27;
const int IN3 = 18;
const int IN4 = 19;

const int ENA = 25;
const int ENB = 23;

// ================================
// Ultrasonic Sensor Pins
// ================================
const int TRIG = 5;
const int ECHO = 34;

// ================================
// Buzzer Pin
// ================================
const int BUZZER = 22;

// ================================
// RGB LED Pins
// ================================
const int RGB1_R = 13;
const int RGB1_G = 14;

// ================================
// Front and Back LED Pins
// ================================
const int FRONT_LED = 4;

const int BACK_LED1 = 32;
const int BACK_LED2 = 33;

// ================================
// Settings
// ================================
const int STOP_DISTANCE = 15;

int motorSpeed =0;

unsigned long lastCommandTime = 0;
const unsigned long timeout = 500;

unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 60;

float distanceCm = 0;

// Current movement command
char currentCommand = 'S';


// ================================
// RGB LED Control
// ================================
void rgbOn() {
  digitalWrite(RGB1_R, HIGH);
  digitalWrite(RGB1_G, HIGH);
}

void rgbOff() {

  digitalWrite(RGB1_R, LOW);
  digitalWrite(RGB1_G, LOW);
}


// ================================
// Front LED Control
// ================================
void frontLedOn() {
  digitalWrite(FRONT_LED, HIGH);
}

void frontLedOff() {
  digitalWrite(FRONT_LED, LOW);
}


// ================================
// Back LED Control
// ================================
void backLedOn() {
  digitalWrite(BACK_LED1, HIGH);
  digitalWrite(BACK_LED2, HIGH);
}

void backLedOff() {
  digitalWrite(BACK_LED1, LOW);
  digitalWrite(BACK_LED2, LOW);
}


// ================================
// Stop Motors
// ================================
void stopMotors() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// ================================
// Stop Car
// ================================
void stopCar() {

  stopMotors();
  currentCommand = 'S';
}


// ================================
// Measure Distance
// ================================
float getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  unsigned long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0) {
    return 0;
  }

  return duration * 0.0343 / 2.0;
}


// ================================
// Forward with Obstacle Protection
// ================================
void forward() {

  // Stop if distance is unsafe or reading fails
  if (distanceCm <= STOP_DISTANCE) {
    stopMotors();
    return;
  }

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}


// ================================
// Backward
// ================================
void backward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}


// ================================
// Turn Left
// ================================
void left() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}


// ================================
// Turn Right
// ================================
void right() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}


// ================================
// Apply Movement at Current Speed
// ================================
void applyMovement() {

  switch (currentCommand) {

    case 'F':
      forward();
      break;

    case 'B':
      backward();
      break;

    case 'L':
      left();
      break;

    case 'R':
      right();
      break;

    default:
      stopCar();
      break;
  }
}


// ================================
// Setup
// ================================
void setup() {

  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrasonic sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(TRIG, LOW);

  // Buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // RGB LED 
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);

  // Front and back LEDs
  pinMode(FRONT_LED, OUTPUT);
  pinMode(BACK_LED1, OUTPUT);
  pinMode(BACK_LED2, OUTPUT);

  // Turn off all LEDs initially
  rgbOff();
  frontLedOff();
  backLedOff();

  // Safe initial state
  stopCar();

  // Start Bluetooth
  mybluetooth.begin("RC_CAR");

  Serial.println("Bluetooth RC Car Ready!");
  Serial.println("Bluetooth Name: RC_CAR");
  Serial.println("Speed Control: 0-9");
  Serial.println("Obstacle Protection: 10 cm");
  Serial.println("Buzzer: Enabled");

  Serial.println("X = RGB LEDs ON");
  Serial.println("x = RGB LEDs OFF");

  Serial.println("W = Front LED ON");
  Serial.println("w = Front LED OFF");

  Serial.println("U = Back LEDs ON");
  Serial.println("u = Back LEDs OFF");
}


// ================================
// Main Loop
// ================================
void loop() {

  // --------------------------------
  // Update Ultrasonic Sensor
  // --------------------------------
  if (millis() - lastSensorTime >= sensorInterval) {

    lastSensorTime = millis();

    distanceCm = getDistance();

    Serial.print("Distance: ");

    if (distanceCm > 0) {
      Serial.print(distanceCm);
      Serial.println(" cm");
    } else {
      Serial.println("No Echo");
    }

    // Buzzer ON when obstacle is within 10 cm
    if (distanceCm > 0 &&
        distanceCm <= STOP_DISTANCE) {

      digitalWrite(BUZZER, HIGH);

    } else {

      digitalWrite(BUZZER, LOW);
    }

    // Obstacle protection during forward movement
    if (currentCommand == 'F') {

      if (distanceCm == 0 ||
          distanceCm <= STOP_DISTANCE) {

        stopMotors();

      } else {

        forward();
      }
    }
  }


  // --------------------------------
  // Read Bluetooth Commands
  // --------------------------------
  if (mybluetooth.available()) {

    char command = mybluetooth.read();

    // Ignore line endings
    if (command == '\n' || command == '\r') {
      return;
    }

    Serial.print("Command: ");
    Serial.println(command);

    lastCommandTime = millis();

    switch (command) {

      // ----------------------------
      // Movement Commands
      // ----------------------------
      case 'F':
        currentCommand = 'F';
        forward();
        break;

      case 'B':
        currentCommand = 'B';
        backward();
        break;

      case 'L':
        currentCommand = 'L';
        left();
        break;

      case 'R':
        currentCommand = 'R';
        right();
        break;

      case 'S':
        stopCar();
        break;


      // ----------------------------
      // RGB LED Commands
      // ----------------------------
      case 'X':
        rgbOn();
        Serial.println("RGB LEDs ON");
        break;

      case 'x':
        rgbOff();
        Serial.println("RGB LEDs OFF");
        break;


      // ----------------------------
      // Front LED Commands
      // ----------------------------
      case 'W':
        frontLedOn();
        Serial.println("Front LED ON");
        break;

      case 'w':
        frontLedOff();
        Serial.println("Front LED OFF");
        break;


      // ----------------------------
      // Back LED Commands
      // ----------------------------
      case 'U':
        backLedOn();
        Serial.println("Back LEDs ON");
        break;

      case 'u':
        backLedOff();
        Serial.println("Back LEDs OFF");
        break;


      // ----------------------------
      // Speed Control: 0-9
      // ----------------------------
      case '0':
        motorSpeed = 40;
        applyMovement();
        break;

      case '1':
        motorSpeed = 80;
        applyMovement();
        break;

      case '2':
        motorSpeed = 100;
        applyMovement();
        break;

      case '3':
        motorSpeed = 120;
        applyMovement();
        break;

      case '4':
        motorSpeed = 140;
        applyMovement();
        break;

      case '5':
        motorSpeed = 160;
        applyMovement();
        break;

      case '6':
        motorSpeed = 180;
        applyMovement();
        break;

      case '7':
        motorSpeed = 200;
        applyMovement();
        break;

      case '8':
        motorSpeed = 230;
        applyMovement();
        break;

      case '9':
        motorSpeed = 255;
        applyMovement();
        break;
    }
  }


  // --------------------------------
  // Bluetooth Safety Timeout
  // --------------------------------
  if (millis() - lastCommandTime > timeout) {
    stopCar();
  }
}