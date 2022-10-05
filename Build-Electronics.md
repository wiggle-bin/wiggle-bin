# WiggleBin Electronics

## WiggleBin Sensors

### Materials
- [D1 Mini](https://www.tinytronics.nl/shop/en/development-boards/microcontroller-boards/with-wi-fi/d1-mini-esp8266-12f-ch340) or any other ESP8266 device
- [Capacitive Soil Moisture sensor](https://www.tinytronics.nl/shop/en/sensors/liquid/capacitive-soil-moisture-sensor-module-with-cable). Make sure to buy a good one - https://www.youtube.com/watch?v=IGP38bz-K48.
- [1MΩ resistor](https://www.tinytronics.nl/shop/en/components/resistors/resistors/1m%CF%89-resistor)
- [DS18B20 TO-92 Thermometer](https://www.tinytronics.nl/shop/en/sensors/temperature/ds18b20-to-92-thermometer-temperature-sensor-with-cable-waterproof-high-temperature-1m) (soil temperature)
- [DS18B20 Adapter](https://www.tinytronics.nl/shop/en/sensors/temperature/ds18b20-adapter) or  4.7kΩ resistor
- [DHT22 Thermometer](https://www.tinytronics.nl/shop/en/sensors/air/humidity/dht22-thermometer-temperature-and-humidity-sensor-module-with-cables) (temperature and humidity)

### Equipment

- Soldering iron
- DuPont wires

### Connecting the sensors

Setup the D1 Mini to be compatible with the Arduino IDE. Follow [these steps](https://github.com/esp8266/Arduino#installing-with-boards-manager).

Connect the sensors on the breadboard according to the schematic below.

![](Design/HelloWormBreadboardFritzing.png)

Fritzing file is available in [Design/HelloWorm.fzz](Design/HelloWorm.fzz)

Here are some real life pictures for reference.

| ![](Images/Electronics/Electronics_Wemos_Close_up.jpg) | ![](Images/Electronics/Electronics_Together.jpg) | ![](Images/Electronics/Electronics_Battery_Temperature.jpg)
|-|-|-|

Upload the code from [Code/WiggleBinSerial](Code/WiggleBinSerial). You can now view the incoming data from the Arduino Serial Monitor.

### Battery

Because the WiggleBin will usually be placed outside in the garden or balcony we added the option to make it battery powered.

Now batteries are not great for nature (chemicals and such). Therefore we created an experimental adapter which can be 3D printed and holds multiple types of batteries re-used from that old phone in your cabinet.

The 3D files for this can be found in `Design/WiggleBinV001/BatteryHolder`.

Wire the wires trough the holes and tighten the battery in with a bolt.

> **Warning**
> Batteries can be dangerous. Please be careful and only proceed when you have enough electronics experience. Otherwise use a battery supply off the shelf of an electronics store or a 5V wall adapter.

| ![](Images/WiggleBinV001/BatteryHolder/BatteryHolder_Top.jpg) | ![](Images/WiggleBinV001/BatteryHolder/BatteryHolder_Side.jpg) | 
|-|-|

## WiggleBin Central

To collect data from the WiggleBin Sensors we need a central unit. 

The Raspberry Pi with [Home Assistant](https://webthings.io) installed acts as a central unit to display sensor info. Home Assistant can also trigger alerts when a sensor passes a certain value, perfect for detecting unhappy worms.

### Materials
- Raspberry Pi

### Home Asisstant on Raspberry Pi

Follow the [Home Assistant start guide](https://www.home-assistant.io/installation/) to set up a Home Assistant station.

After installing Home Assistant follow the [ESPHome start guide](https://esphome.io/guides/getting_started_hassio.html).

This is the config for your WiggleBin Home Assistant sensor. For the DS18B20 sensor you have to [find the address](https://everythingsmarthome.co.uk/building-a-temperature-sensors-for-home-assistant-wemos-d1-mini-with-ds18b20-build-guide/#software) and fill it in under `platform: dallas`.

``` yaml
deep_sleep:
  run_duration: 60s
  sleep_duration: 5min


# DS18B20 Temperature Sensor
dallas:
  - pin: 4
  
sensor:
  - platform: dht
    pin: 2
    model: dht22
    temperature:
      name: "WiggleBin Temperature"
    humidity:
      name: "WiggleBin Humidity"
  - platform: dallas
    name: "WiggleBin Soil Temperature"
    address: "..." # See for getting right address - https://everythingsmarthome.co.uk/building-a-temperature-sensors-for-home-assistant-wemos-d1-mini-with-ds18b20-build-guide/ 
  - platform: adc
    name: "WiggleBin Soil Moisture"
    pin: A0
```



