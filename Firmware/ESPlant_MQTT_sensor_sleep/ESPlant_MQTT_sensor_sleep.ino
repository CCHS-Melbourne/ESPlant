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

/* How many minutes should we deep sleep for (can use fractional numbers) */
#define SLEEP_MINUTES 1

ESP8266WebServer webserver(9000);

ESP_Onboarding server(&webserver);

// Send data over MQTT with TLS
// WiFiClientSecure wclient;
// ESP_MQTTLogger logger(wclient, &webserver, 8883);

// Send data over MQTT without TLS
WiFiClient wclient;
ESP_MQTTLogger logger(wclient, &webserver, 1883);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

Adafruit_BME280 bme;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemp(&oneWire);

void setup() {

  Serial.begin(115200);
  Serial.println("");

  server.begin();
  logger.begin();

  ESP_Kwai.begin(14);
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

    return; // we are done
  }

  Serial.printf("AP: ESP_%06X\n", ESP.getChipId());

  // fall back to AP mode to enable onboarding
  WiFi.mode(WIFI_AP);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(myIP);

}

template <typename T>
void publish(const char * topic, T msg)
{
  logger.publish(topic, String(msg));
}

void loop()
{
  // WiFi connected?
  if (WiFi.status() != WL_CONNECTED) {
    // out-of-loop magic will conect us
    return;
  }

  // MQTT logger connected?
  if (!logger.connected()) {
    Serial.println("Connecting Logger");
    logger.connect();
    return;
  }

  static unsigned N = 0;
  server.handleClient();
  logger.handleClient();

  Serial.print  ("Publishing ");
  Serial.print  (N);
  Serial.println(" ...");

  publish("N",      N++);
  publish("millis", millis());

  publish("temp",     bme.readTemperature());
  publish("pressure", bme.readPressure() / 100.0F);
  publish("humidity", bme.readHumidity());

  sensors_event_t event;
  accel.getEvent(&event);

  publish("acc/x", event.acceleration.x);
  publish("acc/y", event.acceleration.y);
  publish("acc/z", event.acceleration.z);

  kwai_event_t kevent;

  ESP_Kwai.readEvent(&kevent);

  publish("adc/uv_sensor",     kevent.UVSensor);
  publish("adc/soil_1",        kevent.Soil01);
  publish("adc/soil_2",        kevent.Soil02);
  publish("adc/input_voltage", kevent.InputVoltage);
  publish("adc/internal_temp", kevent.InternalTemp);

  dallasTemp.requestTemperatures();

  publish("external/temp_sensor", dallasTemp.getTempCByIndex(0));

  publish("chip/free_heap",       ESP.getFreeHeap());
  publish("chip/vcc",             ESP.getVcc());


  delay(3000);

  Serial.println("hibernating..."); 
  ESP.deepSleep(SLEEP_MINUTES * 60 * 1000l * 1000l);

}
