#include<LiquidCrystal.h>      // lcd(16x2) display hader file
#include<Keypad.h>             // (4x4) keypad hader file
#define len 5

LiquidCrystal lcd(8,9,10,11,12,13);    // lcd to arduino pin comminication

const byte ROWS = 4;      // keypad row declar
const byte COLS = 4;      // keypad collam pin decler

char keys[ROWS][COLS]{     // keypad row and collam pin decler
  {'7','8','9','A'},
  {'4','5','6','B'},
  {'1','2','3','C'},
  {'c','0','=','D'},
};

byte rowPins[4] = {0,1,2,3};       // keypad to arduino row pin decler
byte colpins[4] = {4,5,6,7};       // keypad to arduino collam pin decler
char user[len];                    
char Password[len] = "1234";      // write password decler
byte count = 0;
char key;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colpins, ROWS, COLS);

void setup(){
  lcd.begin(16,2);
  lcd.print("sercing...");
  delay(1500);
  lcd.clear();
}

void loop()
{
  lcd.setCursor(0,0);                   // lcd row and collam seting
  lcd.print("Enter Password");
  key = keypad.getKey();                // user enter read
  if (key) {
    user[count] = key;
    lcd.setCursor(count,1);             // lcd 2nd line decler
    lcd.print(user[count]);
    count++;                            // line by line count
  }
  if (count==len-1) {
    if (!strcmp(user,Password)) {       // password checking
      lcd.clear();
      lcd.print("Password Maching");
      delay(2000);
      count=0;
    } else {
      lcd.clear();
      lcd.print("Wrong Password");
      delay(1500);
      count=0;
    }
  }
}