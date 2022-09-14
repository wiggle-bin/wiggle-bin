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

const char *wiggleBinTypes[] = {"TemperatureSensor", nullptr};
ThingDevice wiggleBin("WiggleBinSensor", "WiggleBin Sensor", wiggleBinTypes);
ThingProperty wiggleBinTemperature("temperature", "", NUMBER, "TemperatureProperty");
ThingProperty wiggleBinHumidity("humidity", "", NUMBER, "HumidityProperty");
ThingProperty wiggleBinHeatIndex("heat index", "", NUMBER, "TemperatureProperty");
ThingProperty wiggleBinSoilTemperature("soil temperature", "", NUMBER, "TemperatureProperty");
ThingProperty wiggleBinSoilMoisture("soil moisture", "", NUMBER, "LevelProperty");
        
// DS18B2 temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>


// DHT humidity/temperature sensors
#include "DHT.h"


// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     


// Simple code for Measuring Voltage from
// Capacitive soil moisture sensor
// Check article on how to set AirValue and WaterValue - https://how2electronics.com/capacitive-soil-moisture-sensor-esp8266-esp32-oled-display/
const int AirValue = 860;
const int WaterValue = 440;
int soil_pin = A0; // AOUT pin on sensor
int soilMoisturePercent = 0;


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
  wiggleBinTemperature.unit = "degree celsius";
  wiggleBinHeatIndex.unit = "degree celsius";
  wiggleBinSoilTemperature.unit = "degree celsius";

  // Set title to "Pressure"
  wiggleBinSoilTemperature.title = "Soil Temperature";
  
  // Set title to "Humidity"
  wiggleBinHumidity.title = "Humidity";
  // Set unit for humidity to %
  wiggleBinHumidity.unit = "percent";
  // Set humidity as read only
  wiggleBinHumidity.readOnly = "true";

  // Set title to "Soil Moisture"
  wiggleBinSoilMoisture.title = "Soil Moisture";
  // Set unit for humidity to %
  wiggleBinSoilMoisture.unit = "percent";
  // Set humidity as read only
  wiggleBinSoilMoisture.readOnly = "true";
  // Set min and max for LevelProperty
  wiggleBinSoilMoisture.minimum = 0;
  wiggleBinSoilMoisture.maximum = 100;

  // Set title to "Heat Index"
  wiggleBinHeatIndex.title = "Heat Index";

  wiggleBin.addProperty(&wiggleBinTemperature);
  wiggleBin.addProperty(&wiggleBinHumidity);
  wiggleBin.addProperty(&wiggleBinHeatIndex);
  wiggleBin.addProperty(&wiggleBinSoilTemperature);
  wiggleBin.addProperty(&wiggleBinSoilMoisture);
 
  adapter->addDevice(&wiggleBin);
  adapter->begin();

  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(wiggleBin.id);
  
  // set the analog reference to 3.3V
  analogReference(EXTERNAL); 

  // Start the DS18B20 sensor
  sensors.begin();

  // Start the DHT sensor
  dht.begin();

  //// GATHERING AND SENDING INFORMATION
  
   // WebThings
  ThingPropertyValue value;
  
  // Temperature
  sensors.requestTemperatures(); 
  // Use getTempFByIndex to het Fahrenheit
  value.number = sensors.getTempCByIndex(0);
  wiggleBinSoilTemperature.setValue(value);

  // Soil Moisture  
  soilMoisturePercent = map(analogRead(soil_pin), AirValue, WaterValue, 0, 100);
  if(soilMoisturePercent > 100) {
    soilMoisturePercent = 100;
  } else if(soilMoisturePercent < 0) {
    soilMoisturePercent = 0;
  }
  value.number = soilMoisturePercent;
  wiggleBinSoilMoisture.setValue(value);
  
  // DHT Sensor
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default), use dht.readTemperature(true) for Fahrenheit
  float t = dht.readTemperature();

  value.number = t;
  wiggleBinTemperature.setValue(value);
  value.number = h;
  wiggleBinHumidity.setValue(value);
  value.number = dht.computeHeatIndex(t, h, false);
  wiggleBinHeatIndex.setValue(value);

  // Send update
  adapter->update();

  Serial.println("Update send to WebThings");
  
  delay(50000);

  Serial.println("Deep Sleep");
  ESP.deepSleep(1*60*1000000); // 1 minute sleep
}


void loop() {
 
}
