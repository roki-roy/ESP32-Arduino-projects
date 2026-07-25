/* project Name:- Basic temperature and humidity mnitor
-----------------------------------------------------------------------------------------------
Real-time temperature and humidity monitoring System.
using ESP32, DHT11, and a 16x2 I2C LCD display.
-----------------------------------------------------------------------------------------------
*/

#include <Wire.h>                                    // I2C communication library
#include <LiquidCrystal_I2C.h>                       // I2C LCD library
#include <Adafruit_Sensor.h>                         // Adafruit sensor support library
#include <DHT.h>                                     // DHT sensor library

const int dhtPin = 4;                                 // DHT11 data pin connected to GPIO 4
#define DHTTYPE DHT11                                 // Define sensor type as DHT11

DHT dht(dhtPin, DHTTYPE);                            // Create DHT sensor object

LiquidCrystal_I2C lcd(0x27, 16,2);                  // Create 16x2 LCD object with I2C address 0x27

void setup(){
  Wire.begin(21,22);                               // Initialize I2C (SDA = GPIO21, SCL = GPIO22)

  lcd.init();
  lcd.backlight();

  dht.begin();                                  // Initialize DHT11 sensor

}

void loop(){
  float humidity = dht.readHumidity();                             // Read humidity value

  float temperature = dht.readTemperature();                       // Read temperature value in Celsius

  if (isnan(humidity) || isnan(temperature)) {                     // Check if sensor reading failed
    lcd.setCursor(0,0);                                            // Move cursor to first row
    lcd.print("Failed to read");                                    // display error massage
    lcd.setCursor(0,1);
    lcd.print("****************");
    delay(2000);                                                   // Wait 2 seconds
    return;                                                       // Exit current loop iteration
  }
  lcd.setCursor(0,0);                                       // Move cursor to first row
  lcd.print("Temp:");                                      // Display temperature label
  lcd.print(temperature);                                // Display temperature value
  lcd.print(" C  ");                                    // Display Celsius unit


  lcd.setCursor(0,1);                                   // Move cursor to second row
  lcd.print("Humd:");                                   // Display humidity label
  lcd.print(humidity);                                 // Display humidity value
  lcd.print(" %    ");                                     // Display percentage symbol

  delay(1000);                                             // Update every 1 second
}