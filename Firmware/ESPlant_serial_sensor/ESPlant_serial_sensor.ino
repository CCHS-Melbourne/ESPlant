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
// We have an interrupt every 10ms and RGB_COLOR_CHANGE_STEPS changes to get to the
// next color, so that's 1 second with a delay factor of 1.
uint8_t RGB_delay_factor = 0;
// How many intermediate shades of colors.
#define RGB_COLOR_CHANGE_STEPS 100
// How many steps we hold the target colors.
#define RGB_COLOR_HOLD_STEPS 20
// How bright to light up neopixel. 1 very bright (blinding), bigger is less bright
#define RGB_LED_BRIGHTNESS 3
// How many trailing LEDs get displayed on neopixel (1 to 10)
#define LED_TRAIL 8

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

// FIXME: the returned values do not need to be float, but the variables I use
// are float for smooth transitions, so it carries on to this function for now.
void LED_Color_Picker(float *blue, float *red, float *green) {
    uint8_t led_color[3];
    float led[3] = { -1, -1, -1 };

    // Keep the first LED bright.
    led_color[0] = random(0, 64);
    // The lower the previous LED is, the brigher that color was
    // and the darker we try to make the second one.
    led_color[1] = constrain(random(0, 255 + 512 - led_color[0]), 0, 255);
    // Don't make the last LED too bright if the others already are (we
    // don't want white) or too dark if the others are too dark already.
    led_color[2] = constrain(
	random( constrain( (64-(led_color[0] + led_color[1])/2), 0, 255), 
		255 + 768 - (led_color[0] + led_color[1])), 
	0,
	255 );

    // Since color randomization is weighed towards the first color
    // we randomize color attribution between the 3 colors.
    uint8_t i = 0;
    while(i  < 3)
    {
	uint8_t color_guess = random(0, 3);
	// Loop until we get a color slot that's been unused.
	if (led[color_guess] != -1.0) continue;
	led[color_guess] = led_color[i++];
    }

    // Fix things after the fact for RGB LEDs are common cathode (bright = 255)
    *blue =  (255 - led[0]) / RGB_LED_BRIGHTNESS;
    *red =   (255 - led[1]) / RGB_LED_BRIGHTNESS;
    *green = (255 - led[2]) / RGB_LED_BRIGHTNESS;
}


bool rgbLedFadeHandler(uint8_t *red, uint8_t *green, uint8_t *blue) {
    static uint8_t RGB_loop_count = 0;
    static uint8_t RGB_led_stage = 0;
    static float RGBa[3] = { 255, 255, 255 };
    static float RGBb[3];
    static float RGBdiff[3];

    RGB_loop_count++;
    if (RGB_loop_count <= RGB_delay_factor) return 0;
    RGB_loop_count = 1;

    if (RGB_led_stage == 0) {
	LED_Color_Picker(&RGBb[0], &RGBb[1], &RGBb[2]);
    }
    if (RGB_led_stage < RGB_COLOR_CHANGE_STEPS)
    {
	for (uint8_t i = 0; i <= 2; i++)
	{
	    if (RGB_led_stage == 0)
	    {
		RGBdiff[i] = (RGBb[i] - RGBa[i]) / RGB_COLOR_CHANGE_STEPS;
	    }
	    RGBa[i] = constrain(RGBa[i] + RGBdiff[i], 0, 255);
//   	    Serial.print("Setting LED ");
//   	    Serial.print(int(i));
//   	    Serial.print(" to ");
//   	    Serial.print(int(RGBa[i]));
//   	    Serial.print(" at led stage ");
//   	    Serial.println(int(RGB_led_stage));
	}
	*red   = int(RGBa[0]);
	*green = int(RGBa[1]);
	*blue  = int(RGBa[2]);
	RGB_led_stage++;
	return 1;
    }
    else if (RGB_led_stage < RGB_COLOR_CHANGE_STEPS + RGB_COLOR_HOLD_STEPS)
    {
	if (RGB_led_stage == RGB_COLOR_CHANGE_STEPS) 
	{
	    // Serial.print(millis());
	    // Serial.println(": Holding Color for LED 1");
	}
	RGB_led_stage++;
	return 0;
    }
    else
    {
	// Serial.print(millis());
	// Serial.println(": Finished Color for LED 1");
	RGB_led_stage = 0;
	return 0;
    }
}


void loop() {
    static uint8_t red = 255;
    static uint8_t green = 255;
    static uint8_t blue = 255;
    static uint16_t curRingLed = 0 ;

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


    logger_publish("led", String(curRingLed));

    // 
    for (uint8_t i=0; i < 10; i++) { // how many times we change LED per second
	curRingLed = ( curRingLed + 1 ) % MAX_LEDS ;
	Serial.print("LED to ");
	Serial.println(curRingLed);
	for (uint16_t j=0; j < 20; j++) { // how many times we change the LED color per LED display
	    rgbLedFadeHandler(&red, &green, &blue);
	    pixels.setPixelColor(curRingLed, pixels.Color(red, green, blue));
	    pixels.show();
	    delay(5);
	}
	pixels.setPixelColor((curRingLed-LED_TRAIL + MAX_LEDS) % MAX_LEDS, pixels.Color(0,0,0)); // turn off current LED
    }
}



/* ------------------------------------------------------------------------- */
// vim:sts=4:sw=4:syntax=cpp

