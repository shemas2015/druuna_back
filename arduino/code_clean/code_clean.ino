#include "Preferences.h"
#include "WiFi.h"
#include "BluetoothSerial.h"


String ssid;
String password;
String command;
String server = "https://www.google.com/";
bool userInput = false;


Preferences preferences;
BluetoothSerial SerialBT;


void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  WiFi.onEvent(WiFiEvent);
  //main_func();

  configWifi();
  

}

void loop() {
  
  /*
  // Reset booton
  int pinState = digitalRead(0);

  
  if(pinState == 0){
      reset();
  }


  int status_wifi = WiFi.status();
  if( status_wifi != 3 ){
    //red Led
    connectWifi();
    delay(5000);
  }

  //Check internet conection
  */
  delay(500);
  
}


void main_func(){
  preferences.begin("credenciales",false);
  ssid = preferences.getString("ssid","");
  password = preferences.getString("ssid","");

  if( ssid == "" || password ==""){
    Serial.println("Config wifi");
    configWifi();
  }
  else
  {
    connectWifi();
  }

  

}

void connectWifi(){
  preferences.begin("credenciales",false);
  ssid = preferences.getString("ssid","");
  password = preferences.getString("password","");
  preferences.end();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
}

/**
 * Reset wifi configuration
 */
void reset(){
  preferences.begin("credenciales",false);
  preferences.putString("ssid","");
  preferences.putString("password","");
  ssid = "";
  password = "";
  preferences.end();
  main_func();
}


void configWifi(){
  WiFiServer server(80);
  Serial.println("Configuring wifi...");
  WiFi.softAP("Druuna", "password");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
  Serial.println("HTTP server started");

  while( ssid == "" || password =="" ){ 
    WiFiClient client= server.available();
    String header;
    String output26State = "off";
    String output27State = "off";

    // Assign output variables to GPIO pins
    const int output26 = 26;
    const int output27 = 27;

  
    if (client) {                             // If a new client connects,
      Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {
        Serial.println("conectado");
        if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
      }

    }
    Serial.println("pasó!");
    delay(800);
    /*
    if (SerialBT.available()) {
      while(SerialBT.available()){
        char incomingChar = SerialBT.read();
        tmpWifi += String(incomingChar);
      }
      if( ssid == "" ){
        ssid  = tmpWifi ;
      }else{
        password = tmpWifi ;
      }
      Serial.println(tmpWifi);
      tmpWifi = "";
      SerialBT.write(1);
      
    }
    */
    delay(500);
  }
  /*
  if( ssid != "" && password != ""){
    preferences.begin("credenciales",false);
    preferences.putString("ssid",ssid);
    preferences.putString("password",password);
    preferences.end();
    SerialBT.end();
    //Apaga indicador de conexión
    //OJO!!! <- Aqui detiene el bluetooth
  }
  */
  
  
  
}


void WiFiEvent(WiFiEvent_t event){
    switch(event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("STA Connected");
            WiFi.enableIpV6();
            break;
        case SYSTEM_EVENT_AP_STA_GOT_IP6:
            Serial.print("STA IPv6: ");
            Serial.println(WiFi.localIPv6());
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("STA IPv4: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("STA Disconnected");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("STA Stopped");
            break;
        default:
            break;
    }
}
  
