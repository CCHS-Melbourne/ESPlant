# Firmware

This is a simple firmware which writes data from all the sensors on the ESPLant to to an MQTT server.

# Tools

This uses [ESP8266 Arduino](https://github.com/esp8266/Arduino) environment.

# Libraries

You will need to clone these libraries to your ~/Documents/Arduino/Library (OSX).

* [ESP_Onboarding](https://github.com/wolfeidau/ESP_Onboarding) - ESP8266 on boarding for wifi.
* [ESP_MQTTLogger](https://github.com/wolfeidau/ESP_MQTTLogger) - Generic MQTT logger with configuration rest endpoint.
* [PubSubClient](https://github.com/Imroy/pubsubclient) - MQTT library by @Imroy.
* [Adafruit_BME280_Library](https://github.com/adafruit/Adafruit_BME280_Library) - BME280 library by @adafruit.
* [Adafruit_ADXL345](https://github.com/adafruit/Adafruit_ADXL345) - ADXL345 library by @adafruit.
* [ArduinoJson] (https://github.com/bblanchon/ArduinoJson)- Arduino JSON library
* [Adafruit_Sensor] (https://github.com/adafruit/Adafruit_Sensor) - Adafruit Sensors @adafruit
* [Arduino-Temperature-Control-Library] (https://github.com/milesburton/Arduino-Temperature-Control-Library) Temp-Controll Library
* 
Please note you will need to copy https://github.com/CCHS-Melbourne/ESPlant/tree/master/Libraries/ESP_Kwai To your Arduino/Library directory
