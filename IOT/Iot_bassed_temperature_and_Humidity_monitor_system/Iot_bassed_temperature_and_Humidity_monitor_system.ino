/* project name:- iot bassed temperature & Humidity monitoring
-----------------------------------------------------------------------------------------------------------
Project Summary:
This project is an ESP32-based Temperature and Humidity Monitoring System. The DHT11 sensor measures
the surrounding temperature and humidity, while the ESP32 creates a web server and connects to Wi-Fi. The
sensor data is displayed on a web page using JavaScript. The fetch() function requests updated data from
the ESP32, and setInterval() automatically updates the values every 1 second without reloading the page.
-------------------------------------------------------------------------------------------------------------
*/

#include<WiFi.h>
#include<Adafruit_Sensor.h>
#include<DHT.h>

// Wi-Fi network information
const char* ssid = "OPPO A60";
const char* password = "12345678";

// Create a web server on port 80
WiFiServer server(80);

// DHT11 sensor pin
const int dhtPin = 4;
#define DHTTYPE DHT11        // Define the DHT sensor type

DHT dht(dhtPin,DHTTYPE);     // Create DHT sensor object

void setup(){

  dht.begin();           // Initialize the DHT11 sensor

  Serial.begin(9600);

  WiFi.begin(ssid,password);

  // Wait until Wi-Fi connection is established
  while (WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.print("IP Address = ");            // Display the ESP32 IP address
  Serial.println(WiFi.localIP());

  server.begin();                          // Start the web server
}

void loop(){

  float temperature = dht.readTemperature();          // Read temperature from DHT11

  float humidity = dht.readHumidity();              // Read humidity from DHT11

  WiFiClient client = server.available();

  if (client) {

    // Wait for the browser to send a request
    while (!client.available()) {
    delay(1);
    }

    // Read the HTTP request
    String request = client.readStringUntil('\r');
    Serial.println(request);      // Print the request in Serial Monitor

    // Handle temperature request
    if (request.indexOf("/temperature") >=0) {

      // Send HTTP response
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();

      client.print(temperature);                // Send temperature value to the browser
      client.stop();                       // Close the connection

      return;
    }

    // Handle humidity request
    if (request.indexOf("/humidity") >=0 ){

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();

      client.print(humidity);
      client.stop();

      return;
    }

    // Send the main web page
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html>");
    client.println("<body style='background-color: darkslateblue;'>");
    // Project title
    client.println("<h1 style='color:white; text-align: center;'>Temperature & Humidity Monitor System</h1>");
    // Project description
    client.println("<p style='color:white; text-align:center;'>Monitor your environment anytime,anywher</p>");
    // Create a flex container for the sensor boxes
    client.println("<div style='display:flex; justify-content:center;gap:40px;margin-top:30px;'>");
    // Temperature box
    client.println("<div style='height:150px;width: 154px; background-color:#1f2937;border:3px solid red;border-radius:15px;text-align:center;'>");
    client.println("<h2 style='color:blanchedalmond;font-style:italic'>Temperature&#127777;</h2>");
    client.println("<i id='temperature' style='color:red;font-size: 30px;'>Loading...</i>");     // Element where temperature will be displayed
    client.println("</div>");
    // Humidity box
    client.println("<div style='height:150px;width: 150px; background-color:#1f2937;border:3px solid aqua;border-radius:15px;text-align:center;'>");
    client.println("<h2 style='color:blanchedalmond;font-style:italic'>Humidity&#x1F4A7;</h2>");
    client.println("<i id='humidity' style='color:aqua;font-size:30px;'>Loading...</i>");        // Element where humidity will be displayed
    client.println("</div>");

    client.println("</div>");

    // Start JavaScript
    client.println("<script>");

    client.println("function updateData(){");           // Function to update sensor data

    // Request temperature data from ESP32
    client.println("fetch('/temperature')");
    client.println(".then(response => response.text())");
    client.println(".then(data => {");
    client.println("document.getElementById('temperature').innerHTML = data+' &deg;C';");     // Display temperature on the webpage
    client.println("});");

    // Request humidity data from ESP32
    client.println("fetch('/humidity')");
    client.println(".then(response => response.text())");
    client.println(".then(data => {");
    client.println("document.getElementById('humidity').textContent = data+ ' %';");         // Display humidity on the webpage
    client.println("});");

    client.println("}");         // End updateData function

    client.println("updateData();");                          // Run the function once immediately
    client.println("setInterval(updateData,1000);");         // Update sensor data every 1 second

    client.println("</script>");         // End JavaScript

    client.println("</body>");
    client.println("</html>");

    client.stop();                  // Close the client connection
  }
}


