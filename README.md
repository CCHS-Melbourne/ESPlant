ESPlant - Environment Sensor Plant
=============

Welcome to the linux.conf.au Open Hardware Project.  You can see information about this and other MiniConfs at http://www.openhardwareconf.org/wiki/Main_Page

The ESPlant kit is designed to provide an open source platform to transmit environmental data to a central source using the power of wifi.

![Board Layout](Photos/ESPlant.front.png?raw=true "Front")
![Board Layout](Photos/ESPlant.back.png?raw=true "Back")

Prototype board in action (Significant changes to the controller board since then)!

![Prototype](Photos/ESPlant.prototype.jpg?raw=true "Prototype")

You can check out photos of our build day in the Photos\Build directory of this repo.

Check out a video of pick-n-placing components of the boards you'll be getting at LCA 2016.
https://www.youtube.com/watch?v=282qku2C4xo

FEATURES
--------

* ESP8266 WiFi enabled microcontroller. You can program it via the Arduino IDE, or alternatively Espressif SDK or esp-open-rtos.
* Secondary microcontroller (STM32F042) acts as USB/Serial interface and i2c Analog to Digital Converter
* Battery holder for 16340 sized 3.7V lithium cell
* Lithium battery charger interface (solar input friendly)
* Board automatically switches power source between solar input, battery, and USB power as applicable.

## Onboard Sensors (on I2C bus)

* BME280 Temperature/Humidity/Barometric Pressure sensor
* ADXL345 accelerometer

## External Sensors (all optional, connected to screw terminals)

* 2x soil moisture sensors (via ADC pins)
* DS18B20 connected to GPIO 12.
* PIR (infrared motion) sensor connected to GPIO 15.
* WS2812B LED strip connected to GPIO 13 (mislabelled as 15 on V1.3 PCB!)

The 3.3V "VSens" power rail is switchable on/off by the microcontroller. It defaults to unpowered (off). The GPIO to use is 14, and it is active low (ie driving the output low turns VSens on, driving it high turns it off).


Using with Arduino
------------------

To use the ESPlant with Arduino:

* No drivers required on Linux, OS X, or Windows 10.
* No manual reset or button pressing dance required to program.
* In [ESP8266 Arduino IDE](https://github.com/esp8266/arduino), under Tools menu, set Board to "NodeMCU V1.0" (fully compatible).
* Can set upload speed to 230400. 460800 works in most cases (you might get occasionl failures at 460800)


CREDITS
------------

This project was designed by the 2016 linux.conf.au Open Hardware Team!
 - John Spencer
 - Angus Gratton
 - Andy Gelme
 - Jon Oxer

You can see our githubs at:
 - https://github.com/mage0r
 - https://github.com/projectgus
 - https://github.com/geekscape
 - https://github.com/jonoxer

Made at hackmelbourne.org!

SPECIAL THANKS
------------

To the HackMelbourne (CCHS, http://hackmelbourne.org) community of Melbourne, Australia.

To all other open-source developers whose countless hours supported every other aspect of this design.

DISTRIBUTION
------------
The specific terms of distribution of this project are governed by the
license referenced below. Please contact the copyright owner if you wish to modify the board for distribution. Please utilize this design for personal or research projects. Please acknowledge all contributors.

LICENSE
-------
Licensed under the TAPR Open Hardware License (www.tapr.org/OHL).
The "license" sub-folder also contains a copy of this license in plain text format.

Copyright John Spencer, Angus Gratton, Andy Gelme, Jon Oxer 2015
