#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <OneWire.h>

OneWire onewire(12); // external one-wire connection

#define SEALEVELPRESSURE_HPA (1013.25)

/* Set to turn on some hacks for the V1.0 prototype, that can be removed later */
#define PROTO_ESPLANT 0

/* TIPS

   To automatically upload without needing to juggle buttons, set the board type in Arduino IDE to "NodeMCU 1.0 (ESP-12E Module)"
   Upload baud rate 230400 should work
*/

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


Adafruit_BME280 bme;

void setup() {
#if PROTO_ESPLANT
  /* The Adafruit ADXL345 library does not allow setting the i2c address
     from the sketch.

     For Prototype ESPlant V1.0, edit the Adafruit_ADXL345_U.h header and change the line:

     #define ADXL345_ADDRESS                 (0x53)

     to

     #define ADXL345_ADDRESS                 (0x1D)

     For production ESPlant V1.2, the hardware address was changed to 0x53 so no modification is necessary.
  */
  if (ADXL345_ADDRESS != 0x1D) {
    Serial.println(F("WARNING: Need to change the ADXL345_ADDRESS in Adafruit_ADXL345_U.h, see comment"));
  }
#else
  if (ADXL345_ADDRESS != 0x53) {
    Serial.println(F("Need to change ADXL345_ADDRESS back to default, see comment!"));
  }
#endif

#if PROTO_ESPLANT
  Wire.begin(5, 4); /* SDA, SCL swapped from the Arduino ESP8266 defaults on  V1.0 proto */
#else
  Wire.begin();
#endif

  Serial.begin(115200);
  Serial.println(F("ESPlant sensor test..."));
  bool fail = false;

  bool bme_res;

#if PROTO_ESPLANT
  /* Also important: For ESPlant V1.0, i2c address is 0x76 so needs to be passed to constructor.

     For production ESPlant V1.2, address was changed to 0x77 same as Adafruit library default - so can use default begin()
  */
  bme_res = bme.begin(0x76);
#else
  bme_res = bme.begin();
#endif

  if (!bme_res) {
    Serial.println(F("Uh - oh, could not find a valid BME280 sensor, check i2c address (see comments) and soldering!"));
    fail = true;
  }

  if (!accel.begin())
  {
    Serial.println(F("Uh - oh, could not find a valid ADXL345 accel... Check i2c address (see comments) and soldering!"));
    fail = true;
  }
  //while(fail) { }

  accel.setRange(ADXL345_RANGE_2_G);

  Serial.println("Enabling VSens...");
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);

}

void loop() {
  test_ds18b20();

  /* Accelerometer */
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.println("Accelerometer");
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m / s ^ 2 ");

  /* BME280 */
  Serial.println("BME280:");
  Serial.print(F("Temperature = "));
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(F(" hPa"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.readHumidity());
  Serial.println(F(" % "));

  Serial.println();

   test_adc();

  delay(5000);

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

void test_ds18b20()
{
  byte data[120];
  byte addr[80];
  int i;

  Serial.println("Testing DS18B20...");
  onewire.reset_search();
  onewire.search(addr);

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("OneWire CRC is not valid!");
    return;
  }

  Serial.print("ROM =");
  for ( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  Serial.println();

  if (addr[0] != 0x28) {
    Serial.println("Got wrong ID byte for DS18B20");
    return;
  }

  onewire.reset();
  onewire.select(addr);
  onewire.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);

  onewire.reset();
  onewire.select(addr);
  onewire.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = onewire.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  float celsius = raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.println(celsius);
}


