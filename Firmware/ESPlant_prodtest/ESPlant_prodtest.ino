#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
/*
 * "Production" test skethc for ESPlant
 * 
 * Relies on someone manually walking the ESPlant through each test stage.
 * 
 * HOWTO Use:
 * 
 * - Manually inspect the ESPlant and its solder joints.
 * - With the ESPlant disconnected from solar power and USB, place a battery in the holder and check the power switch is "On". The ESP8266 built-in LED should blink at least once (it may come back on, or not).
 * - If you need to double-check the built-in LED, press and release the "ESP Reset" button or turn the power switch to Off and then back to On.
 * - Connect USB to ESPlant. With a battery in the battery holder, cause either the Charging or Charged LED should come on solid. The other LED stays off. Any other LED pattern is a fail.
 * - Remove the battery while keeping USB connected. The green Charged LED should come on solid, while the red Charging LED flickers. Any other LED pattern is a fail.
 * 
 * - Connect all sensors to ESPlant except for (optionally) the UV Sensor (this sensor is hard to test indoors.)
 * - Situate the IR sensor facing a wall or inside a small box, so it can't easily be triggered by motion.
 * - Press Upload to flash this sketch, verify it uploads correctly.
 * - Open Serial Monitor, press and release the "ESP RESET" button on the board.
 * - The LED ring should immediately flash 4 LEDs on very quickly, then off very quickly. If there's no flash, or the LEDs stay on, this is a fail.
 * - The serial monitor output should be "ESPlant production test..." then rows of output as various sensors are tested. The word "FAIL" in the output indicates a problem.
 * - The serial monitor will prompt you to touch the probes of the soil moisture sensor at ADC #3. Low numbers (<100) should be being printed at regular intervals.
 *   Hold onto the probes, and the resistance in your skin simulates wet soil.
 *   The numbers in the output should increase to more than 400 fairly quickly. If the values seem random then this is a fail.
 * - The same process repeats for the soil moisture sensor at ADC #4.
 * - Finally the output will prompt you to "Now Move!". Wave your hand in front of the IR sensor. The device should instantly react to the motion.
 * - When finished, the LED ring should turn green (pass) or red (fail). If the LEDs do not remain on at a solid colour, this is a fail.
 */

OneWire onewire(12); // external one-wire connection for DS18B20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 13, NEO_GRB + NEO_KHZ800);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

Adafruit_BME280 bme;

#define ADC_CHANNEL_UV 0
#define ADC_CHANNEL_SOIL1 3
#define ADC_CHANNEL_SOIL2 4
#define ADC_CHANNEL_VIN 5
#define ADC_CHANNEL_TEMP 6

int16_t read_adc(int channel);

bool fail = false;

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println(F("\r\n\r\n\r\nESPlant production test..."));
  delay(500);

  Serial.println(F("Flashing LEDs..."));
  set_vsens(true);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setPixelColor(0, pixels.Color(255,255,255));
  pixels.setPixelColor(1, pixels.Color(255,0,0));
  pixels.setPixelColor(2, pixels.Color(0,255,0));
  pixels.setPixelColor(3, pixels.Color(0,0,255));
  pixels.show();
  delay(250);
  set_vsens(false);
  Serial.println(F("LEDs powered off."));

  test_bme();
  test_accel();

  set_vsens(true);
  test_ds18b20();
  test_vin();
  //test_uvsensor();
  test_soilsensor(ADC_CHANNEL_SOIL1);
  test_soilsensor(ADC_CHANNEL_SOIL2);
  test_pir();

  Serial.println("Tests done");
  Serial.println("************************************************");
  if(fail)
    Serial.println("**********    FAIL FAIL FAIL FAIL FAIL ********");
  else
    Serial.println("**********            PASS            ********");
  Serial.println("************************************************");

  set_vsens(true);
  for(int i = 0; i < 16; i++) {    
    pixels.setPixelColor(i, fail ? pixels.Color(255,0,0,0) : pixels.Color(0,64,0));
  }
  pixels.show();    
}

void loop() {
}

void set_vsens(bool on)
{
  Serial.print(F("Setting VSens = "));
  Serial.println(on ? F("On") : F("Off"));
  pinMode(14, OUTPUT);
  digitalWrite(14, on ? LOW : HIGH);
  delay(200); /* Let the voltage settle */
}

void test_accel()
{
  if (ADXL345_ADDRESS != 0x53) {
    Serial.println(F("FAIL - Need to change ADXL345_ADDRESS back to default, see comment!"));
    fail = true;
    return;
  }

  if (!accel.begin())
  {
    Serial.println(F("FAIL - Accelerometer not responding on i2c"));
    fail = true;
    return;
  }
  accel.setRange(ADXL345_RANGE_2_G);

  /* Accelerometer */
  sensors_event_t event;
  accel.getEvent(&event);

  if (event.acceleration.z > 8 && event.acceleration.y < 1 && event.acceleration.x < 1) {
    Serial.println("Accelerometer readings OK");
    return;
  }
  Serial.println("FAIL accelerometer out of spec:");
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m / s ^ 2 ");
  fail = true;
}

