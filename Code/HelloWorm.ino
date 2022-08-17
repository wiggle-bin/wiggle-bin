// Temperature Sensor
// https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

// Soil Sensor
// https://makersportal.com/blog/2020/5/26/capacitive-soil-moisture-calibration-with-arduino
// on calibration - https://makersportal.com/blog/2020/5/26/capacitive-soil-moisture-calibration-with-arduino

// DHT Sensor
/* DHT Shield - Simple
 *
 * Example testing sketch for various DHT humidity/temperature sensors
 * Written by ladyada, public domain
 *
 * Depends on Adafruit DHT Arduino library
 * https://github.com/adafruit/DHT-sensor-library
 */

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
  
  // set the analog reference to 3.3V
  analogReference(EXTERNAL); 

  // Start the DS18B20 sensor
  sensors.begin();

  // Start the DHT sensor
  dht.begin();
}

void loop() {
  // Temperature
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");

  // Soil Moisture
  Serial.print("Soil Moisture Sensor Voltage: ");
  Serial.print((float(analogRead(soil_pin))/1023.0)*3.3); // read sensor
  Serial.println(" V");

  // DHT Sensor
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
  delay(5000);
}
