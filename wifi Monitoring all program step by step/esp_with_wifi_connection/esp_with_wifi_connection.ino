/* work -> esp32 with own wifi connection program.

..working syntex -> local IP address show and server connection.
*/

#include <WiFi.h>                         // wifi hader file

const char* ssid = "OPPO A60";             // own wifi network name (ssid)
const char* password = "12345678";         // wifi password

void setup(){
  Serial.begin(9600);                      // serial monitor opening

  WiFi.begin(ssid, password);                // wifi network connecting start
  
  // wifi connection wait until it happens

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");           // as long as it does not get connected, print connecting...
  }

  Serial.println("");
  Serial.println("Wifi Connected");            // successfully connected to server
  Serial.print("IP Address = ");
  Serial.println(WiFi.localIP());              // esp32 IP Address showing
}

void loop(){
       // here tasks of the main program
}