void test_bme()
{
  if (!bme.begin()) {
    Serial.println(F("FAIL - BME280 not responding on i2c"));
    fail = true;
    return;
  }

  float t = bme.readTemperature();
  float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

  if (t > 10 && t < 38 && alt > 10 && alt < 100) {
    Serial.println("BME280 readings OK");
    return;
  }

  fail = true;
  Serial.println("FAIL - BME280 readings inaccurate:");
  Serial.print(F("Temperature = "));
  Serial.print(t);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(F(" hPa"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(alt);
  Serial.println(F(" m"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.readHumidity());
  Serial.println(F(" % "));
}

void test_ds18b20()
{
  byte data[120];
  byte addr[80];
  int i;

  onewire.reset_search();
  onewire.search(addr);

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("FAIL - DS18B20 CRC is not valid!");
    fail = true;
    return;
  }

  if (addr[0] != 0x28) {
    Serial.println("FAIL - got wrong ID byte for DS18B20");
    fail = true;
    return;
  }

  onewire.reset();
  Serial.println("DS18B20 reading OK");
  delay(50);
}

void test_uvsensor()
{
  int uv = read_adc(ADC_CHANNEL_UV);

  if (uv == -1) {
    Serial.println("FAIL - UV sensor i2c read fails");
    fail = true;
    return;
  }

  Serial.println("UV sensor - put bright light on sensor...");
  delay(500);
  while (uv < 10) {
    Serial.print(uv);
    Serial.print(".. ");
    delay(200);
    uv = read_adc(ADC_CHANNEL_UV);
  }
  Serial.print("UV uncovered reading ");
  Serial.println(uv);

Serial.println("Depowering UV...");
  set_vsens(false);
  while (uv > 2) {
    Serial.print(uv);
    Serial.print(".. ");
    delay(200);
    uv = read_adc(ADC_CHANNEL_UV);
  }
  set_vsens(true);
  Serial.println("UV repowered.");  

  Serial.println("UV sensor - remove bright light on sensor");
  while (uv > 5)  {
    Serial.print(uv);
    Serial.print(".. ");
    delay(200);
    uv = read_adc(ADC_CHANNEL_UV);
  }
  Serial.print("UV covered reading ");
  Serial.println(uv);

}

void test_soilsensor(int channel)
{
  Serial.print("Testing soil sensor on ADC channel ");
  Serial.println(channel);

  int soil = read_adc(channel);
  if (soil == -1) {
    Serial.println("FAIL - Couldn't read ADC soil channel");
    fail = true;
    return;
  }

  Serial.println("Please release soil sensor");
  Serial.println("Waiting for soil sensor to read 'uncontacted'...");
  while (soil > 100) {
    Serial.print(soil);
    Serial.print(".. ");
    delay(200);
    soil = read_adc(channel);
  }

  Serial.print("Please hold the contacts of the soil sensor connected to ADC #");
  Serial.println(channel);

  soil = read_adc(channel);
  while (soil < 400) {
    Serial.print(soil);
    Serial.print(".. ");
    delay(200);
    soil = read_adc(channel);
  }
/*
  set_vsens(false);
  Serial.println(F("Waiting for ADC to depower..."));
  soil = read_adc(channel);
  while (soil > 10) {
    Serial.print(soil);
    Serial.print(".. ");
    delay(200);
    soil = read_adc(channel);
  }
  set_vsens(true);
*/

  Serial.print(F("Soil sensor ADC channel "));
  Serial.print(channel);
  Serial.println(" tested OK");
}

void test_vin()
{
  int32_t vin = read_adc(ADC_CHANNEL_VIN);

  if(vin == -1) {
    Serial.println("FAIL - Couldn't read i2c VIN channel");
    fail = true;
    return;
  }

  /* Approximate conversion from raw reading to millivolts */
  vin = vin * 1915 / 1000;

  Serial.print("VIN reading ");
  Serial.print(vin);
  Serial.println("mV");

  if(vin < 4000 || vin > 5100) {
    Serial.println("FAIL - VIN reading out of expected range for USB powered board");
    fail = true;    
  }
}

void test_pir()
{
  Serial.println("Hold still...");
  while(digitalRead(15) == HIGH) {
    delay(200);  
  }
  Serial.println("Now move!");
  while(digitalRead(15) == LOW) {
    delay(200);
  }
  Serial.println("PIR test OK");
}

int16_t read_adc(int channel)
{
  Wire.beginTransmission(0x50);
  Wire.write(channel);
  int res = Wire.endTransmission();

  if (res != 0 ) {
    Serial.print("Failed ADC i2c write channel number 5 = ");
    Serial.println(res);
    fail = true;
    return -1;
  }

  res = Wire.requestFrom(0x50, 2);
  if (res != 2) {
    Serial.print("Failed ADC i2c readback = ");
    Serial.println(res);
    fail = true;
    return -1;
  }

  uint8_t low = Wire.read();
  uint8_t high = Wire.read();
  return low + ((int)high << 8);
}
