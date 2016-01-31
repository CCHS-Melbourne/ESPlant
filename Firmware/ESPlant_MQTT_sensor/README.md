# Firmware

This is a simple firmware which writes data from all the sensors on the ESPLant to to an MQTT server.

# Building

You will need Arduino 1.6.6 or newer and the ESP8266 "staging" release.

* Use File->Open and open the Firmware/ESPlant_serial_sensor from this repository. This is a simple sketch that outputs all the analog values read by the board to the serial monitor.
* Press the Upload button (looks like "Play")

# Libraries

The project uses the following libraries. The [main README](https://github.com/CCHS-Melbourne/ESPlant#libraries-you-will-need) has instructions for automatically installing the libraries into the Arduino libraries directory in your home directory.

* [ESP_Onboarding](https://github.com/wolfeidau/ESP_Onboarding) - ESP8266 on boarding for wifi.
* [ESP_MQTTLogger](https://github.com/wolfeidau/ESP_MQTTLogger) - Generic MQTT logger with configuration rest endpoint.
* [PubSubClient](https://github.com/Imroy/pubsubclient) - MQTT library by @Imroy.
* [Adafruit_BME280_Library](https://github.com/adafruit/Adafruit_BME280_Library) - BME280 library by @adafruit.
* [Adafruit_ADXL345](https://github.com/adafruit/Adafruit_ADXL345) - ADXL345 library by @adafruit.
* [Adafruit_Neopixel](https://github.com/adafruit/Adafruit_NeoPixel) - WS2812b
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor) - Adafruit Unified Sensor Driver
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* [OneWire](https://github.com/PaulStoffregen/OneWire)

# Usage

## Onboarding to wifi

* Connect to the ESPlant using USB
* Open the Arduino Serial Monitor
* Copy the Access Token and note the name of your AP!
* Connect to your AP
* Connect the device to wifi using the following command
```
curl -v -H "Authorization: Token access_token_here" 'http://192.168.4.1:9000/wifiSetup?ssid=xxx&pass=xxx'
```

## Logging data to an MQTT server

* Install mosquitto (MQTT client and server)
* Find out your IP and configure the ESPlant to send messages to your system using the following command.
    * The IP address of your ESPlant is printed to the console once it reboots and connects to wifi. This goes in `x.x.x.x`.
    * Also you need your IP address of course which goes into `y.y.y.y`.
```
curl -v -H "Authorization: Token access_token_here" 'http://X.X.X.X:9000/mqttSetup?mqtt_url=mqtt://test@y.y.y.y'
```
* Reset your ESPlant.
* To subscribe to messages
```
mosquitto_sub -v -t '#'
```
