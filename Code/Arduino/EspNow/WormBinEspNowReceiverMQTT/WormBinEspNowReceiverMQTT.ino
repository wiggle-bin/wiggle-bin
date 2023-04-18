#include <ESP8266WiFi.h>
#include <espnow.h>
#include <TM1637TinyDisplay.h>

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

void loop() {}

void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  display.showNumber(myData.soilTemp);
 
  Serial.write(incomingData, len);
  Serial.write('\n');
}
