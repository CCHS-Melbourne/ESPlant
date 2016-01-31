/*
ESP_Kwai.cpp - ESP8266 Library which bridges I2C to ADC on the ESPlant.

  Copyright (c) 2014 Mark Wolfe and Angus Gratton. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  Modified 8 May 2015 by Hristo Gochkov (proper post and file upload handling)

*/

#include <Wire.h>
#include <Arduino.h>
#include "ESP_Kwai.h"

ESP_Kwai::ESP_Kwai() {
}

bool ESP_Kwai::begin(uint8_t vsens_pin) {
    Wire.begin();
    Serial.println("Enabling VSens...");
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

bool ESP_Kwai::readEvent(kwai_event_t* event) {

    int i;
    int16_t val;
    for(i = 0; i < NUM_ADC_CHANNELS; i++) {

      switch(i) {
        case ADC_UV:
          val = read_adc(ADC_UV);
          if (val == -1) {
            return false;
          }
          event->UVSensor = val;
          break;

        case ADC_1:
          val = read_adc(ADC_1);
          if (val == -1) {
            return false;
          }
          event->ADC01 = val;
          break;

        case ADC_2:
          val = read_adc(ADC_2);
          if (val == -1) {
            return false;
          }
          event->ADC02 = val;
          break;

        case ADC_SOIL1:
          val = read_adc(ADC_SOIL1);
          if (val == -1) {
            return false;
          }
          event->Soil01 = val;
          break;

        case ADC_SOIL2:
          val = read_adc(ADC_SOIL2);
          if (val == -1) {
            return false;
          }
          event->Soil02 = val;
          break;

        case ADC_INPUT_VOLTAGE:
          val = read_adc(ADC_INPUT_VOLTAGE);
          if (val == -1) {
            return false;
          }
	  /* Convert raw units to millivolts */
          event->InputVoltage = (uint32_t)val * 1825 / 1000;
          break;

        case ADC_TEMPSENSOR:
          val = read_adc(ADC_TEMPSENSOR);
          if (val == -1) {
            return false;
          }
          event->InternalTemp = val;
          break;
      }
    }

    return true;
}

int16_t ESP_Kwai::read_adc(int channel) {
  Wire.beginTransmission(0x50);
  Wire.write(channel);
  int res = Wire.endTransmission();

  if (res != 0 ) {
    Serial.print("Failed ADC i2c write channel number 5 = ");
    Serial.println(res);
    return -1;
  }

  res = Wire.requestFrom(0x50, 2);
  if (res != 2) {
    Serial.print("Failed ADC i2c readback = ");
    Serial.println(res);
    return -1;
  }

  uint8_t low = Wire.read();
  uint8_t high = Wire.read();
  return low + ((int)high << 8);
}
