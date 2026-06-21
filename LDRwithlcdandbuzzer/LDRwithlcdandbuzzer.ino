#include<LiquidCrystal.h>

int ldr = A2;                        // ldr cunect to arduino analog pin
int buzzer = 2;                      // buzzer cunect to arduino
int RS=13,E=12,D4=11,D5=10,D6=9,D7=8;

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);

int valu = 0;

void setup() {
  pinMode(buzzer, OUTPUT);
  lcd.begin(16,2);                  // lcd type(16x2)
  lcd.print("Sersing...");          // home scring print
  delay(1500);                      // 1.5s stay
  lcd.clear();                      // display empety

}

void loop() {
  valu = analogRead(ldr);
  if (valu<300) {                  // when valu 300 below
    lcd.setCursor(0,0);            // lcd row(0) colum(2) start
    lcd.print("Light is off"); 
    tone(buzzer,500);              // buzzer frequncy(500)
  }

  else if (valu>300 && valu<845) { // when valu 300 high and 845 below (condition)
    lcd.setCursor(0,0);            
    lcd.print("Light is on"); 
    noTone(buzzer);
    delay(1000);
  }

  else if (valu>845) {             // when valu 845 HIGH (condition)
    lcd.setCursor(0,0);
    lcd.print("Light Max Lavel");
    tone(buzzer, 500);
    delay(1000);
  }

}
