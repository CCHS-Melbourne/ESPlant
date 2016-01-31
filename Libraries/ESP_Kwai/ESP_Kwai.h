/*
ESP_Kwai.h - ESP8266 Library which bridges I2C to ADC on the ESPlant.

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
#ifndef ESP_Kwai_h
#define ESP_Kwai_h

#include <functional>

typedef struct {
    int16_t UVSensor;
    int16_t ADC01;
    int16_t ADC02;
    int16_t Soil01;
    int16_t Soil02;
    int16_t InputVoltage;
    int16_t InternalTemp;
} kwai_event_t;

enum kwai_adc_channel_t {
  ADC_UV = 0, /* "UV" */
  ADC_1 = 1, /* "ADC1" */
  ADC_2 = 2, /* "ADC2" */
  ADC_SOIL1 = 3, /* "SOIL1" */
  ADC_SOIL2 = 4, /* "SOIL2" */
  ADC_INPUT_VOLTAGE = 5, /* Input voltage monitor (highest of USB,solar or battery). In millivolts. */
  ADC_TEMPSENSOR = 6, /* internal temp sensor */
};

#define NUM_ADC_CHANNELS 7

class ESP_Kwai {
  public:
    ESP_Kwai();
    /* Start an ESP_Kwai event by applying power on the given VSens pin */
    bool begin(uint8_t vsens_pin = 14);
    /* Fill an "event" structure with all of the ADC channels at once */
    bool readEvent(kwai_event_t* event);

    /* Read a single channel and return the raw reading */
    int16_t read_adc(kwai_adc_channel_t channel);
};

#endif //ESP_Kwai_h
