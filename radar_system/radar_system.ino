#include <ESP32Servo.h>      // ESP32 microcontroller servo motor hader file 

const int led = 15;          // led pin Declare.
const int led2 = 5;          // 2nd led pin Declare. 
const int buzzer = 27;       // buzzer to Set alarm pin Declare. 
const int trigPin = 14;      // ultrasonic triger pin declare.
const int echoPin = 12;      // ultrasonic echo pin declare.

// defining time and distance
long duration;
int distance;
Servo myServo; // Object servo

void setup() {
  pinMode(led, OUTPUT);             // led as an Output
  pinMode(led2, OUTPUT);            // 2nd pin Output declering
  pinMode(buzzer, OUTPUT);          // buzzer Output declering
  pinMode(trigPin, OUTPUT);         // trigPin as an Output
  pinMode(echoPin, INPUT);          // echoPin as an Input
  Serial.begin(9600);
  myServo.attach(2);                // Pin Connected To Servo
}
void loop() {

  for(int i=15;i<=165;i++){          // rotating servo i++ depicts increment of one degree, 15 to 165 degri angle decler
  myServo.write(i);                  // Servo angle set Command.
  delay(30);                         // 30 melesecond Wait

  distance = calculateDistance();    // logic stor to distance veriable. 

  if (distance < 22) {               // when subject or object close to 22cm then
    digitalWrite(led, HIGH);         // led is on condition
    digitalWrite(led2, LOW);         // second led2 off condition and
    digitalWrite(buzzer, HIGH);      // buzzer sound condition.

    while (distance < 22) {                  // when subject or object close to 22cm then
      distance = calculateDistance();        // Detecting suject and servo off this time, as long as subject or object don't move 
      delay(100);                                  // when move subject out of area while loop finished, and servo Rotateing full angle
    }

    digitalWrite(led, LOW);                     // if subject move to out of area then led off
    digitalWrite(buzzer, LOW);                   // buzzer off
  }

  else if (distance <= 80) {                   // else if subject 22-80cm in to the area
    digitalWrite(led, LOW);                    // 1st led off condition
    digitalWrite(buzzer, LOW);                 // buzzer off condition and
    digitalWrite(led2, HIGH);                  // 2nd led2 on condition
  }
   // else led, led2, buzzer off condition
  else {
    digitalWrite(led, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(buzzer, LOW);
  }

  // angle, distance print to serial monitor statment 
  Serial.print(i);
  Serial.print(","); 
  Serial.print(distance); 
  Serial.print(".");
  }
  // Repeats the previous lines from 165 to 15 degrees
  for(int i=165;i>15;i--){                              // return 165 to 15 degri loop             
  myServo.write(i);
  delay(30);
  distance = calculateDistance();

  if (distance < 22) {
    digitalWrite(led, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(buzzer, HIGH);

    while (distance < 22) {
      distance = calculateDistance();
      delay(100);
    }

    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
  }

  else if (distance <= 80) {
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(led2, HIGH);
  }

  else {
    digitalWrite(led, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(buzzer, LOW);
  }

  // Return condition pin to serial monitor
  Serial.print(i);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
  }
}
int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);                    // for sensore start 2 microsecond delay
  
  digitalWrite(trigPin, HIGH);             // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance= duration*0.034/2;                   // counting sentimitter, and Measure One Direction
  return distance;
}
