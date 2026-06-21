/*
project name - Fier detecation

... when without fier print 'No fier'
... when fiering prient 'fier fier'
*/

const int fiersensor = 12;      // fiersensore pin number
const int led = 27;             // led pin number
const int pump = 2;             // pump pin number

void setup() {
  Serial.begin(9600);           // serial monitor in print
  Serial.print("fier sencing...");  // Connection animation
  delay(2000);                      // 2s stay

  pinMode(fiersensor, INPUT);       // sensore declaration
  pinMode(led, OUTPUT);             // led declaration
  pinMode(pump, OUTPUT);            // pump declaration
}

void loop() {
  int firevalu = digitalRead(fiersensor);     // rier sencing read

  if (firevalu == HIGH) {               // when without fier, led off loop. fier sensore always high without fier, and when fireing sensore low.
    digitalWrite(led, LOW);             // without fier led low
    Serial.print("No fier\n");          // display print No fier
    digitalWrite(pump, LOW);            // pump off(LOW) declaration
  }
  else {
    digitalWrite(led, HIGH);            // fiering time led 'ON'
    Serial.print("fier fier...\n");        // this time display print 'fier fier...'
    digitalWrite(pump, HIGH);              // pump on(HIGH) declaring
    Serial.print("pump motor on\n");
  }
  delay(500);                           // serial monitor display delay


}
