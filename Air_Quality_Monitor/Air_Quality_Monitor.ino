/* Project Name: Air Quality Monitoring System
------------------------------------------------------------------------------------------------------------
summary:-
This Arduino/ESP32 program is an Air Quality Monitoring System that uses an MQ135 gas sensor to
measure air quality levels and display the readings on a 16×2 I2C LCD. The system continuously monitors the
surrounding air and classifies the condition into three categories: Safe,Risky!, and Danger.
-------------------------------------------------------------------------------------------------------------
Equipments:- ESP32, MQ-135, LCD(16x2) with I2C module, red led, green led, buzzer
*/

#include <Wire.h>                           // Library for I2C communication
#include <LiquidCrystal_I2C.h>              // Library for I2C LCD display

LiquidCrystal_I2C lcd(0x27,16,2);           // Create LCD object (Address: 0x27, 16 columns, 2 rows)

const int MQ135_PIN = 34;                   // MQ135 sensor connected to ESP32 analog pin 34
const int red_Led = 13;                     // Red LED connected to pin 13
const int green_led = 27;                   // Green LED connected to pin 27
const int buzzer = 14;                      // Buxzzer connectin to pin 14

void setup(){
  Wire.begin(21,22);                        // Initialize I2C Communication (SAD=21, SCL=22)

  lcd.init();                              // Initalize LCD
  lcd.backlight();                          // turn no led backlight

  pinMode(red_Led, OUTPUT);                 // Set red LED pin as output
  pinMode(green_led, OUTPUT);               // Set green LED pin as output
  pinMode(buzzer, OUTPUT);                  // Set buzzer pin as output

  lcd.setCursor(0,0);                       // Move cursor to first row, first column
  lcd.print("Searching...");                // Display startup message
  delay(3000);                              // Wait for 3 seconds
  lcd.clear();                              // Clear LCD screen
} 

void loop(){
  int airQuality = analogRead(MQ135_PIN);            // Read analog value from MQ135 sensor

  lcd.setCursor(0,0);                                // Move cursor to first row
  lcd.print("Air Q: ");                              // Display label
  lcd.print(airQuality);                             // Display sensor reading
  lcd.print("       ");                              // Clear leftover digits from previous reading

  // SAFE CONDITION
  if (airQuality <= 500 ) {
    lcd.setCursor(0,1);                   // Move cursor to second row
    lcd.print("***** SAFE *****");        // Display SAFE status

    digitalWrite(green_led, HIGH);        // green led on 
    digitalWrite(red_Led, LOW);           // red led off
    digitalWrite(buzzer, LOW);            // buzzer off
  }
  // Risky condition
  else if (airQuality <= 700 ) {
    lcd.setCursor(0, 1);                  // Move cursor to second row
    lcd.print("**** RISKY! ****");        // Display Risky status

    digitalWrite(green_led, LOW);         // green led off
    digitalWrite(red_Led, HIGH);          // red led ON
    digitalWrite(buzzer, LOW);            // buzzer off
  }

  // Danger Condition
  else {
    lcd.setCursor(0, 1);                     // Move cursor to second row
    lcd.print("**** DANGER ****");           // display Danger Status
    
    digitalWrite(green_led, LOW);            // green led off
    
    // Creates a flashing visual alert and audible alarm.
    digitalWrite(red_Led, HIGH);           
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(red_Led, LOW);
    digitalWrite(buzzer, LOW);
    delay(200);
  }

  delay(500);                         // Wait 0.5 second before next reading
}