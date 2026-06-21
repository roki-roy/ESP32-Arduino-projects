// LDR Monitoring program

int ldr = A4;    // arduino pin dicler
int valu = 0;

void setup() {
  Serial.begin(9600);     // vartual terminetor valu showing 

}

void loop() {
  valu = analogRead(ldr);
  Serial.println(valu);      // ldr valu showing
  delay(1000);               // 1s afer print
  if (valu<300) {            // when ldr valu 300 Below
    Serial.println("light is off"); // show light is off
    delay(1000);                    // 1s after print
  }
  else if (valu>300 && valu<800) {     // when valu 300 uper and 800 below 
    Serial.println("light is on");     // show light is on
    delay(1000);
  }
  else if (valu>800) {                // when ldr valu 800 uper
    Serial.println("light is max lavel");  // show light is max lavel
    delay(500);                            // 0.5s after show
  }
}
