/* Project name:- Automatic Water Dispensing System.
--------------------------------------------------------------------------------------------------------------------
Project Summary:-

This program uses an HC-SR04 ultrasonic sensor to detect a hand or cup.
When an object comes within 5.9 cm,the water pump turns ON,the red LED turns ON,
and the buzzer beeps once. When the object moves away, the pump turns OFF,the green LED lights up,
and the system waits for the next detection. The LCD continuously displays the measured distance and system status.
---------------------------------------------------------------------------------------------------------------------
Equepments: Microcontroler(ESP32), Ultrasonic Sensore, 2 led Red or Green, Buzzer, pump motor
*/

#include<Wire.h>                         // Library for I2C communication
#include<LiquidCrystal_I2C.h>            // Library for I2C LCD display

// Create LCD object with I2C address 0x27 and size 16 columns x 2 rows
LiquidCrystal_I2C lcd(0x27, 16,2);

// Pin definitions
const int led = 14;              // red led (water flowing indicator)
const int led2 = 26;             // green led (water off indicator)
const int Echo_pin = 18;         // Echo pin of ultrasonic sensor
const int trig_pin = 5;          // Trigger pin of ultrasonic sensor
const int pump = 2;              // Water pump control pin
const int buzzer = 13;           // Buzzer pin

int buzzerPlayed = 0;           // Variable to ensure buzzer sounds only once when object is detected

void setup(){
  Wire.begin(21, 22);          // Initialize I2C communication on ESP32 (SDA=21, SCL=22)

  lcd.init();                  // Initialize LCD
  lcd.backlight();             // Turn on lcd backlight

  lcd.setCursor(3,0);           // display startup message
  lcd.print("Auto Water");

  lcd.setCursor(4,1);
  lcd.print("Dispenser");

  delay(5000);                   // Show welcome screen for 5 seconds
  lcd.clear();                   // Clear LCD

  // Configure ultrasonic sensor pins
  pinMode(trig_pin, OUTPUT);
  pinMode(Echo_pin, INPUT);
  // Configure output devices
  pinMode(pump, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop(){

  // Send trigger pulse to ultrasonic sensor
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(Echo_pin, HIGH);                // Measure time taken for echo to return

  float distance = duration*0.034/2;                     // Calculate distance in centimeters

  // Display measured distance on LCD
  lcd.setCursor(0,0);
  lcd.print("Distance:");
  lcd.print(distance);
  lcd.print("cm");

  if (distance <= 5.9) {                               // If hand/cup is detected within 5.9 cm

    // Show water flowing status
    lcd.setCursor(0,1);
    lcd.print("** Flowing... **");
    // Turn ON pump and green LED
    digitalWrite(pump, HIGH);
    digitalWrite(led, HIGH);
    digitalWrite(led2, LOW);

    if (buzzerPlayed == 0) {                          // Play buzzer only once when object first enters range
      digitalWrite(buzzer, HIGH);                     // Turn buzzer ON
      delay(500);                                     // Beep for 0.5 second
      digitalWrite(buzzer, LOW);                      // Turn buzzer OFF

      buzzerPlayed = 1;                               // Mark buzzer as already played
    }
  }
  else {
    // Show pump OFF status
    lcd.setCursor(0,1);
    lcd.print("*** Flow OFF ***");
    // Turn OFF pump and green LED
    digitalWrite(pump, LOW);
    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);       // turn on green led

    buzzerPlayed = 0;               // Reset buzzer flag for next detection
  }
  delay(200);                   // Small delay before next measurement
}