// useing 16x2 lcd display

#include<LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8); // Arduino pins RS-13 number pin, E-12 number pin
                                    // D4-11 num pin, D5-10 num pin, D6-9 num pin, D7-8 num pin

void setup() {
  lcd.begin(16,2); // Display type
  lcd.setCursor(3,1);           // function - setCursor(cullam,row)
  lcd.print("Roki Roy");        // display show Roki Roy
  delay(5000);                  // 5s delay
  lcd.clear();                  // display clear
}

void loop() {
  lcd.setCursor(5,0);
  lcd.print("HELLO");
}
