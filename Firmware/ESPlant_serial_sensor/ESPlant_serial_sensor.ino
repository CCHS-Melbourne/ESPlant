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
// How many intermediate shades of colors.
#define RGB_COLOR_CHANGE_STEPS 100
// How many steps we hold the target colors.
#define RGB_COLOR_HOLD_STEPS 20
// How bright to light up neopixel. 1 very bright (blinding), bigger is less bright
// #define RGB_LED_BRIGHTNESS 3
uint8_t RGB_LED_BRIGHTNESS = 1;
// How many trailing LEDs get displayed on neopixel (1 to 10)
// #define RGB_LED_TRAIL 8
uint8_t RGB_LED_TRAIL = 0;
// How quickly we rotate LEDs (1 to 10)
uint8_t RGB_LED_SPEED = 1;
// How long to keep LEDs on after detection motion (seconds)
#define LED_TIMEOUT 10

void setup() {
    Serial.begin(115200);
    Serial.println("ESPlant_serial_sensor\n");

    Serial.printf("AP: ESP-%06X\n", ESP.getChipId());

    ESP_Kwai.begin();
    dallasTemp.begin();
    bool bme_res = bme.begin();

    if (!bme_res) {
	Serial.println(F("Uh - oh, could not find a valid BME280 sensor, check i2c address (see comments) and soldering! (or try a full power cycle)"));
    }

    if(!accel.begin()){
	Serial.println(F("Uh - oh, could not find a valid ADXL345 sensor, check i2c address (see comments) and soldering! (or try a full power cycle)"));
    } else {
	accel.setRange(ADXL345_RANGE_16_G);
    }

    // Turn all NeoPixels off
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.show();
    Serial.println(F("You can change LED intensity and length/speed by touching soil sensors with wet fingers"));
    delay(2000);
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
    static uint8_t RGB_led_stage = 0;
    static float RGBa[3] = { 255, 255, 255 };
    static float RGBb[3];
    static float RGBdiff[3];

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
    static uint32_t last_motion = 0;
    static bool leds_on = TRUE;

    Serial.println ( "------------------------" ) ;

    sensors_event_t event;
    kwai_event_t kevent;

    accel.getEvent(&event);
    ESP_Kwai.readEvent(&kevent);
    dallasTemp.requestTemperatures();
 
    logger_publish_c("acc/x", String(event.acceleration.x));
    logger_publish_c("acc/y", String(event.acceleration.y));
    logger_publish("acc/z", String(event.acceleration.z));
 
    logger_publish("adc/internal_temp", String(kevent.InternalTemp*0.01)+"C");
    logger_publish("temp", String(bme.readTemperature())+"C");
    logger_publish("external/temp_sensor", String(dallasTemp.getTempCByIndex(0))+"C");
    logger_publish("pressure", String(bme.readPressure() / 100.0F)+"mbar");
    logger_publish("humidity", String(bme.readHumidity())+"%");
 
    logger_publish("adc/soil_1", String(kevent.Soil01));
    logger_publish("adc/soil_2", String(kevent.Soil02));
    // ADC shows value below 100 when nothing is connected and goes up to high 3200s
    // out of 4096.
    // We take this down to 1 to 11
    RGB_LED_BRIGHTNESS = 1 + (3200 - kevent.Soil01) / 330;
    // Take to 0 to 10
    RGB_LED_TRAIL = (3200 - kevent.Soil02) / 330;
    RGB_LED_SPEED = 11 - RGB_LED_TRAIL;

    // UV value and solar panel voltage should be loosely correlated.
    logger_publish("adc/uv_sensor", String(kevent.UVSensor));
#ifdef SOLAR_CONNECTED_ADC2
    // I measure panel voltage separately with a voltage divider (2x 10k
    // resistor), after measuring real voltage ends up being 1.67V what ADC reads
    logger_publish("adc/solar_voltage", String(kevent.ADC02*0.00167)+"V");
#endif
    logger_publish("adc/input_voltage", String(kevent.InputVoltage*0.001)+"V");
    // I used a volt meature to measure a correction factor (0.956 instead of 1)
    logger_publish("chip/vcc", String(ESP.getVcc()*0.000956)+"V");

    logger_publish("pir", digitalRead(15) ? "HIGH" : "low" );
    if (digitalRead(15)) {
	last_motion = millis();
    }   

    logger_publish("chip/free_heap", String(ESP.getFreeHeap()));
    logger_publish("led", String(curRingLed));
#ifdef FIXED_MOTION_SENSOR
    leds_on = ((millis() - last_motion)/1000 < LED_TIMEOUT);
#else
    leds_on = TRUE;
#endif
// This can slow down the loop a fair bit, and this goes away after reboot
     if (leds_on) {
 	Serial.print("+++ LEDs> brightness: ");
 	Serial.print(kevent.Soil01);
 	Serial.print(" > ");
 	Serial.print(RGB_LED_BRIGHTNESS);
 	Serial.print(", trail: ");
 	Serial.print(kevent.Soil02);
 	Serial.print(" > ");
 	Serial.println(RGB_LED_TRAIL);
     } else {
 	Serial.println("+++ LEDs: OFF");
     }

    for (uint8_t i=0; i < 10; i++) { // how many times we change LED per second
	curRingLed = ( curRingLed + 1 ) % MAX_LEDS ;
	// Serial.print("LED to ");
	// Serial.println(curRingLed);
	for (uint16_t j=0; j < 20; j++) { // how many times we change the LED color per LED display
	    rgbLedFadeHandler(&red, &green, &blue);
	    if (leds_on) {
		pixels.setPixelColor(curRingLed, pixels.Color(red, green, blue));
		pixels.show();
	    }
	    delay(RGB_LED_SPEED);
	}
	pixels.setPixelColor((curRingLed-RGB_LED_TRAIL + MAX_LEDS) % MAX_LEDS, pixels.Color(0,0,0)); // turn off current LED
	if (!leds_on) pixels.show();
    }
}



/* ------------------------------------------------------------------------- */
// vim:sts=4:sw=4:syntax=cpp

