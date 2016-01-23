/* i2c slave interface to control onboard ADC */
#pragma once
#include <stdbool.h>

/* Call this once to initialise the i2c ADC interface */
void i2c_adc_if_init(void);

/* Returns true if the i2c interface is currently doing something */
bool i2c_adc_is_active(void);

