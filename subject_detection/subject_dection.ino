/*
project name :- subject dececation

. when subject 20 cm close led on
. when subject 20 cm out led off

Eqepment : esp32, ultrasonic sensore, led

*/
#define led 14                  // led pin conection
#define TRIG_PIN 13            // Aulrasonic sensor TRIG pin connect to arduino 9 pin
#define ECHO_PIN 12            // echo pin number 12

void setup(){
  Serial.begin(115200);          // vertual terminal baud
  Serial.println("sercing...");
  delay(1000);

  pinMode(led, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);    // Ultrasonic paint declar, as TRIG pin OUTPUT
  pinMode(ECHO_PIN, INPUT);
}

void loop(){
  long duration;                // Subject sensing time duration
  float distance;                // subject distance sensing

  digitalWrite(TRIG_PIN, LOW);    
  delayMicroseconds(1);           // 1 microsecond delay

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);          // tig pin HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN, LOW);    // then trig low 

  duration = pulseIn(ECHO_PIN, HIGH);  // how long pin high or low sens

  distance = duration * 0.0343 / 2;    // duration*air_velocity(340 M/S), sound go and comeing Frequency for creating one Diction 2

  if (distance < 20) {
    digitalWrite(led, HIGH);
    Serial.print("Led on \n");      // when suhbject 20 cm close led on
  }

  else {
    digitalWrite(led, LOW);
    Serial.print("Led off \n");     // when subject 20 cm away led off
  }

  Serial.print("Sub Gap:- ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}