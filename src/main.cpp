/*#################################An example to connect thingcontro.io MQTT over TLS1.2###############################

*/
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

// Modbus
#include "REG_CONFIG.h"
#include <HardwareSerial.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <time.h>
#include <WiFi.h>

#include <stdbool.h>

#define WIFI_AP  "MyWifi"
#define WIFI_PASSWORD  "HelloWorld"

String deviceToken = "202539";  //Sripratum@thingcontrio.io
//String deviceToken = "tOkvPadbQqLFsmc0sCON";
char thingsboardServer[] = "mqtt.thingcontrol.io";
int status = WL_IDLE_STATUS;
String downlink = "";
char *bString;
int PORT = 8883;

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

WiFiManager wifiManager;

void reconnectMqtt()
{
  wifiClient.setInsecure();
  if ( client.connect("e86009f0-ed53-11ed-ba9d-dd9676207dd2", deviceToken.c_str(), NULL) )
  {
    Serial.println( F("Connect MQTT Success."));
  }
  else
  {
    Serial.print(F("Failed to Connect MQTT"));
  }
}

void setup() 
{

  Serial.begin(115200);

  Serial.println();
  Serial.println(F("***********************************"));

  if (!wifiManager.autoConnect(WIFI_AP, WIFI_PASSWORD)) 
  {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    //    ESP.reset();
    delay(10000);
  }

  //  wifiClient.setFingerprint(fingerprint);
  client.setServer( thingsboardServer, PORT );
  Serial.println(client.connected());
  reconnectMqtt();
  
  Serial.print("Start..");

}

void loop()
{
  status = WiFi.status();
  if ( status == WL_CONNECTED)
  {
    if ( !client.connected() )
    {
      reconnectMqtt();
    }
    delay(5000);
    client.loop();
  }
  if (currentMillis - starSendTeletMillis >= periodSendTelemetry)  //test whether the period has elapsed
  {
    readAnalog();
    sendtelemetry();
    writeSD();
    starSendTeletMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}

void getMac()
{
  Serial.println("OK");
  Serial.print("+deviceToken: ");
  Serial.println(WiFi.macAddress());
}

void viewActive()
{
  Serial.println("OK");
  Serial.print("+:WiFi, ");
  Serial.println(WiFi.status());
  if (client.state() == 0)
  {
    Serial.print("+:MQTT, [CONNECT] [rc = " );
    Serial.print( client.state() );
    Serial.println( " : retrying]" );
  }
  else
  {
    Serial.print("+:MQTT, [FAILED] [rc = " );
    Serial.print( client.state() );
    Serial.println( " : retrying]" );
  }
}

void setWiFi()
{
  Serial.println("OK");
  Serial.println("+:Reconfig WiFi  Restart...");
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  if (!wifiManager.startConfigPortal("ThingControlCommand"))
  {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    //    ESP.reset();
    delay(5000);
  }
  Serial.println("OK");
  Serial.println("+:WiFi Connect");
  //  wifiClient.setFingerprint(fingerprint);
  client.setServer( thingsboardServer, PORT );  // secure port over TLS 1.2
  reconnectMqtt();
}


void processToken()
{
  char *aString;
  
  //  aString = cmdHdl.readStringArg();
  Serial.println("OK");
  Serial.print("+:deviceToken , ");
  Serial.println(F(aString));
  deviceToken = aString;
  reconnectMqtt();
}

void unrecognized(const char *command)
{
  Serial.println("ERROR");
}