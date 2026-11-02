/*
witout delay(), use millis() to LED Blinking code
 
*/
const int LED = 2;

unsigned long previusTime = 0;

const unsigned long interval = 4000;

void setup(){
  pinMode(LED, OUTPUT);
}

void loop(){
  unsigned long currentTime = millis();

  if (currentTime - previusTime >= 4000) {
    previusTime = currentTime;

    digitalWrite(LED, !digitalRead(LED));     // led LOW and HIGH
  }
}