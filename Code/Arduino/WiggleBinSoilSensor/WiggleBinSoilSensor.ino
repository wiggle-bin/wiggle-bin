#include <TinyWireS.h>
#include <microDS18B20.h>

typedef struct {
  byte soilMoist;
  float soilTemp;
} SoilSensorData_t;

typedef union I2C_Packet_t{
 SoilSensorData_t sensor;
 byte I2CPacket[sizeof(SoilSensorData_t)];
};

#define PACKET_SIZE 5

I2C_Packet_t soilSensor;  

const int AirValue = 880;
const int WaterValue = 480;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;

MicroDS18B20<3> DS18B20sensor;

void setup() {  
  TinyWireS.begin(8);                
  TinyWireS.onRequest(requestEvent);
}

void loop() {
  TinyWireS_stop_check();
}

void requestEvent()
{
  // Soil Moist
  soilMoistureValue = analogRead(A2); 
  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  soilSensor.sensor.soilMoist = soilMoisturePercent; 
  
  // Soil Temperature
  DS18B20sensor.requestTemp();
  soilSensor.sensor.soilTemp = DS18B20sensor.getTemp();

  // Send data over I2C
  for (int k=0; k < PACKET_SIZE; k++) {
    TinyWireS.send(soilSensor.I2CPacket[k]);
  }
}
