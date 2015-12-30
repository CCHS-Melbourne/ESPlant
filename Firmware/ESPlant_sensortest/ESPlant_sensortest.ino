#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>

#define SEALEVELPRESSURE_HPA (1013.25)

/* Set to turn on some hacks for the V1.0 prototype, that can be removed later */
#define PROTO_ESPLANT 1

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
}

void loop() {
  /* Accelerometer */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m / s ^ 2 ");

  /* BME280 */
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
  delay(1000);

}
