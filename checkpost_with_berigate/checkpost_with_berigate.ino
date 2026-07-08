/* Project name : checkpost with berigate automatic system

if subject is front to chakpost then subject detect and servo 0 degri berigate down, led will on, buzzer high 1s
if subjevt gone to checkpost then berigate 90 degri up and led will off, buzzer off.

elements = esp32, led, buzzer, servo, ultrasonic sensore

*/

#include<ESP32Servo.h>       // esp 32 sevro hader file

const int led = 27;          // led pin number
const int buzzer = 14;        // buzzer pin number

const int echo_pin = 12;        // ultrasonice echo pin number
const int trig_pin = 13;         // ultrasonice trig pin number

Servo myservo;                   // servo objecgt

int pos = 0;                      // position variable
int buzzerPlayed = 0;              // buzzer high and low condition read 

void setup(){
  myservo.attach(2);
  myservo.write(0);

  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop(){
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);

  float distance = duration*0.034/2;

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 10) {                          // when subject distance 10 cm close then servo 90 degri open
    pos = 90;
    myservo.write(pos);                          // specifice angle roteting condition

    digitalWrite(led, HIGH);

    if (buzzerPlayed == 0) {
      digitalWrite(buzzer, HIGH);                  // buzzer high condition and low condition
      delay(1000);
      digitalWrite(buzzer, LOW);

      buzzerPlayed = 1;
    }
  }

  else {
    pos = 0;                                       // servo return 0 degri
    myservo.write(pos);

    digitalWrite(led, LOW);
    buzzerPlayed = 0;

  }
  delay(100);
}