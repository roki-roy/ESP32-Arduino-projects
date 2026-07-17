// project name:- wifi to door control

#include <WiFi.h>
#include <ESP32Servo.h>        // servo hader file

const int led = 13;

const char* ssid = "OPPO A60";
const char* password = "12345678";

WiFiServer server(80);

Servo myservo;

int pos = 0;

void setup(){
  Serial.begin(9600);

  myservo.attach(2);
  pinMode(led, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Sercing...");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  server.begin();

}
void loop(){
  WiFiClient client = server.available();

  if (client) {

    String request = client.readStringUntil('\r');   //this line declering client is usually a WiFiClient or EthernetClient object.readStringUntil('\r') reads characters from the client until it encounters a carriage return character ('\r').
    Serial.println(request);
    
    // servo position set 

    if (request.indexOf("/dooron") != -1) {
      for (pos = 0; pos<=90; pos+=3) {
        myservo.write(pos);
        digitalWrite(led, HIGH);
        Serial.println("Door Open");
      }
    }
    if (request.indexOf("/dooroff") != -1) {
      for (pos = 90; pos >= 0; pos-=4) {
        myservo.write(pos);
        digitalWrite(led, LOW);
        Serial.println("Door close");
      }
    }

    Serial.println("Member Comeing");

    client.println("HTTP/1.1 200 OK");
    client.println("Conten-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html>");
    client.println("<body>");

    client.println("<center><h1 style ='font-size:80px; margin-top:100px;'>DOOR CONTROLAR</h1></center>");

    client.println("<center><h2 style ='font-size:30px; margin-top:60px;'>Smart access control using WiFi techonology</h2></center>");

    client.println("<div style ='display:flex; justify-content: center; gap:100px; margin-top: 50px'>");

    client.println("<a href ='/dooron'><button><h1 style='font-size:40px;'>OPEN</h1></a>");
    client.println("<a href ='/dooroff'><button><h1 style='font-size:40px;'>CLOSE</h1></a>");

    client.println("</body>");
    client.println("</html>");

    client.flush();
    delay(200);
    client.stop();
  }

}


