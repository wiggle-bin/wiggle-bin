// https://how2electronics.com/interface-capacitive-soil-moisture-sensor-arduino/
// on calibration - https://makersportal.com/blog/2020/5/26/capacitive-soil-moisture-calibration-with-arduino

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <TM1637TinyDisplay.h>

const int MIN_TEMP_WORM_BIN = 15;
const int MAX_TEMP_WORM_BIN = 30;
const int MIN_MOIST_PERCENTAGE_WORM_BIN = 60;
const int MAX_MOIST_PERCENTAGE_WORM_BIN = 85;

const int AIR_VALUE = 4000;   // Value when sensor is dry
const int WATER_VALUE = 2100;  // Value when sensor emerged in water
int soilMoistureValue = 0;
int soilMoisturePercent = 0;

// Structure to receive data
typedef struct struct_message {
  float soilTemp;     
  float soilMoisture;
} struct_message;

// Create a struct_message called myData
struct_message myData;

uint8_t  fixedAddress[]      = {0xe6, 0x66, 0x9f, 0xa5, 0x6e, 0xe9}; // locally managed macAddress

// Define Digital Pins LED Display
#define CLK D2
#define DIO D3

TM1637TinyDisplay display(CLK, DIO);

/* ############################ Setup ############################################ */

void setup() {

  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  wifi_set_macaddr(STATION_IF, &fixedAddress[0]);
  if (esp_now_init() != 0) {
    delay(1000);
    ESP.restart();
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  // Initialize Display
  display.begin();
  
}

/* ############################ Loop ############################################# */

void loop() {
  // TEMP message
  display.showNumber(myData.soilTemp);
  delay(2000);
  
  if (myData.soilTemp > MAX_TEMP_WORM_BIN) {
    display.showString("HOT");
  } else if (myData.soilTemp < MIN_TEMP_WORM_BIN) {
    display.showString("COLD");
  } else {
    display.showString("LOVE");
  }
  delay(2000);
  
  // MOIST message
  Serial.println("soilMoisture");
  Serial.println(myData.soilMoisture);
  soilMoisturePercent = map(myData.soilMoisture, AIR_VALUE, WATER_VALUE, 0, 100);
  display.showNumber(soilMoisturePercent);
  delay(2000);
  
  if (soilMoisturePercent > MAX_MOIST_PERCENTAGE_WORM_BIN) {
    display.showString("WET");
  } else if (soilMoisturePercent < MIN_MOIST_PERCENTAGE_WORM_BIN) {
    display.showString("DRY");
  } else {
    display.showString("LOVE");
  }
  delay(2000);
}

void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  Serial.write(incomingData, len);
  Serial.write('\n');
}
