
// Keypad-smallcal (4x4)

#include<Keypad.h>     // keypad hader file

const byte ROWS = 4;   // 4 row keypad 
const byte COLS = 4;   // 4 colam keypad 

char keys[ROWS][COLS]{   // keypad all baton Initialize
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'c','0','=','+'},
};

byte rowPins[4] = {2,3,4,5};   // arduino to keypad row Connection pins
byte colPins[4] = {6,7,8,9};   // colam pines

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  if (key) {
  Serial.println(key);
  }
}

