#include "NetworkSettings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

/* ------------------------ Change Me --------------------------------------- */

const char*     ssid              = WIFI_SSID;
const char*     password          = WIFI_PASS;
const char*     mqtt_server       = MQTT_SERVER;
const char*     mqtt_username     = MQTT_USERNAME;
const char*     mqtt_password     = MQTT_PASSWORD;

/* ------------------------------------------------------------------ */

#define DEBUG_FLAG = true;

#ifdef DEBUG_FLAG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

/* ------------------------ Messages --------------------------------------- */

typedef struct struct_message {
  float soilTemp;
  float soilMoisture;
} struct_message;

struct_message  msg;
uint8_t         incomingData[sizeof(struct struct_message)];
size_t          received_msg_length;

/* --------------------------- Wifi/MQTT ---------------------------------------------- */

WiFiClient    wifiClient;
#define       MQTT_MSG_SIZE    200
char          mqttTopic[MQTT_MSG_SIZE];
#define       MSG_BUFFER_SIZE  (50)
PubSubClient  mqttClient(wifiClient);
String        thingName;
const char*   willTopic         = "LWT";
const char*   willMessage       = "offline";
boolean       willRetain        = false;
byte          willQoS           = 0;

/* ############################ Setup ############################################ */

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  while (WiFi.status() != WL_CONNECTED) {
    debug(".");
    delay(1000);
  }
  debug("IP address:\t");
  debugln(WiFi.localIP());

  mqttClient.setServer( mqtt_server , 1883);
  String MAC = WiFi.macAddress();
  MAC.replace(":", "");
  thingName = "Wiggle_Bin_" + MAC;

}

/* ############################ Loop ############################################# */

void loop() {
  mqttReconnect();
  mqttClient.loop();

  if (Serial.available()) {
    received_msg_length = Serial.readBytesUntil('\n', incomingData, sizeof(incomingData));
    debugln("Serial available");
    if (received_msg_length == sizeof(incomingData)) {  // got a msg from a sensor
      memcpy(&msg, incomingData, sizeof(msg));
      sensorMessageOverMQTT();
    }
  }
}

/* ############################ Sensors ############################################# */

void sensorMessageOverMQTT() {

  char macAddr[18];
  DynamicJsonDocument sensor(256);

  debugln("SoilTemp");
  debugln(msg.soilTemp);
  debugln("SoilMoisture");
  debugln(msg.soilMoisture);

  sensor["data"]["soilTemp"] = (float)msg.soilTemp;
  sensor["data"]["soilMoisture"] = (float)msg.soilMoisture;

  char payload[100];
  size_t n = serializeJson(sensor, payload);
  mqttPublish(macAddr, payload, n );

}

/* ################################# MQTT ########################################### */

void mqttReconnect() {
  if ( mqttClient.connected() ) return;
  if (mqttClient.connect( thingName.c_str(), mqtt_username, mqtt_password, "homeassistant/status", willQoS, willRetain, willMessage)) {
    mqttClient.publish("wiggle_bin/status", "online");
    mqttClient.publish("homeassistant/status", "online");
    debugln(F("wiggle_bin/status ... online "));
  }
}

void mqttPublish(char macAdress[], String payload,  size_t len ) {

  strcpy (mqttTopic, "wiggle_bin/data");
  debug(mqttTopic);
  debug(' ');
  debugln(payload);
  mqttClient.publish(mqttTopic, payload.c_str() , len);
}
