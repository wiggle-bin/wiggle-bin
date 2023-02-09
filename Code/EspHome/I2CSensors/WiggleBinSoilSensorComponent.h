#include "esphome.h"

using namespace esphome;

class WiggleBinSoilSensor : public PollingComponent, public Sensor {
 public:
  WiggleBinSoilSensor() : PollingComponent(15000) {}

  Sensor *temperature_sensor = new Sensor();
  Sensor *moisture_sensor = new Sensor();

  float get_setup_priority() const override { return esphome::setup_priority::BUS; }

  typedef struct {
    byte soilMoist;
    float soilTemp;
  } SoilSensorData_t;

  union FloatToBytes_t 
  { 
    float f; 
    char b[sizeof(float)]; 
  };

  #define PACKET_SIZE 5
  #define soilSensordResponderAddress 8
  
  void setup() override {
    Wire.begin();
  }

  void update() override {
    SoilSensorData_t soilSensor; 
    
    byte index = 0;
    byte I2C_Packet[PACKET_SIZE];

    Wire.requestFrom(soilSensordResponderAddress, PACKET_SIZE);
    
    while (Wire.available()){
      I2C_Packet[index++] = Wire.read();
    }

    soilSensor.soilMoist = I2C_Packet[0];
    
    FloatToBytes_t soilTemp;
    soilTemp.b[0] = I2C_Packet[1];
    soilTemp.b[1] = I2C_Packet[2];
    soilTemp.b[2] = I2C_Packet[3];
    soilTemp.b[3] = I2C_Packet[4];
  
    soilSensor.soilTemp = soilTemp.f;

    temperature_sensor->publish_state(soilSensor.soilTemp);
    moisture_sensor->publish_state(soilSensor.soilMoist);
  }
};