/*
============================================================
Project Name: SMART HOME AUTOMATION SYSTEM
============================================================
Equipments:-

Main controller : ESP32
Sensors          : DHT11, MQ-135
Actuators        : 4-channel relay, fan
Display          : 16x2 I2C LCD
Interface        : ESP32 Wi-Fi web dashboard

Main functions:
1. Connect ESP32 to Wi-Fi.
2. Display startup/network information on the LCD.
3. Read temperature and humidity from DHT11.
4. Read analog air-quality value from MQ-135.
5. Control four relay channels from a browser.
6. Control fan speed from 0–100% using PWM.
7. Serve a live HTML/CSS/JavaScript dashboard.
8. Update sensor values and relay states periodically.

*[Please Use smartphone to Explore Website better view]*

============================================================
*/

// Include Wi-Fi library so the ESP32 can connect to the local network.
#include<WiFi.h>
// Include Adafruit sensor base library used by DHT sensor support.
#include<Adafruit_Sensor.h>
// Include DHT library for reading temperature and humidity.
#include<DHT.h>
// Include I2C communication library for the LCD.
#include<Wire.h>
// Include the I2C LCD library.
#include<LiquidCrystal_I2C.h>
// Create a 16x2 I2C LCD object at address 0x27.
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "ECE Tech Lab";
const char* password = "79987769";

WiFiServer server(80);

const int fenPin = 33;

// 4 Chennel Relay Module pin declaration

const int Relay_1 = 26;
const int Relay_2 = 27;
const int Relay_3 = 25;
const int Relay_4 = 14;

// Store the ON/OFF state of the four relay channels.
bool relayState[4] = {false, false, false, false};
// GPIO 34 reads the MQ-135 analog air-quality signal.
const int MQ135_PIN = 34;
// GPIO 4 is connected to the DHT11 data pin.
const int dhtpin = 4;
#define DHTTYPE DHT11

DHT dht(dhtpin,DHTTYPE);   // Create the DHT11 sensor object.
//Sensore valu
float temperature = 0.0;
int humidity = 0;

// dht timing
unsigned long previousDHT = 0;
const unsigned long DHT_INTERVAL = 1000;

void setup() {

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("YOUR SMART HOME");
  lcd.setCursor(0,1);
  lcd.print("****<SYSTEM>****");
  delay(4000);
  lcd.clear();

  dht.begin();

  Serial.begin(9600);

  WiFi.begin(ssid,password);

  pinMode(fenPin, OUTPUT);

  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);

  while (WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");

    lcd.setCursor(0,0);
    lcd.print("Conecting...");
  }
  Serial.println("");
  Serial.println("Connected");

  Serial.print("IP Address = ");
  Serial.println(WiFi.localIP());

  lcd.setCursor(0,0);
  lcd.print("WEBSITE IP: ");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  digitalWrite(Relay_1, HIGH);
  digitalWrite(Relay_2, HIGH);
  digitalWrite(Relay_3, HIGH);
  digitalWrite(Relay_4, HIGH);

  server.begin();
}

