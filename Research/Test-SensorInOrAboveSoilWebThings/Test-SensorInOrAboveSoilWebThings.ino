#import "NetworkSettings.h"

// WebThings schema
// https://webthings.io/schemas
// WebThings code bases on
// https://github.com/WebThingsIO/webthing-arduino/blob/master/examples/BME280/BME280.ino

// WebThings
#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char* ssid = NETWORK_SSID;
const char* password = NETWORK_PASSWORD;

WebThingAdapter *adapter;

const char *helloWormTypes[] = {"TemperatureSensor", nullptr};
ThingDevice helloWorm("HelloWormSensor", "Hello Worm Sensor", helloWormTypes);
ThingProperty helloWormTemperature("temperature", "", NUMBER, "TemperatureProperty");
ThingProperty helloWormHumidity("humidity", "", NUMBER, "HumidityProperty");
ThingProperty helloWormHeatIndex("heat index", "", NUMBER, "TemperatureProperty");
ThingProperty helloWormSoilTemperature("soil temperature", "", NUMBER, "TemperatureProperty");
ThingProperty helloWormSoilMoisture("soil moisture", "", NUMBER, "LevelProperty");
        
// DS18B2 temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>


// DHT humidity/temperature sensors
#include "DHT.h"


// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     


// Simple code for Measuring Voltage from
// Capacitive soil moisture sensor
int soil_pin = A0; // AOUT pin on sensor


// DHT humidity/temperature sensors
#define DHTPIN D4  


// Uncomment whatever type DHT you are using
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);


// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Connect to WIFI
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
  #if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
  #endif
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  adapter = new WebThingAdapter("w25", WiFi.localIP());

// Set unit for temperature
  helloWormTemperature.unit = "degree celsius";
  helloWormHeatIndex.unit = "degree celsius";
  helloWormSoilTemperature.unit = "degree celsius";

  // Set title to "Pressure"
  helloWormSoilTemperature.title = "Soil Temperature";
  
  // Set title to "Humidity"
  helloWormHumidity.title = "Humidity";
  // Set unit for humidity to %
  helloWormHumidity.unit = "percent";
  // Set humidity as read only
  helloWormHumidity.readOnly = "true";

  // Set title to "Soil Moisture"
  helloWormSoilMoisture.title = "Soil Moisture";
  // Set unit for humidity to %
  helloWormSoilMoisture.unit = "percent";
  // Set humidity as read only
  helloWormSoilMoisture.readOnly = "true";
  // Set min and max for LevelProperty
  helloWormSoilMoisture.minimum = 0;
  helloWormSoilMoisture.maximum = 100;

  // Set title to "Heat Index"
  helloWormHeatIndex.title = "Heat Index";

  helloWorm.addProperty(&helloWormTemperature);
  helloWorm.addProperty(&helloWormHumidity);
  helloWorm.addProperty(&helloWormHeatIndex);
  helloWorm.addProperty(&helloWormSoilTemperature);
  helloWorm.addProperty(&helloWormSoilMoisture);
 
  adapter->addDevice(&helloWorm);
  adapter->begin();

  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(helloWorm.id);
  
  // set the analog reference to 3.3V
  analogReference(EXTERNAL); 

  // Start the DS18B20 sensor
  sensors.begin();

  // Start the DHT sensor
  dht.begin();
}


void loop() {
  // WebThings
  ThingPropertyValue value;
  
  // Temperature
  sensors.requestTemperatures(); 
  // Use getTempFByIndex to het Fahrenheit
  value.number = sensors.getTempCByIndex(0);
  helloWormSoilTemperature.setValue(value);

  // Soil Moisture  
  value.number = ( 100 - ( (analogRead(soil_pin)/1023.00) * 100 ));
  helloWormSoilMoisture.setValue(value);
  
  // DHT Sensor
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default), use dht.readTemperature(true) for Fahrenheit
  float t = dht.readTemperature();
  
  value.number = t;
  helloWormTemperature.setValue(value);
  value.number = h;
  helloWormHumidity.setValue(value);
  value.number = dht.computeHeatIndex(t, h, false);
  helloWormHeatIndex.setValue(value);

  // Send update
  adapter->update();
  
  delay(5000);
}
