#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
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
#include <DallasTemperature.h>

#include <Ticker.h>
#include <PubSubClient.h>
#include <ESP_MQTTLogger.h>
#include <ESP_Kwai.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define ONE_WIRE_BUS 12

ADC_MODE(ADC_VCC);

ESP8266WebServer webserver(9000);

ESP_Onboarding server(&webserver);

// Send data over MQTT with TLS
// WiFiClientSecure wclient;
// ESP_MQTTLogger logger(wclient, &webserver, 8883);

// Send data over MQTT without TLS
WiFiClient wclient;
ESP_MQTTLogger logger(wclient, &webserver, 1883);

ESP_Kwai kwai;

Ticker flipper;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

bool sendMessage = false;

Adafruit_BME280 bme;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemp(&oneWire);

void flip() {
  sendMessage = !sendMessage;
}

void setup() {

  Serial.begin(115200);
  Serial.println("");

  server.begin();
  logger.begin();

  kwai.begin(14);
  dallasTemp.begin();

  Serial.print("Access Token: ");
  Serial.println(server.getToken());

  logger.setToken(server.getToken());

  bool configured = server.loadWifiCreds();

  Serial.println("Loading onboarding server");
  server.startServer(configured);

  // do we have config
  if (configured) {

    // configure station mode
    WiFi.mode(WIFI_STA);

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

    if (!accel.begin()) {
      Serial.println(F("Uh - oh, could not find a valid ADXL345 sensor, check i2c address (see comments) and soldering!"));
    } else {
      accel.setRange(ADXL345_RANGE_16_G);
    }

    flipper.attach(15, flip);

    return; // we are done
  }

  Serial.printf("AP: ESP_%06X\n", ESP.getChipId());

  // fall back to AP mode to enable onboarding
  WiFi.mode(WIFI_AP);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(myIP);

}

void loop() {
  server.handleClient();
  logger.handleClient();

  if (sendMessage) {

    logger.publish("temp", String(bme.readTemperature()));
    logger.publish("pressure", String(bme.readPressure() / 100.0F));
    logger.publish("humidity", String(bme.readHumidity()));

    sensors_event_t event;
    accel.getEvent(&event);

    logger.publish("acc/x", String(event.acceleration.x));
    logger.publish("acc/y", String(event.acceleration.y));
    logger.publish("acc/z", String(event.acceleration.z));

    kwai_event_t kevent;

    kwai.readEvent(&kevent);

    logger.publish("adc/uv_sensor", String(kevent.UVSensor));
    logger.publish("adc/soil_1", String(kevent.Soil01));
    logger.publish("adc/soil_2", String(kevent.Soil02));
    logger.publish("adc/battery_voltage", String(kevent.BatteryVoltage));
    logger.publish("adc/internal_temp", String(kevent.InternalTemp));

    dallasTemp.requestTemperatures();

    logger.publish("external/temp_sensor", String(dallasTemp.getTempCByIndex(0)));

    logger.publish("chip/free_heap", String(ESP.getFreeHeap()));
    logger.publish("chip/vcc", String(ESP.getVcc()));

    sendMessage = false;
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!logger.connected()) {
      logger.connect();
    }
  }
}
