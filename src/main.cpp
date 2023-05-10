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
const char* ssid = "AndriodAP0AAD";
const char* pass = "HelloWorld";
const char* MQTT_server = "https://thingcontrol.io/";
const char* MQTT_username = "sreshthaputrav@gmail.com";
const char* MQTT_pass = "arsenal412";
const int port = 8883;
const char* CAcert = "rootCACertificate";

WiFiClientSecure secureClient;
PubSubClient client(secureClient);

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
  secureClient.setCACert(CAcert);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WIFI
  setupMQTT();

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