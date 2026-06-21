/* esp32

project name:- 'lode controle with bluetooth'
blutooth softwer name 'bluthooth terminal'

. click a = led 2 on
. click b = led 2 off

. click c = led 1 on
. click d = led 2 off

*/

#include "BluetoothSerial.h"      // Bluetooth header file

BluetoothSerial myblutoot;        // maked bluetooth comminication variable. as like BluetoothSerial this keyword is remote system 
                                  // and myblutoot is self created object
char receivedChar;                // this line is inshored input will charecter type.
int led = 12;                     // led port number
int led2 = 2;                     // led2 port number

void setup(){
  Serial.begin(9600);             // vartual serial monitor inshoreing

  myblutoot.begin("Rocky_esp32");    // bluetooth name declering
  pinMode(led, OUTPUT);              // lode position inshore
  pinMode(led2, OUTPUT);
}

void loop(){

  // bluetooth data recived Command

  if (myblutoot.available()) {               // input data checking                
    receivedChar = myblutoot.read();         // input data read and store in variable as like 'receivedchar'

    if (receivedChar == '\n' || receivedChar == '\r') {       
      return;                                                   // ignor '\n' and '\r' comand then print
    }

    Serial.print("recive: ");                   // recived cherecter print comand
    Serial.println(receivedChar);               // recived chrecter stored in variable
    
    if (receivedChar == 'a') {                  // when input a 
    digitalWrite(led2, HIGH);                   // led2 on
    Serial.println("LED = 2 on\n");             // serial monitor showing
  }

  else if (receivedChar == 'b') {              // when input b
    digitalWrite(led2, LOW);                   // led2 off
    Serial.println("led 2 off\n");             // serial monitor showing
  }

  else if (receivedChar == 'c') {             // when input c
    digitalWrite(led, HIGH);                  // led 1 on
    Serial.println("LED = 1 on\n");           // serial monitor showing
  }

  else if(receivedChar == 'd'){              // when input d
    digitalWrite(led, LOW);                  // led 1 off
    Serial.println("LED = 1 off\n");
  }
  else {
    Serial.println("not exit.\nprint a,b,c,d");        // if input a,b,c,d without Another cherecter, then showing not exit.
  }
  }

}
