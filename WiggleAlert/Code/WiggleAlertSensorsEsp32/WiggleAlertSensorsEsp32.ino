#include <WiFi.h>
#include <DallasTemperature.h>
#include <esp_now.h>

#define SOIL_MOISTURE_PIN A0
#define SOIL_TEMP_PIN 1 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(SOIL_TEMP_PIN);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xe6, 0x66, 0x9f, 0xa5, 0x6e, 0xe9};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float soilTemp;
  float soilMoisture;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

int soilMoistureValue = 0;
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

 
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Start the DS18B20 sensor
  sensors.begin();
}
 
void loop() {  
    sensors.requestTemperatures(); 
    myData.soilTemp = sensors.getTempCByIndex(0);
    Serial.println(sensors.getTempCByIndex(0));

    soilMoistureValue = analogRead(SOIL_MOISTURE_PIN); 
    myData.soilMoisture = soilMoistureValue;
    Serial.println(soilMoistureValue);

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }

    delay(2000);
}
