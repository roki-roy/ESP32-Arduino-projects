/*  
.......................................................
work flow ->
.. basic Control Panel creating code with esp32
.. phone or computer to web page opening
.. Heading(Control Panel) and Button creat
.. Handling button, and signal input

.......................................................
*/

#include <WiFi.h>                                    // wifi hader file

const char* ssid = "OPPO A60";                   // wifi name
const char* password = "12345678";             // wifi password

WiFiServer server(80);                               // server token

void setup(){

  // wifi connection Setup
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }
  Serial.println("");
  Serial.println("Wifi Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  server.begin();
}


void loop(){
  WiFiClient client = server.available();

  if (client) {

  String request = client.readStringUntil('\r');   //this line declering client is usually a WiFiClient or EthernetClient object.readStringUntil('\r') reads characters from the client until it encounters a carriage return character ('\r').
  Serial.println(request);

  // signal input condition

  if (request.indexOf("/load1on") != -1) {
    Serial.println("Load 1 ON");
  }
  if (request.indexOf("/load1off") != -1) {
    Serial.println("Load 1 OFF");
  }
  if(request.indexOf("/load2on") != -1){
    Serial.println("Load 2 ON");
  }

  if(request.indexOf("/load2off") != -1){
    Serial.println("Load 2 OFF");
  }
  if(request.indexOf("/load3on") != -1){
    Serial.println("Load 3 ON");
  }
  if(request.indexOf("/load3off") != -1){
    Serial.println("Load 3 OFF");
  }
  if(request.indexOf("/load4on") != -1){
    Serial.println("Load 4 ON");
  }
  if(request.indexOf("/load4off") != -1){
    Serial.println("Load 4 OFF");
  }

  Serial.println("New Client");                    // Print a message to the Serial Monitor when a new client connects

  client.println("HTTP/1.1 200 OK");                 // Send HTTP response status code (200 = success)
  client.println("Content-Type: text/html");           // Tell the browser that the content is HTML
  client.println("Connection: close");              // Instruct the browser to close the connection after receiving the response
  client.println();                              // Send a blank line to indicate the end of the HTTP headers

  client.println("<html>");                      // Start the HTML document
  client.println("<body>");                       // Start the body section of the HTML page   

  client.println("<center><h1 style= 'font-size:80px; margin-top:100px;'>Control Panel</h1></center>");
  // Display a large centered heading "Control Panel" on the webpage

  client.println("<center><h2 style = 'font-size: 30px; margin-top: 70px;'>Home Automation basic system</h2></center>");  // <h2> small Heading Headline

  client.println("<div style='display:flex; justify-content: center; gap:100px; margin-top: 50px'>");
  // Create a container that uses Flexbox to place elements side-by-side,
  // centered horizontally, with 100px space between them

  client.println("<a href = '/load1on'><button><h1 style = 'font-size:40px;'>Load(1) ON</h1></button></a>");     //a is anchor(link) href is location,Create a button labeled "Load(1) ON" with large text
  client.println("<a href = '/load1off'><button><h1 style = 'font-size:40px;'>Load(1) OFF</h1></button></a>");     // Create a button labeled "Load(1) OFF" with large text

  client.println("</div>");          // End the Flexbox container

  client.println("<div style = 'display:flex; justify-content: center; gap:100px; margin-top: 50px'>");
  client.println("<a href = '/load2on'><button><h1 style = 'font-size:40px;'>Load(2) ON</h1></button></a>");  
  client.println("<a href = '/load2off'><button><h1 style = 'font-size:40px;'>Load(2) OFF</h1></button></a>");  
  // for second button

  client.println("</div>");

  client.println("<div style = 'display:flex; justify-content: center; gap:100px; margin-top: 50px'>");
  client.println("<a href = '/load3on'><button><h1 style = 'font-size:40px;'>Load(3) ON</h1></button></a>");  
  client.println("<a href = '/load3off'><button><h1 style = 'font-size:40px;'>Load(3) OFF</h1></button></a>");  
  // for 3rd button 
  client.println("</div>");

  client.println("<div style = 'display:flex; justify-content: center; gap:100px; margin-top: 50px'>");
  client.println("<a href = '/load4on'><button><h1 style = 'font-size:40px;'>Load(4) ON</h1></button></a>");  
  client.println("<a href = '/load4off'><button><h1 style = 'font-size:40px;'>Load(4) OFF</h1></button></a>");  
  // forth button

  client.println("</body>");      // end the HTML body section
  client.println("</html>");      // end the html document

  client.flush();    // Wait until all outgoing data has been sent to the client
  delay(200);          // Pause the program for 200 milliseconds to ensure data transmission completes
  client.stop();      // Close the connection with the client and free resources
}
}