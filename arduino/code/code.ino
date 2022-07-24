#include "Preferences.h"
#include "WiFi.h"
#include "BluetoothSerial.h"

//RGB INDICATOR
#define LED_RED 21
#define LED_GREEN 22
#define LED_BLUE 23
#define VOLT 19
#define RELE 18

String ssid;
String password;
String command;
String server = "https://www.google.com/";
bool userInput = false;
const char * udpAddress = "34.132.144.174";
const int udpPort = 8090;


Preferences preferences;
BluetoothSerial SerialBT;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(VOLT, OUTPUT);
  pinMode(RELE, OUTPUT);
  digitalWrite(VOLT, HIGH);
  indicator(0);
  digitalWrite(RELE, HIGH);


  
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
      digitalWrite(RELE, LOW);
      Serial.print(del);
      Serial.println(" segundos");
      delay(del*1000);
      digitalWrite(RELE, HIGH);
      Serial.print("Apaga");
      
    }
    
    
  }
  else{
    connectToHost();
  }

  delay(500);
  
}

void connectToHost(){
  if(!client.connect(udpAddress, udpPort)) {
    
  }else{
    client.print(WiFi.macAddress());
    client.print("1");
    delay(1000);
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

void indicator(int opc){
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  switch(opc){
    case 1:
      digitalWrite(LED_RED, LOW);
      break;
    case 2:
      digitalWrite(LED_GREEN, LOW);
      break;
    case 3:
      digitalWrite(LED_BLUE, LOW);
      break;
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
    indicator(3);
    delay(400);
    indicator(0);
    delay(400);
  }
  Serial.println("Confguración terminada");
  
  
  
}




void WiFiEvent(WiFiEvent_t event){
    switch(event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            indicator(2);
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
            indicator(1);
            Serial.println("STA Disconnected");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("STA Stopped");
            break;
        default:
            break;
    }
}
  
