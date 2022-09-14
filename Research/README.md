# Wiggle Bin Research

This document describes the research and prototype process for developing a Smart Worm Bin during the [Open Hardware Academy](https://www.openhardware.academy/01_Welcome.html).

The Smart Worm Bin project is about developing a [vermicomposting](https://en.wikipedia.org/wiki/Vermicompost) bin which makes both **worms** and **humans** happy.

The project is divided into research for worms and humans.

The planning is divided in the following phases:
1. Worm research
2. Worm experiments
3. Worm prototypes
4. Human prototypes
5. Combined prototype


## Ideas

- Instead of a Leave Sensor, build a Worm Sensor. Sensing similar to a worm and for condtions like temperature. https://www.seeedstudio.com/leaf-wetness-sensor-p-b-4861.html?queryID=e2de2cc839ede3a06e05ddef412d6b94&objectID=4862&indexName=bazaar_retailer_products

## Worms
  
**Goal**
Happy and productive worms
  
**Research question**
How can we measure worm happiness?
A. Worm population
B. Worms not fleeing
C. Enviromental conditions
  
**Control:**
- Soil wetness (wet but not to wet)
- Temperature (13C - 25C)
- Oxygen
- Food

**Possible problems:**
- Overheating
- Drowning
- Too wet
- Too dry
- Lack of oxygen
- Over or under feeding
- Feeding the wrong food

### Measure

#### A. How many worms are present (approximately) in the soil?

The following experiments will be done while also tracking the worm count manually.

Sensor A: Camera on top of the bin
    - Infrared imaging
    - [thermal camera](https://www.hackster.io/naveenbskumar/intruder-detection-using-ultra-low-powered-thermal-vision-24ed45)
Sensor B: Movement in the soil
    - [accelerometer](https://www.youtube.com/watch?v=FElVgfi-zuY)
    - [piezoelectric ribbon sensor](https://www.adafruit.com/product/4931)
    - [piezo element](https://www.adafruit.com/product/1739)
    - [piezo trilling sensor](https://www.conrad.nl/nl/p/seeed-studio-101020031-pi-zo-trillingssensor-1-stuk-s-1369532.html)
    - [microwave radar](https://www.otronic.nl/a-59664644/bewegingssensors/bewegingssensor-met-microgolf-radar-rcwl-051/?gclid=EAIaIQobChMIoL3F7fC--QIVE_hRCh2xvQ3PEAQYCyABEgI77_D_BwE) + [parallax version](https://www.digikey.nl/nl/products/detail/parallax-inc./32213/3523698?utm_adgroup=Evaluation%20Boards%20-%20Sensors&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Development%20Boards%2C%20Kits%2C%20Programmers&utm_term=&productid=3523698&gclid=EAIaIQobChMIoL3F7fC--QIVE_hRCh2xvQ3PEAQYDyABEgJvnfD_BwE)
    - [root scanning](https://plantmethods.biomedcentral.com/articles/10.1186/s13007-017-0160-z)
    - [capacative touch sensing](https://www.tinytronics.nl/shop/en/sensors/touch)
    - ground-penetrating radar
Sensor C: Touch sensor (with worm feed)
Sensor D: Gate sensor (between buckets)
Sensor E: Gasses being created from decomposition 
    - [air quality](https://www.mikroe.com/air-quality-click)
Sensor F: Soil shrinkage
Sensor G: ...


#### B. Are worms fleeing a bad condition?

When conditions get bad, worms flee either to the top or bottom. Worms fleeing a bad condition is probably easier to sense then worm population count (see question A.). 

Sensor A: Safe hiding place sensor
Sensor B: Camera on top or bottom of the bin
Sensor C: Tracking conditions such as temperature
Sensor D: ...

#### C. Are the conditions okay for worms?

This is probably the easiest to measure and likely to be applied to the Smart Worm Bin for home usage. However the questions above are more interesting research questions.

Sensor A: Moist
- [microwave](https://www.skahasensing.ca/post/sensing-soil-moisture-with-microwaves-how-does-it-work)
Sensor B: Oxygen
Sensor C: Temperature

### Research

How do worms find each other?
> The worms are thought to find one another through the release of pheromones. Once a partner is found, the two worms line up in opposite directions so that each worm's male opening lines up with the other worm's sperm receptacle, known as the spermatheca.
https://sciencing.com/how-do-earthworms-reproduce-13406886.html

Do worms communicate?
> Researchers have found that they can communicate with each other using touch and taste. They can feel vibrations in the soil so they can avoid predators. There's even evidence to suggest that they like to travel in herds and follow a leader.
https://www.abc.net.au/gardening/factsheets/earthworm-communication/12194098

How do worms move?
https://unclejimswormfarm.com/composting-worms-and-how-they-move/
https://www.nih.gov/news-events/nih-research-matters/magnetic-field-sensor-unearthed-worms

Papers:
- https://www.researchgate.net/figure/Intra-and-interworm-measurements-of-LGF-and-MGF-conduction-velocities-The-two-right-box_fig4_260446606
- https://www.biorxiv.org/content/10.1101/352948v1.full
- https://www.tudelft.nl/agtech/projects/radar-for-soil-moisture
- https://www.sciencedirect.com/science/article/pii/S0003267015001804
- https://www.nih.gov/news-events/nih-research-matters/magnetic-field-sensor-unearthed-worms

### Existing Worm Tracking Systems

https://www.mbfbioscience.com/products/wormlab
https://www.youtube.com/watch?v=_GjknIvk9xs

### Sensors

#### Microwave radar

Tinytronics
https://www.tinytronics.nl/shop/en/sensors/motion/dfrobot-gravity-digital-microwave-sensor-radar-motion-sensor

Andreass Spies
https://www.youtube.com/watch?v=9WiJJgIi3W0
https://www.youtube.com/watch?v=PNbAM9IhfBE

Usage in soil
https://physics.aps.org/story/v10/st17

Papers
https://mdpi-res.com/d_attachment/applsci/applsci-12-05471/article_deploy/applsci-12-05471-v2.pdf?version=1653966187

#### Ground Penetrating Radar

https://hackaday.com/2019/02/07/ground-penetrating-radar-for-the-masses/
https://hackaday.io/project/175115-gprino 
https://hforsten.com/third-version-of-homemade-6-ghz-fmcw-radar.html http://docs.gprmax.com/en/latest/examples_simple_2D.html#a-scan-from-a-metal-cylinder (simulation)

#### Acoustic Emission

https://www.nature.com/articles/s41598-018-28582-9

#### Capacitive Soil Sensor

https://makersportal.com/blog/2020/5/26/capacitive-soil-moisture-calibration-with-arduino


## Humans

**Goal**
Effortless vermicomposting without dirty hands
  
**Research question**
How can we make vermicomposting effortless?

**Audience**

In general this project focuses on home users with a single worm bin for the family. However some outcome might also be useful for bio-researchers and vermicompost/worm farms.

- Researchers
    - documentation on experiments
    - sensors
- Vermicompost and worm farms
    - sensors
- Worm bins at home 
    - complete worm bin which keeps conditions correct for worms
    - application to check worm conditions

**Possible problems:**
- Flies
- Smell
- Dirty hands
- Harvest times
- Feeding times
- Not knowing if worms are okay

#### How full is the bin?

Ultrasonic:
https://wiki.dfrobot.com/_A02YYUW_Waterproof_Ultrasonic_Sensor_SKU_SEN0311
https://www.tinytronics.nl/shop/en/sensors/distance/waterproof-ultrasonic-sensor-jsn-sr04t
Pulsed radar: 
https://www.sparkfun.com/products/16826

#### How can I keept the bin at the right temperature?

PTC Heating element: 
https://www.dfrobot.com/product-2415.html
Peltier Element: 
https://www.tinytronics.nl/shop/nl/mechanica-en-actuatoren/onderdelen/koelelementen/peltier-element-60w-tec1-12706
Temperature setting boards
https://www.tinytronics.nl/shop/nl/sensoren/temperatuur/digitale-thermostaat-module-met-relais-xh-w1209
https://www.tinytronics.nl/shop/nl/sensoren/temperatuur/digitale-thermostaat-module-met-relais-12v-xh-w3001

*Measuring temperature*

Non-contact Infared:
https://www.dfrobot.com/product-1823.html
https://www.dfrobot.com/product-1824.html

Contact temperature sensor:
https://www.tinytronics.nl/shop/nl/sensoren/temperatuur/ds18b20-to-92-thermometer-temperatuur-sensor-met-kabel-waterproof-hoge-temperatuur-1m
https://www.tinytronics.nl/shop/nl/platformen-en-systemen/sonoff/accessoires/sonoff-ds18b20-water dichte-temperatuursensor-voor-de-th10th16

Temperature AND humidity
https://www.dfrobot.com/product-1636.html
https://www.dfrobot.com/product-2600.html
https://www.tinytronics.nl/shop/nl/sensoren/lucht/vochtigheid/dfrobot-sht31-weerbestendige-temperatuur-en-luchtvochtigheidssensor
https://www.dfrobot.com/product-2279.html

## Batteries

https://www.youtube.com/watch?v=FrCgQgahzsI
https://myesp8266.blogspot.com/2017/07/battery-powered-wemos-d1-with-wemoss.html
https://arduinodiy.wordpress.com/2016/12/25/monitoring-lipo-battery-voltage-with-wemos-d1-minibattery-shield-and-thingspeak/
https://emariete.com/en/co2-meter-with-battery-well-done/
https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/

*Re-use phone battery*
https://www.kickstarter.com/projects/betterre/better-re-upcycling-power-pack-reusing-smartphone

Power bank:
https://www.mischianti.org/2019/01/24/emergency-power-bank-homemade/

*Temperature*
https://www.youtube.com/watch?v=2wbWN-0qttY

## WebThings

Reading the database
https://discourse.mozilla.org/t/access-to-database/72107x

https://webthings.io/framework/

## Camera

On worms and reaction to light. Red makes them happy, IR even happier.
https://redlightman.com/blog/red-light-doubles-vermicompost-rate/. 
https://www.researchgate.net/figure/Planarian-Photophobic-Responses-Vary-by-Wavelength-A-Images-of-the-photophobia-assay_fig1_269415991

A. Use RED led panel /  strip or IR LEDs
B. Could use a red filter or IR transmitting sheet and take advantage of sunlight.
https://www.eplastics.com/plexiglass/acrylic-sheets/ir-transmitting
https://www.ebay.com/itm/231787983493

Camera options:

**Night vision / infrared camera:**

Pros and cons:
- Cheaper then thermal camera
- Runs on low voltage
- Worms like the IR light

https://www.youtube.com/watch?v=sttQAL4r4TQ

**Thermal camera:**

Pros and cons:
- Provides indication of temperatureon soil level
- More expensive

**Normal camera with red light:**

Pros and cons:
- Allows for a peep hole for human to look into
- Worms like red light, a bit less then IR light
- Can be used with standard phone camera as well

5v led strip:
NeoPixel needs capacitator
https://www.otronic.nl/a-64167598/leds/24-bit-rgb-leds-ws2812b-cirkel-rond-neopixel/
https://www.otronic.nl/a-71908879/leds/5mm-super-flux-piranha-4pin-led-rood/
https://www.otronic.nl/a-62484269/leds/8-bit-rgb-leds-ws2812b-op-printplaatje/
https://www.otronic.nl/a-64167231/leds/12-bit-rgb-leds-ws2812b-cirkel-rond-neopixel/
https://www.tinytronics.nl/shop/en/lighting/led-strips/led-strips/ws2812b-digital-5050-rgb-led-strip-30-leds-1m
https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels
https://www.otronic.nl/a-64167231/leds/12-bit-rgb-leds-ws2812b-cirkel-rond-neopixel/
https://elektronicavoorjou.nl/en/product/5v-cob-led-strip-rood/
https://nl.farnell.com/seeed-studio/104020108/rgb-led-strip-30-m-seeedunio-arduino/dp/4007787?st=LED-strip%20rood%205v
https://nl.aliexpress.com/item/4000299931029.html?spm=a2g0o.ppclist.product.2.6b7dFD0fFD0fQs&pdp_npi=2%40dis%21EUR%21%E2%82%AC%200%2C87%21%E2%82%AC%200%2C74%21%21%21%21%21%40211b5e2516626716687135845e7904%2110000001240410826%21btf&_t=pvid%3Ab766f141-e50f-44e7-8069-83ef8838bc86&afTraceInfo=4000299931029__pc__pcBridgePPC__xxxxxx__1662671668&gatewayAdapt=glo2nld


Powering LEDs on battery:
https://www.youtube.com/watch?app=desktop&v=ycFTxCoNAQk

RED Led panel with MAX7219 chip:
https://www.best-microcontroller-projects.com/max7219.html
https://www.instructables.com/Arduino-8x8-LED-Matrix/
only one column at a time is on, so not great for our purpose

Could try out the WLG M20088A/BEG panel I already have:
https://arduino.stackexchange.com/questions/21237/how-to-use-a-bi-color-matrix-led-8x8-with-24-pins
Tutorial on ESP8266 connected to led matrix https://microcontrollerslab.com/max7219-led-dot-matrix-display-esp8266-nodemcu-tutorial/

Blue filter - not for worms, but plants
https://www.raspberrypi.com/news/whats-that-blue-thing-doing-here/

**Articles**

*Option to add camera with night vision. Also includes battery connector. Includes many options, is 20$.*
https://www.google.com/amp/s/www.arducam.com/ov2640/%3famp

*ESP32 with self-made night camera. Version runs on 12v because of IR led ring.*
https://hackermagnet.com/night-vision-on-esp32-cam/ 

## Soil Moisture Sensor

Casing
https://www.thingiverse.com/thing:2952434/comments

Fix:
https://www.youtube.com/watch?v=QGCrtXf8YSs

Waterproofing
https://www.ourpcb.com/waterproof-pcb.html
https://www.youtube.com/watch?v=h8ycmroFQSs
https://www.youtube.com/watch?v=1ZCn0xkkQSk

Nail polish:
https://orlybeauty.com/blogs/news/what-is-gel-nail-polish-made-of

Camera + AI
https://www.photonics.com/Articles/Standard_Digital_Camera_and_AI_Pair_for_Soil/a66799
https://munsell.com/about-munsell-color/how-color-notation-works/how-to-read-color-chart/

## WemosD1 mini deep sleep

Resistor or schottky diode BAT43 between D0 and RST
https://github.com/universam1/iSpindel/issues/59
https://www.esp8266.com/viewtopic.php?f=13&t=7918&start=24#sthash.9AUQQ49G.dpuf