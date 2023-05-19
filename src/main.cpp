#include <Arduino.h>

// tempurature and humidity
#include<Wire.h>
#include<SHT2x.h>

//MQTT
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// tempurature and humidity
uint32_t start;
uint32_t stop;

SHT2x sht;
uint32_t connectionFails = 0;

//WIFI
const char* ssid = "MyWifi";
const char* password = "HelloWorld";
const char* MQTT_server = "mqtt.thingcontrol.io";
const char* user = "202539";
const char* pass = "";
const int port = 8883;
const char* id = "esp32dev";

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

void setupWifi(){
  //Wifi setup
  delay(100);
  Serial.print("\nconnecting to\t");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10000);
    Serial.print("-");
  }
  
  Serial.print("\nConnected to ");
  
}
  //connect to broker
void ConnectBroker(){
  client.setServer(MQTT_server, port);
  while(client.connected()){
    Serial.print("\nConnecting to MQTT broker . . .");
    if(!client.connect("esp32dev", user, pass)){
    }
    else{
      Serial.print("\nFail with state");
      Serial.println(client.state());
      delay(20000);
    }
  }
}
//Reconnect
void ReconnectBroker(){
  if (!client.connected()){
    Serial.print("\ndisconnected from Broker");
    ConnectBroker();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WIFI
  setupWifi();
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());

  //connect to broker
  ConnectBroker();
  Serial.print("\nconnnected to MQTT broker");

  // tempurature and humidity
  Serial.print("\nSHT2x_LIB_VERSION: \t");

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
  delay(10000);

  //MQTT
  ReconnectBroker();
}