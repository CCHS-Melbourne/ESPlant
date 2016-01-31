// Sketch to output sensor values to serial and cycle through the WS2812 leds on the NeoPixels
// Useful to verify operation of all sensors and connected devices
// After uploading to ESPlant, open serial monitor at 115200 to see updating values

#include <FS.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP_Kwai.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define DEBUG
#define ONE_WIRE_BUS 12

ADC_MODE(ADC_VCC);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

Adafruit_BME280 bme;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemp(&oneWire);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 13, NEO_GRB + NEO_KHZ800);

#define MAX_LEDS 12
uint16_t curRingLed = 0 ;

void setup() {

  Serial.begin(115200);
  Serial.println("ESPlant_serial_sensor\n");

  Serial.printf("AP: ESP-%06X\n", ESP.getChipId());

  ESP_Kwai.begin();
  dallasTemp.begin();
  bool bme_res = bme.begin();

  if (!bme_res) {
    Serial.println(F("Uh - oh, could not find a valid BME280 sensor, check i2c address (see comments) and soldering!"));
  }

  if(!accel.begin()){
    Serial.println(F("Uh - oh, could not find a valid ADXL345 sensor, check i2c address (see comments) and soldering!"));
  } else {
    accel.setRange(ADXL345_RANGE_16_G);
  }

  // Turn all NeoPixels off
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();

}

void logger_publish(String Label, String Value) {
  Serial.println(Label + " = " + Value ) ;
}

void logger_publish_c(String Label, String Value) {
  Serial.print(Label + " = " + Value + "  ") ;
}

void loop() {

  Serial.println ( "------------------------" ) ;

  logger_publish("temp", String(bme.readTemperature()));
  logger_publish("pressure", String(bme.readPressure() / 100.0F));
  logger_publish("humidity", String(bme.readHumidity()));

  sensors_event_t event;
  accel.getEvent(&event);

  logger_publish_c("acc/x", String(event.acceleration.x));
  logger_publish_c("acc/y", String(event.acceleration.y));
  logger_publish("acc/z", String(event.acceleration.z));

  kwai_event_t kevent;

  ESP_Kwai.readEvent(&kevent);
  
  logger_publish("adc/uv_sensor", String(kevent.UVSensor));
  logger_publish("adc/soil_1", String(kevent.Soil01));
  logger_publish("adc/soil_2", String(kevent.Soil02));
  logger_publish("adc/input_voltage", String(kevent.InputVoltage));
  logger_publish("adc/internal_temp", String(kevent.InternalTemp));

  dallasTemp.requestTemperatures();
  logger_publish("external/temp_sensor", String(dallasTemp.getTempCByIndex(0)));

  logger_publish("chip/free_heap", String(ESP.getFreeHeap()));
  logger_publish("chip/vcc", String(ESP.getVcc()));

  logger_publish("pir", digitalRead(15) ? "HIGH" : "low" );

  // Turn on next LED in NeoPixel ring
  pixels.setPixelColor(curRingLed, pixels.Color(0,0,0)); // turn off current one
  curRingLed = ( curRingLed + 1 ) % MAX_LEDS ;
  pixels.setPixelColor(curRingLed, pixels.Color(255,255,255));
  pixels.show();

  logger_publish("led", String(curRingLed));
  
  delay(3000);

}
