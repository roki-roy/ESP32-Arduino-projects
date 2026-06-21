// buzzer Connection progrem in arduino

int buzzer = 8;    // arduino pin 

void setup() {

}

void loop() {
  tone(buzzer,400);    // pin and frequency seting
  delay(1000);         // 1s after Beep

  noTone(buzzer);      // tone off
  delay(1000);         // 1s 
}
