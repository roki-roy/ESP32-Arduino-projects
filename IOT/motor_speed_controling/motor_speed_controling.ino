/* project name:- wifi to motor speed control
--------------------------------------------------------------------------
Main parts fo code:-
. WiFi: Connects ESP32 to the WiFi network.
. Web Server: Creates a control webpage.
. HTML: Creates the speed slider.
. JavaScript: Sends the slider value to ESP32.
. HTTP request: Sends values such as /speed?value=50.
. map() function: Converts 0–100% into 0–255 PWM.
. analogWrite(): Outputs the PWM signal from GPIO 25.
. Motor: Its speed is controlled according to the slider position.
---------------------------------------------------------------------------
Equipment:- esp32, DC Motor with(MOSFET/Motor driver)
*/

#include<WiFi.h>

const char* ssid = "esp32 Wifi";          // WiFi network name and password
const char* password = "12345678";

WiFiServer server(80);                   // Create a web server on port 80

const int motorPin = 25;             // Motor is connected to GPIO 25

void setup(){

  pinMode(motorPin, OUTPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);           // Connect ESP32 to WiFi

  // Wait until WiFi connection is established
  while (WiFi.status() !=WL_CONNECTED) {          
    delay(500);

    Serial.println("Connecting...");
  }

  // WiFi connected successfully
  Serial.println("");
  Serial.println("Connected");

  // Print ESP32 IP address
  Serial.print("IP Address = ");
  Serial.print("IP Address = ");       
  Serial.print(WiFi.localIP());

  server.begin();
}

void loop(){

  WiFiClient client = server.available();          // Check if a client/browser has connected

  // Wait until the browser sends a request
  if (client) {
    while (!client.available()) {
      delay(1);
    }

    // Read the HTTP request from the browser
    String request = client.readStringUntil('\r');
    Serial.println(request);

    // Check if the request contains the speed control endpoint
    // Example: /speed?value=50
    if (request.indexOf("/speed?value=")>=0) {
      int startIndex = request.indexOf("value=")+6;            // Find the starting position of the speed value
      int endIndex = request.indexOf(" ", startIndex);          // Find the ending position of the speed value

      int speed = request.substring(startIndex,endIndex).toInt();     // Extract the speed value and convert it to an integer

      int pwmValue = map(speed,0,100, 0,255);      // Convert speed from 0-100% to PWM range 0-255

      analogWrite(motorPin, pwmValue);          // Send PWM signal to the motor pin
    }

    // Send HTTP response to the browser
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    // ========================================
    //              HTML START
    //=========================================
    client.println("<html>");
    client.println("<body style='background-color: black'>");
    client.println("<h1 style='color: rgb(70,70,246); text-align: center; margin-top: 350px; font-size:50px'>Motor Speed Control System</h1>");

    client.println("<div style='text-align: center; margin-top: 50px;'>");

    // Create the motor speed slider
    // Minimum = 0%
    // Maximum = 100%
    // Initial value = 0%
    client.println("<input style='width: 450px; height: 30px;' type='range' id='motorSpeed' min='0' max='100' value='0'>");

    // Display the current motor speed
    client.println("<p style='color: rgb(207,139,244); font-size:35px'>Motor Speed: <span id='speedValu'>0</span>%</p>");
    client.println("</div>");

    // =======================================
    //           JAVASCRIPT START
    // =======================================
    client.println("<script>");
    // Get the slider element
    client.println("const slider = document.getElementById('motorSpeed');");
    // Get the element that displays the speed value
    client.println("const speedValu = document.getElementById('speedValu');");

    // Run this function whenever the slider is moved
    client.println("slider.addEventListener('input', function() {");
    // Display the current slider value
    client.println("speedValu.textContent = slider.value");

    // Send the speed value to the ESP32
    // Example: /speed?value=50
    client.println("fetch('/speed?value='+ slider.value);");
    client.println("});");

    client.println("</script>");    // javascript end

    // HTML End
    client.println("</body>");
    client.println("</html>");

    client.stop();     // close the client connection
  }


}
