// motion detecting with pir sensore 

int pir = 12;     //pir sensore pin number
int led = 21;

void setup() {
  Serial.begin(9600);

  pinMode(pir, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  int state = digitalRead(pir);

  if (state == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("Motion Detected!");
  }
  else{
    digitalWrite(led, LOW);
    Serial.println("No motion");
  }
  delay(1000);
}