void loop(){

  if (millis() - previousDHT >= DHT_INTERVAL) {

    previousDHT = millis();

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
  }

  WiFiClient client = server.available();

  if (client) {

    unsigned long startTime = millis();

    while (!client.available() && millis() - startTime < 200) {
      delay(1);
    }

    if (!client.available()) {
      client.stop();
      return;
    }

    String request = client.readStringUntil('\r');
    Serial.println(request);

    //============ temperature & humidity =============

    if (request.indexOf("/temperature") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type: text/html");
      client.println("Connection: close");
      client.println();

      client.println(temperature);
      client.stop();
      return;
    }

    if (request.indexOf("/humidity") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type: text/html");
      client.println("Connection: close");
      client.println();

      client.println(humidity);
      client.stop();
      return;
    }

    //============ Relay =============

    if (request.indexOf("/relay1/on") >= 0) {
      digitalWrite(Relay_1, LOW);
      relayState[0] = true;
    }

    if (request.indexOf("/relay1/off") >= 0) {
      digitalWrite(Relay_1, HIGH);
      relayState[0] = false;
    }

    if (request.indexOf("/relay2/on") >= 0) {
      digitalWrite(Relay_2, LOW);
      relayState[1] = true;
    }

    if (request.indexOf("/relay2/off") >= 0) {
      digitalWrite(Relay_2, HIGH);
      relayState[1] = false;
    }

    if (request.indexOf("/relay3/on") >= 0) {
      digitalWrite(Relay_3, LOW);
      relayState[2] = true;
    }

    if (request.indexOf("/relay3/off") >= 0) {
      digitalWrite(Relay_3, HIGH);
      relayState[2] = false;
    }

    if (request.indexOf("/relay4/on") >= 0) {
      digitalWrite(Relay_4, LOW);
      relayState[3] = true;
    }

    if (request.indexOf("/relay4/off") >= 0) {
      digitalWrite(Relay_4, HIGH);
      relayState[3] = false;
    }

    //============= Relay States ==============

    if (request.indexOf("/relayStates") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();

      client.print(relayState[0]);
      client.print(",");
      client.print(relayState[1]);
      client.print(",");
      client.print(relayState[2]);
      client.print(",");
      client.print(relayState[3]);

      client.stop();
      return;
    }

    //=============Fen Speed==========

    // Receive the fan-speed value from the web slider.

    if (request.indexOf("/motorSpeed?value=") >= 0) {

      int start = request.indexOf("/motorSpeed?value=")+18;
      int end = request.indexOf(" ", start);

      int Speed = request.substring(start, end).toInt();

      int pwmValue = map(Speed,0,100,0,255);   // Convert 0–100% slider value into ESP32 PWM range 0–255.

      analogWrite(fenPin, pwmValue);   // Apply the calculated PWM value to the fan control pin.
    }

    //------------Air Qualiry--------------

    // Read the MQ-135 analog value and return it to the browser.

    if(request.indexOf("/mq135") >= 0){
      int mq135 = analogRead(MQ135_PIN);

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println(mq135);

      client.stop();
      return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();

    //=========================================
    //            html css start
    //=========================================
    client.println("<html>");
    client.println("<head>");
    client.println("<style>");

    // ========= Temperature & Humidity ==========
    
    client.println(".content {");
    client.println("display: flex;");
    client.println("justify-content: center;");
    client.println("gap: 35px;");
    client.println("margin-top: 70px;");
    client.println("}");

    client.println(".circle {");
    client.println("width: 335px;");
    client.println("height: 335px;");
    client.println("border-radius: 50%;");

    client.println("display: flex;");
    client.println("justify-content: center;");
    client.println("align-items: center;");
    client.println("}");

    client.println(".inner {");
    client.println("width: 300px;");
    client.println("height: 300px;");

    client.println("border-radius: 50%;");
    client.println("background-color: rgb(77,76,76);");

    client.println("display: flex;");
    client.println("flex-direction: column;");
    client.println("justify-content: center;");
    client.println("align-items: center;");
    client.println("font-size: 40px;");
    client.println("}");

    client.println("#temValu{");
    client.println("color: rgb(229, 22, 22);");
    client.println("font-style: italic");
    client.println("}");

    client.println("#humValu{");
    client.println("color: aqua;");
    client.println("font-style: italic");
    client.println("}");

    //----Togol awitch---------

    client.println(".switch{");
    client.println("background: red;");
    client.println("}");
    client.println(".switch::before{");
    client.println("content:'';");
    client.println("display: inline-block; width: 50px; height: 50px; background: white; border-radius: 50%; transition: 0.3s; margin-top:5px;");
    client.println("}");

    client.println("input:checked+span{");
    client.println("background: green;");
    client.println("}");

    client.println("input:checked + span::before{transform: translateX(50px);}");

    //==========Slyder=============

    client.println("#motorSpeed{");
    client.println("writing-mode:vertical-lr;");
    client.println("direction:rtl;");
    client.println("width:20px;");
    client.println("height:500px;");
    client.println("appearance:none;");
    client.println("background:transparent;");
    client.println("}");

    client.println("#motorSpeed::-webkit-slider-runnable-track{");
    client.println("width:20px;");
    client.println("height:20px;");
    client.println("border-radius:10px;");
    client.println("background:linear-gradient(to top, red 0%, red var(--value, 0%), #555 var(--value, 0%), #555 100%);");
    client.println("}");

    client.println("#motorSpeed::-webkit-slider-thumb{");
    client.println("appearance:none;");
    client.println("width:35px;");
    client.println("height:35px;");
    client.println("border-radius:50%;");
    client.println("background:red;");
    client.println("cursor:pointer;");
    client.println("margin-left:-7px;");
    client.println("}");

    //============ Air Quality ============= 

    client.println(".content{display:flex;justify-content:center;margin-top:100px}");
    client.println(".gauge{width:700px;height:700px;position:relative;overflow:hidden}");
    client.println(".progress{position:absolute;width:700px;height:700px;border-radius:50%}");
    client.println(".coustom{position:absolute;width:645px;height:645px;border-radius:50%;background:#0f172A;left:27px;top:27px;display:flex;justify-content:center;align-items:center}");
    client.println(".quality{color:rgb(9,218,233);font-size:60px;margin-bottom:10px;font-weight: bold}");
    client.println("#mqValue{font-size:57px;font-weight:bold;font-style: italic;}");
    client.println("#mqStatus{font-size:31px;margin-top:5px}");
    client.println(".text-content{width:100%;display:flex;flex-direction:column;justify-content:center;align-items:center;transform:translateY(-100px)}");

    client.println("</style>");
    client.println("</head>");

    //================= Body ========================

    client.println("<body style='background-color: #0f172A'>");

    client.println("<h1 style='text-align: center; margin-top:70px; color: white; font-size:50px;'>Your Smart Home System</h1>");
    client.println("<p style='text-align:center; color: #94a3b8; font-size:25px;'>Control and monitor your smart home system easily with ESP32</p>");

    client.println("<div class='content'>");

    client.println("<div class='circle' id='temperature'>");
    client.println("<div class='inner'>");
    client.println("<div style='color: rgb(242,123,12); margin-bottom:8px;font-size:32px;'>Temperature&#x1F321;</div>");
    client.println("<div id='temValu'></div>");
    client.println("<div id='temStatus' style='font-size:30px; margin-top:8px; color: rgb(212, 171, 6);'></div>");
    
    client.println("</div>");
    client.println("</div>");

    client.println("<div class='circle' id='humidity'>");
    client.println("<div class='inner'>");
    client.println("<div style='color: aquamarine; margin-bottom: 8px;font-size:32px;margin-left:7px;'>Humidity&#x1F4A7;</div>");
    client.println("<div id='humValu'></div>");
    client.println("<div id='humStatus' style='font-size:30px; margin-top:8px; color: rgb(164, 250, 5);'></div>");

    client.println("</div>");
    client.println("</div>");
    client.println("</div>");

    //----------- Slyder ------------

    client.println("<div style='position:absolute; left:200px; top:710px;'>");

    client.println("<input type='range' id='motorSpeed' min='0' max='100' value='0'>");

    client.println("<p style='color:aqua; font-size:30px; margin-left: -73px;'>Fen Speed: <span id='speedValu'>0</span>%</p>");
    client.println("</div>");

    //--------Togol Switch------------

    client.println("<div style='width:400px; height: 600px;margin-left:450px; margin-top: 50px; border: solid rgb(64, 105, 219); border-radius:13px;'>");
    client.println("<p style='text-align: center; color: rgb(167, 183, 196);font-size:45px;'>Devise's &#x1F6DC;</p>");

    client.println("<div style='display:flex; align-items:center; justify-content:space-between; margin:15px 20px;'>");
    client.println("<p style='margin:0; color: white;font-size:40px;'>&#x1F4A1;1</p>");

    client.println("<label style='cursor: pointer;'>");

    client.println("<input style='display: none;' type='checkbox'>");
    client.println("<span class='switch' style='display: inline-block; width:104px;height:60px;border-radius: 40px;margin-top:1px;'></span>");
    client.println("</label>");
    client.println("</div>");

    client.println("<div style='display:flex; align-items:center; justify-content:space-between; margin:15px 20px;'>");
    client.println("<p style='margin:0; color: white;font-size:40px;'>&#x1F4A1;2</p>");
    client.println("<label style='cursor: pointer;'>");

    client.println("<input style='display:none;' type='checkbox'>");
    client.println("<span class='switch' style='display: inline-block; width:104px;height:60px;border-radius: 40px;margin-top:1px;'></span>");
    client.println("</label>");
    client.println("</div>");

    client.println("<div style='display:flex; align-items:center; justify-content:space-between; margin:15px 20px;'>");
    client.println("<p style='margin:0; color: white;font-size:40px;'>&#9881;PUMP</p>");
    client.println("<label style='cursor: pointer;'>");

    client.println("<input style='display:none;' type='checkbox'>");
    client.println("<span class='switch' style='display: inline-block; width:104px;height:60px;border-radius: 40px;margin-top:1px;'></span>");
    client.println("</label>");
    client.println("</div>");

    client.println("<div style='display:flex; align-items:center; justify-content:space-between; margin:15px 20px;'>");
    client.println("<p style='margin:0; color: white;font-size:40px;'>&#128268;</p>");
    client.println("<label style='cursor: pointer;'>");

    client.println("<input style='display:none;' type='checkbox'>");
    client.println("<span class='switch' style='display: inline-block; width:104px;height:60px;border-radius: 40px;margin-top:1px;'></span>");
    client.println("</label>");
    client.println("</div>");

    client.println("</div>");

    //============= Air Quality==============
    
    client.println("<div class='content'><div class='gauge'>");
    client.println("<div class='progress' id='mqGauge'>");
    client.println("<div class='coustom'><div class='text-content'>");
    client.println("<div class='quality'>Air Quality</div>");
    client.println("<div id='mqValue'>0</div>");
    client.println("<div id='mqStatus'></div>");
    client.println("</div></div></div></div></div>");

    //=====================================
    // =========[ Script Start ]=========
    // ====================================

    client.println("<script>");

    client.println("function updateData(){");

    // ===== TEMPERATURE =====

    client.println("fetch('/temperature')");
    client.println(".then(response => response.text())");
    client.println(".then(data => {");

    client.println("let temperature = parseFloat(data);");
    client.println("let temperatureRing = (temperature / 50) * 100;");

    client.println("document.getElementById('temperature').style.background =");
    client.println("`conic-gradient(");
    client.println("rgb(235, 83, 7) 0% ${temperatureRing}%,");
    client.println("rgb(45, 149, 247) ${temperatureRing}% 100%");
    client.println(")`;");

    client.println("document.getElementById('temValu').innerHTML =");
    client.println("temperature.toFixed(2) + '&deg;C';");

    client.println("let tempStatus = '';");

    client.println("if (temperature < 25) {");
    client.println("tempStatus = 'NORMAL';");
    client.println("} else if (temperature < 35) {");
    client.println("tempStatus = 'HOT';");
    client.println("} else {");
    client.println("tempStatus = 'Extreme';");
    client.println("}");

    client.println("document.getElementById('temStatus').textContent = tempStatus;");

    client.println("});");


    // ===== HUMIDITY =====

    client.println("fetch('/humidity')");
    client.println(".then(response => response.text())");
    client.println(".then(data => {");

    client.println("let humidity = parseFloat(data);");

    client.println("document.getElementById('humidity').style.background =");
    client.println("`conic-gradient(");
    client.println("rgb(60, 51, 242) 0% ${humidity}%,");
    client.println("rgba(176, 170, 189, 0.8) ${humidity}% 100%");
    client.println(")`;");

    client.println("document.getElementById('humValu').textContent =");
    client.println("humidity.toFixed(0) + '%';");

    client.println("let humStatus = '';");

    client.println("if (humidity < 40) {");
    client.println("humStatus = 'DRY';");
    client.println("} else if (humidity < 60) {");
    client.println("humStatus = 'NORMAL';");
    client.println("} else if (humidity < 80) {");
    client.println("humStatus = 'HUMID';");
    client.println("} else {");
    client.println("humStatus = 'Extreme';");
    client.println("}");

    client.println("document.getElementById('humStatus').textContent = humStatus;");

    client.println("});");

    client.println("}");

    client.println("updateData();");
    client.println("setInterval(updateData, 1000);");

    //------------Slyder----------------

    client.println("const slider = document.getElementById('motorSpeed');");
    client.println("const speedValu = document.getElementById('speedValu');");

    client.println("let lastRequest = 0;");

    client.println("slider.addEventListener('input', function() {");

    client.println("speedValu.textContent = slider.value;");

    client.println("let value = slider.value;");
    client.println("this.style.setProperty('--value', value + '%');");

    client.println("let now = Date.now();");

    client.println("if (now - lastRequest >= 30) {");
    client.println("lastRequest = now;");
    client.println("fetch('/motorSpeed?value=' + slider.value);");
    client.println("}");

    client.println("});");

    //------Togol Switch script-------

    client.println("document.querySelectorAll('input[type=checkbox]').forEach((sw,index) => {");
    client.println("sw.addEventListener('change',()=> {");

    client.println("let state = sw.checked ? 'on' : 'off';");
    client.println("fetch('/relay' + (index + 1) + '/' + state);");

    client.println("});");
    client.println("});");
    
    client.println("function updateRelay(){");
    client.println("fetch('/relayStates').then(r=>r.text()).then(data=>{");
    client.println("data.split(',').forEach((s,i)=>document.querySelectorAll('input[type=checkbox]')[i].checked=s=='1');");
    client.println("});");
    client.println("}");
    client.println("updateRelay();");

    //------------------- Air Quality -----------------

    client.println("function updateMQ(){");
    client.println("fetch('/mq135')");
    client.println(".then(response => response.text())");
    client.println(".then(mq135 => {");

    client.println("mq135 = Number(mq135);");
    client.println("let maxValue=1200;");
    client.println("let percentage=(mq135/maxValue)*100;");
    client.println("let degree=(percentage/100)*180;");
    client.println("let status,progressColor;");
    client.println("let value=document.getElementById('mqValue');");
    client.println("let mqStatus=document.getElementById('mqStatus');");

    client.println("if(mq135<=250){");
    client.println("status='VERY CLEAR';");
    client.println("progressColor='rgb(2,224,39)';");
    client.println("value.style.color='#07db0a';");
    client.println("}");

    client.println("else if(mq135<=500){");
    client.println("status='FAIRLY CLEAR';");
    client.println("progressColor='rgb(165,219,2)';");
    client.println("value.style.color='#a5db02';");
    client.println("}");

    client.println("else if(mq135<=700){");
    client.println("status='WARNING';");
    client.println("progressColor='rgb(255,180,0)';");
    client.println("value.style.color='#ffeb3b';");
    client.println("}");

    client.println("else{");
    client.println("status='DANGER';");
    client.println("progressColor='red';");
    client.println("value.style.color='red';");
    client.println("}");

    client.println("document.getElementById('mqGauge').style.background=");
    client.println("`conic-gradient(from 270deg,${progressColor} 0deg ${degree}deg,rgb(45,149,247) ${degree}deg 180deg,transparent 180deg 360deg)`;");

    client.println("value.innerHTML=mq135;");
    client.println("mqStatus.innerHTML=status;");

    client.println("if(status=='VERY CLEAR')mqStatus.style.color='lightgreen';");
    client.println("else if(status=='FAIRLY CLEAR')mqStatus.style.color='#a2d43f';");
    client.println("else if(status=='WARNING')mqStatus.style.color='#e0a43d';");
    client.println("else mqStatus.style.color='red';");

    client.println("})");
    client.println(".catch(error => console.log(error));");
    client.println("}");

    client.println("updateMQ();");
    client.println("setInterval(updateMQ,1000);");

    client.println("</script>");
    client.println("</body>");
    client.println("</html>");

    client.stop();
  }

}