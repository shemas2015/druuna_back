#include "Preferences.h"
#include "WiFi.h"
#include "BluetoothSerial.h"


String ssid;
String password;
String command;
String server = "https://www.google.com/";
bool userInput = false;
const char * udpAddress = "172.30.1.81";
const int udpPort = 55555;


Preferences preferences;
BluetoothSerial SerialBT;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  WiFi.onEvent(WiFiEvent);
  main_func();
  

}

void loop() {
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
  if( client.connected() ){
    while (client.available()) {
      char c = client.read();
      int del = c - '0';
      Serial.print("Prende ");
      Serial.print(del);
      Serial.println(" segundos");
      delay(del*1000);
      Serial.print("Apaga");
      
    }
    
    
  }
  else{
    connectToHost();
    delay(2000);
  }

  delay(500);
  
}

void connectToHost(){
  if(!client.connect(udpAddress, udpPort)) {
    Serial.println("Connection to host failed");
    delay(2000);
  }else{
    client.print(WiFi.macAddress());
    client.print("1");
  }
  
}


void main_func(){
  preferences.begin("credenciales",false);
  ssid = preferences.getString("ssid","");
  password = preferences.getString("ssid","");

  if( ssid == "" || password ==""){
    Serial.println("Config wifi");
    configWifi();
  }

  connectWifi();

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
  String str;
  int index;
  WiFiServer server(80);
  Serial.println("Configuring wifi...");
  WiFi.softAP("Druuna", "");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  
  server.begin();
  Serial.println("HTTP server started");

  while( ssid == "" || password =="" ){ 
    WiFiClient client= server.available();
    String header;
    if (client) {
      String currentLine = "";
      while (client.connected()) {
        if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        header += c;
        if (c == '\n') {
          client.println("HTTP/1.1 200 OK");
          client.println("Access-Control-Allow-Origin: *");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          client.println(WiFi.macAddress());

          str = header;
          int posit = 0;
          preferences.begin("credenciales",false);
          do{
            index = str.indexOf("dR4n4l");
            if( posit == 1 ){
              ssid = str.substring(0,index);
              preferences.putString("ssid",ssid );
            }
            if( posit == 2 ){
              password = str.substring(0,index);
              preferences.putString("password",password);
            }
            str = str.substring(index  + 6, str.length() );
            posit++;
          }while(index > 0);
          preferences.end();
          currentLine = "";
          client.stop();
        } else if (c != '\r') {
          currentLine += c;
        }
      }
      }
    }
    delay(800);
  }
  Serial.println("Confguración terminada");
  
  
  
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
  
