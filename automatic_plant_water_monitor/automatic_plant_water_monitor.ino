/* Project Name -> automatic plant Water monitoring system.
---------------------------------------------------------------------
work flow ->
. sensore reading
. decisition making
. actuator control
---------------------------------------------------------------------
Equipments -> ESP32(Microcontroller), soil motion sensore, water pump, green led, red led, buzzer
*/
const int soil_pin = 33;        // soil sensore to esp32 pin number.
const int pump = 2;             // pump pin number
const int green_led = 14;       // green led pin number
const int red_led = 12;         // red led pin number
const int buzzer = 13;          // buzzer pin number

int buzzerPlayed = 0;           // starting time buzzer LOW condition dacler

void setup(){
  Serial.begin(115200);         // Serial monitor setup

  // pump led buzzer condition dacler
  pinMode(pump, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop(){
  int sensoreValu = analogRead(soil_pin);      // Read the value from the soil moisture sensor

  Serial.print("soil valu = ");                // valu prient
  Serial.println(sensoreValu);                 // main variable

  if (sensoreValu >= 2000) {                   // Check if the sensor value is greater than or equal to 2000
    digitalWrite(pump, HIGH);                  // turn on the pump
    Serial.println("pump ON");                 // pump on to serial monitor
    digitalWrite(red_led, HIGH);               // red led on
    digitalWrite(green_led, LOW);              // green led off

    if (buzzerPlayed == 0) {                   // Play the buzzer only once when the condition becomes true
      // Turn ON the buzzer for 1 second
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      // Mark that the buzzer has already played
      buzzerPlayed = 1;
    }

  }  
  else {
    digitalWrite(pump, LOW);                 // turn off the water pump
    Serial.println("pump OFF");
    digitalWrite(red_led, LOW);              // Turn OFF the red LED 
    digitalWrite(green_led, HIGH);           // and turn ON the green LED

    buzzerPlayed = 0;
  }

  delay(500);               // Wait for 500 milliseconds before the next loop
}