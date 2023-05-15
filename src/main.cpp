#include <Arduino.h>

// tempurature and humidity
#include<Wire.h>
#include<SHT2x.h>

//MQTT
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <REG_CONFIG.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

// tempurature and humidity
uint32_t start;
uint32_t stop;

SHT2x sht;
uint32_t connectionFails = 0;

//WIFI
const char* ssid = "AndriodAP0AAD";
const char* password = "HelloWorld";
const char* MQTT_server = "https://thingcontrol.io/";
const char* user = "sreshthaputrav@gmail.com";
const char* pass = "arsenal412";
const int port = 8883;
const char* id = "esp32dev";

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

void setupMQTT(){
  //Wifi setup
  delay(100);
  Serial.print("\nconnecting to");
  Serial.print(ssid);

  WiFi.begin(ssid,pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("-");
  }
  
  Serial.print("\nConnected to");

  //MQTT setup
  //int FirmwareVersionCheck::setCACert(rootCACertificate);
}
  //connect to broker
void ConnectBroker(){
  client.setServer(MQTT_server, port);
  while(!client.connected()){
    Serial.println("Connecting to MQTT broker . . .");
    if(client.connect(id, user, pass)){
      Serial.println("connnect to MQTT broker");
    }
    else{
      Serial.print("Fail with state");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WIFI
  setupMQTT();

  //connect to broker
  ConnectBroker();

  // tempurature and humidity
  Serial.print("SHT2x_LIB_VERSION: \t");

  sht.begin();

  uint8_t stat = sht.getStatus();
  Serial.print(stat, HEX);
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

  // tempurature and humidity
   if ( sht.isConnected()  )
  {
    start = micros();
    bool b = sht.read();
    stop = micros();

    int error = sht.getError();
    uint8_t stat = sht.getStatus();

    
    Serial.print("Temperatue: \t");
    Serial.println(sht.getTemperature(), 1);
    Serial.print("Humidity: \t");
    Serial.println(sht.getHumidity(), 1);
    delay(30000);
  }
  else
  {
    connectionFails++;
    Serial.print(millis());
    Serial.print("\tNot connected:\t");
    Serial.print(connectionFails);
    // sht.reset();
  }
  Serial.println();
  delay(1000);

  //MQTT
  
}