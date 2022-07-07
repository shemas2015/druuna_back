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
  
    if (client) {                             // If a new client connects,
      Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {
        if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          
          currentLine = "";
          // Break out of the while loop
          break;
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
      }
    }
    delay(800);
    
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
  
