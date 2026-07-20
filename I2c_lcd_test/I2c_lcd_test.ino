// I²C module to 16x2 lcd display check code
 
#include <Wire.h>                                 // Library for I²C communication
#include <LiquidCrystal_I2C.h>                    // Library for I²C LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);              // LCD I²C address = 0x27, 16 columns and 2 rows

void setup() {
  Wire.begin(21, 22);   // Initialize pin I2c on SDA = 21, SCL = 22

  lcd.init();           // Initialize the LCD
  lcd.backlight();      // Turn on the LCD backlight

}

void loop() {
  // Display "Checking..." on the second row
  // Column 3, Row 1 (rows start from 0)

  lcd.setCursor(3,1);
  lcd.print("Checking...");

  lcd.setCursor(0, 0);         // Move cursor to the beginning of the first row
  lcd.print("Hello World!");    // Display the first message
  delay(3000);               // Wait for 3 seconds
  
  // Display the second message
  // The extra space at the end overwrites the previous '!'
  lcd.setCursor(0, 0);
  lcd.print("ESP32 + LCD ");
  delay(3000);       // Wait for 3 seconds
}