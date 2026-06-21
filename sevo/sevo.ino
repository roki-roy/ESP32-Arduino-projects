#include<Servo.h>     // Servo hader file

Servo myservo;        // Servo object

int pos = 0;

void setup(){
  myservo.attach(8);         // microcontrolar pin dicler
}

void loop(){
  for (pos = 0; pos<=180; pos+=5) {           // loop and (pos+=5) power dicler
    myservo.write(pos);                       // Specific angle for rotted (write)
    delay(20);
  }

  for (pos = 180; pos>=0; pos-=5) {           // Servo Reverse rotating loop
    myservo.write(pos);
    delay(15);
  }
}