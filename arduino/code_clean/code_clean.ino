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
  Serial.println("Configuring wifi...");
  //indicador de espera de conexión
  String tmpWifi = "";
  //Desconecta wifi para activa bluetooth
  WiFi.disconnect(true);
  
  
  //Lee por bluetooth las credenciales y las asigna
  SerialBT.begin("Druuna Fuck Machine"); //Bluetooth device name

  while( ssid == "" || password =="" ){ 
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
    delay(100);
  }

  if( ssid != "" && password != ""){
    preferences.begin("credenciales",false);
    preferences.putString("ssid",ssid);
    preferences.putString("password",password);
    preferences.end();
    SerialBT.end();
    //Apaga indicador de conexión
    //OJO!!! <- Aqui detiene el bluetooth
  }
  
  
  
  
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
  
