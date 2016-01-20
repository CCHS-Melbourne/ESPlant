#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP_Onboarding.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <OneWire.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <ESP_MQTTLogger.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define DEBUG

WiFiClient wclient;
ESP_Onboarding server;
ESP_MQTTLogger logger(wclient);
Ticker flipper;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

bool sendMessage = false;

Adafruit_BME280 bme;

void flip(){
  sendMessage = !sendMessage;
}

void setup() {

  Serial.begin(115200);
  Serial.println("");

  server.begin();
  logger.begin();

  Serial.print("Access Token: ");
  Serial.println(server.getToken());

  logger.setToken(server.getToken());

  // do we have config
  if (server.loadWifiCreds()) {
    String ssid = server.getSSID();
    String pass = server.getPassword();

    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // use config to connect
    WiFi.begin(ssid.c_str(), pass.c_str());

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    bool bme_res = bme.begin();

    if (!bme_res) {
      Serial.println(F("Uh - oh, could not find a valid BME280 sensor, check i2c address (see comments) and soldering!"));
    }

    if(!accel.begin()){
      Serial.println(F("Uh - oh, could not find a valid ADXL345 sensor, check i2c address (see comments) and soldering!"));
    } else {
      accel.setRange(ADXL345_RANGE_16_G);
    }

    flipper.attach(15, flip);

    Serial.println("Enabling VSens...");
    pinMode(14, OUTPUT);
    digitalWrite(14, LOW);

    return; // we are done
  }

  Serial.printf("AP: ESP-%06X\n", ESP.getChipId());

  // fall back to AP mode to enable onboarding
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(myIP);

  Serial.println("Loading onboarding server");
  server.startServer();
}

void loop() {
  server.handleClient();
  logger.handleClient();

  if (sendMessage) {

    logger.publish("temp", String(bme.readTemperature()));
    logger.publish("pressure", String(bme.readPressure() / 100.0F));
    logger.publish("humidity", String(bme.readHumidity()));

    Serial.print("e");

    sensors_event_t event;
    accel.getEvent(&event);

    logger.publish("acc/x", String(event.acceleration.x));
    logger.publish("acc/y", String(event.acceleration.y));
    logger.publish("acc/z", String(event.acceleration.z));

    Serial.print("a");

    test_adc();

    Serial.print("o");

    sendMessage = false;
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!logger.connected()) {
      if (logger.connect()){
//        Serial.println("Connected to MQTT server");
      }
    }
  }
}

void test_adc()
{
  for (int channel = 0; channel < 7; channel++)
  {
    Serial.println("***");
    Wire.beginTransmission(0x50);
    Wire.write(channel);
    int res = Wire.endTransmission();
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" register write result ");
    Serial.println(res);
    delay(100);

    if (res)
      continue;

    res = Wire.requestFrom(0x50, 2);
    if (res == 2) {
      uint8_t low = Wire.read();
      uint8_t high = Wire.read();
      Serial.print("Register read count ");
      Serial.print(res);
      Serial.print(" value 0x");
      Serial.print(high, HEX);
      if (low < 0x10)
        Serial.print("0");
      Serial.println(low, HEX);
    }  else {
      Serial.print("Register read count ");
      Serial.print(res);
    }
  }
}
