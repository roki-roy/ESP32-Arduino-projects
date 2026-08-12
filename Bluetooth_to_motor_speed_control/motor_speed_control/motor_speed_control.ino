/* Project name: Bluetooth bassed motor speed control sysem.
------------------------------------------------------------------------------------------------------------
Summary:-
This project is a Bluetooth-Based Motor Speed Control System using an ESP32. The ESP32 receives
Bluetooth commands from a smartphone and controls the speed of a DC motor using PWM. Four different
speed levels are available: 25%, 50%, 75%, and 100%, along with a motor OFF command. Four LEDs indicate
the selected speed level, while a 16×2 I2C LCD displays the received command and current motor speed. The
system provides a simple wireless method for controlling motor speed remotely through Bluetooth.

Receive character and Bluetooth command:

f = OFF
l = LOW  25% motor speed
m = MEDIAM 50% motor speed
h = HIGH 75% motor speed
s = SUPPER 100% motor speed

--------------------------------------------------------------------------------------------------------------
Equipment:- ESP32, led(red,green,white and yellow), DC Motor with(Transistor/MOSFET/Motor driver)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

LiquidCrystal_I2C lcd(0x27,16,2);           // lcd type

BluetoothSerial mybluetooth;

char recivedChar;

// esp32 to motor pin Initialized
const int motorPin = 25;

// Motor Speed Initialized
const int LOW_SPEED = 64;           //25%
const int MEDIAM_SPEED = 128;       // 50%
const int HIGH_SPEED = 191;         //75%
const int SUPER_SPEED = 255;        // 100%

// LED pin Initialized
const int white = 13;
const int yellow = 14;
const int green = 4;
const int red = 2;

void setup(){
  Wire.begin(21,22);
  lcd.init();
  lcd.backlight();

  mybluetooth.begin("esp32_motor");      // Bluetooth name

  pinMode(motorPin, OUTPUT);

  analogWrite(motorPin, 0);

  lcd.setCursor(0,0);
  lcd.print("Connect network ");
  delay(4000);
  lcd.clear();

  pinMode(white, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}

void loop(){
  if (mybluetooth.available()) {
    recivedChar = mybluetooth.read();

    if (recivedChar == '\n' || recivedChar == '\r') {
      return;
    }
    lcd.setCursor(0,0);
    lcd.print("recive:");
    lcd.print(recivedChar);
    lcd.print("          ");

    if (recivedChar == 'l') {
      analogWrite(motorPin, LOW_SPEED);
      digitalWrite(white,HIGH);

      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);

      lcd.setCursor(0,1);
      lcd.print("Speed 25%    ");
    }

    else if (recivedChar == 'm') {
      analogWrite(motorPin, MEDIAM_SPEED);
      digitalWrite(yellow,HIGH);

      digitalWrite(white, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      lcd.setCursor(0,1);
      lcd.print("Speed 50%    ");
    }

    else if (recivedChar == 'h') {
      analogWrite(motorPin, HIGH_SPEED);
      digitalWrite(green,HIGH);

      digitalWrite(yellow, LOW);
      digitalWrite(white, LOW);
      digitalWrite(red, LOW);

      lcd.setCursor(0,1);
      lcd.print("Speed 75%    ");
    }
    else if (recivedChar == 's') {
      analogWrite(motorPin, SUPER_SPEED);
      digitalWrite(red, HIGH);

      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(white, LOW);

      lcd.setCursor(0,1);
      lcd.print("Speed 100%   ");
    }

    else if (recivedChar == 'f') {
      analogWrite(motorPin, 0);
      digitalWrite(green,LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(white, LOW);
      digitalWrite(red, LOW);

      lcd.setCursor(0,1);
      lcd.print("Motor OFF           ");
    }

    else {
      lcd.setCursor(0,1);
      lcd.print("Unknown valu!");
    }
  }
}