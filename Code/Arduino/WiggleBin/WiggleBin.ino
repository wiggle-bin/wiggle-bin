// I2C communication thanks to examples from https://github.com/Scott216/I2C-Examples

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;

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
#define soilSensordResponderAddress 8

bool getSoilData(SoilSensorData_t* soilSensor);

void setup() {
  Wire.begin();
  Serial.begin(115200);

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  // SoilSensor
  SoilSensorData_t soilSensor; 
  if( getSoilData( &soilSensor )) {
    Serial.print("SoilSensor moist: ");
    Serial.println(soilSensor.soilMoist);
    Serial.print("SoilSensor temperature: ");
    Serial.println(soilSensor.soilTemp);
    Serial.println();
  } else {
    Serial.println(F("No packet received from SoilSensor"));
  }

  // BME
  if (bme.performReading()) {
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");
  
    Serial.print("Pressure = ");
    Serial.print(bme.pressure / 100.0);
    Serial.println(" hPa");
  
    Serial.print("Humidity = ");
    Serial.print(bme.humidity);
    Serial.println(" %");
  
    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");
  
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
  } else {
    Serial.println("No BME sensor detected");
  }

  Serial.println();
  
  delay(5000);

}

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
