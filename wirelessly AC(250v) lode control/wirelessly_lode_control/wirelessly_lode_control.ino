/* Project name: Bluetooth to Wirelessly (250AC) load control.

Equipments -> ESP32, 4 channel relay, up to 4 lodes and bluetooth temminal Software.

User Control system ->

.. a = lode1 on
.. b = lode1 off

.. c = lode2 on
.. d = lode2 off

.. e = lode3 on 
.. f = lode3 off

.. g = lode4 on
.. h = lode4 off

*/

#include"BluetoothSerial.h"            // Bluetooth hader file

BluetoothSerial mybluetooth;            // Bluetooth Remote system

char recivedChar;                       // Data type Declar, this data type is charecter type

// Microcontroller to relay pin Declar

const int Relay1 = 13;
const int Relay2 = 2;
const int Relay3 = 21;
const int Relay4 = 19;

void setup(){
  Serial.begin(9600);                    // vertual terminal number declar

  mybluetooth.begin("roy_esp32");          // Bluetooth name Ensure

  // pin modes type declar
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(19, OUTPUT);

  // Relay first Mode set

  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);

  Serial.println("Send charecter a,b,c,d,e,f,g,h");            // user operate charecter ensure line
}


void loop(){

  // Bluetooth data recive Command
  if (mybluetooth.available()) {
    recivedChar = mybluetooth.read();             // recive data checking

    if (recivedChar == '\n' || recivedChar == '\r') {             // ignor \n and \r
      return;
    }

    Serial.print("Recive = ");                   // recive charecter printing line
    Serial.println(recivedChar);                 // recive charecter store variable 

    // Controlling condition

    if (recivedChar == 'a') {
      digitalWrite(Relay1, LOW);
      Serial.println("lode1 on\n");
    }
    else if (recivedChar == 'b') {
      digitalWrite(Relay1, HIGH);
      Serial.println("lode 1 off\n");
    }
    else if (recivedChar == 'c') {
      digitalWrite(Relay2, LOW);
      Serial.println("lode 2 on\n");
    }
    else if (recivedChar == 'd') {
      digitalWrite(Relay2, HIGH);
      Serial.println("lode 2 off\n");
    }
    else if (recivedChar == 'e') {
      digitalWrite(Relay3, LOW);
      Serial.println("lode 3 on\n");
    }
    else if (recivedChar == 'f') {
      digitalWrite(Relay3, HIGH);
      Serial.println("lode 3 off\n");
    }
    else if (recivedChar == 'g') {
      digitalWrite(Relay4, LOW);
      Serial.println("lode 4 on\n");
    }
    else if (recivedChar == 'h') {
      digitalWrite(Relay4, HIGH);
      Serial.println("lode 4 off\n");
    }
    else {
      Serial.println("not match!\nplz enter a,b,c,d,e,f,g,h");
    }

  }
}