#include <ESP8266WiFi.h>
#include <espnow.h>
#include <TM1637TinyDisplay.h>

// Define Digital Pins LED Display
#define CLK D2
#define DIO D3

TM1637TinyDisplay display(CLK, DIO);

// Structure to receive data
typedef struct struct_message {
  float soilTemp;     
  float soilMoisture;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  Serial.print("Soil Temperature: ");
  Serial.println(myData.soilTemp);
  
  display.showNumber(myData.soilTemp);

  Serial.print("Soil Moisture: ");
  Serial.println(myData.soilMoisture);
  
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Display
  display.begin();
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
