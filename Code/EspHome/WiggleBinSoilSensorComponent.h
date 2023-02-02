#include "esphome.h"

using namespace esphome;

bool getSoilData(SoilSensorData_t* soilSensor);

class WiggleBinSoilSensor : public PollingComponent, public Sensor {
 public:
  typedef struct {
    byte soilMoist;
    float soilTemp;
  } SoilSensorData_t;

  typedef union FloatToBytes_t 
  { 
    float f; 
    char b[sizeof(float)]; 
  };

  #define PACKET_SIZE 5

  WiggleBinSoilSensor() : PollingComponent(15000) {}

  float get_setup_priority() const override { return esphome::setup_priority::BUS; }

  void setup() override {
    Wire.begin();
  }

  void write_state(float state) override {
    SoilSensorData_t soilSensor; 
    if(getSoilData( &soilSensor )) {
        publish_state(soilSensor.soilMoist);
    }
  }
};

bool getSoilData(SoilSensorData_t* soilSensor)
{
  bool gotI2CPacket = false;  
  byte index = 0;
  byte I2C_Packet[PACKET_SIZE];

  Wire.requestFrom(soilSensordResponderAddress, PACKET_SIZE);
  
  while (Wire.available()){
      I2C_Packet[index++] = Wire.read();
      gotI2CPacket = true;
  }

  if(gotI2CPacket) {
      gotI2CPacket = false;

      soilSensor->soilMoist = I2C_Packet[0];
      
      FloatToBytes_t soilTemp;
      soilTemp.b[0] = I2C_Packet[1];
      soilTemp.b[1] = I2C_Packet[2];
      soilTemp.b[2] = I2C_Packet[3];
      soilTemp.b[3] = I2C_Packet[4];
    
      soilSensor->soilTemp = soilTemp.f;

      return true;
   } else { 
      return false; 
   }
}