#include <ESP8266WiFi.h>
#include <microDS18B20.h>
#include <espnow.h>

MicroDS18B20<D1> sensor;

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

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

int soilMoistureValue = 0;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {  
  if ((millis() - lastTime) > timerDelay) {
    sensor.requestTemp();
    
    delay(1000);
    myData.soilTemp = sensor.getTemp();

    soilMoistureValue = analogRead(A0); 
    myData.soilMoisture = soilMoistureValue;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
