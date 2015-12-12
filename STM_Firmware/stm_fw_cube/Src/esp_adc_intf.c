#include "stm32f0xx_hal.h"
static void i2c_init(void);
static void adc_init(void);
static void timeout_timer_init(void);

static ADC_HandleTypeDef hadc;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htimeout;

static uint8_t adc_addr = 0xFF; /* single byte ADC register number */
static volatile uint16_t adc_value;

void esp_adc_intf_init(void)
{
  adc_init();
  i2c_init();
  timeout_timer_init();

  HAL_I2C_Slave_Receive_IT(&hi2c1, &adc_addr, 1);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* Go back to waiting for an ADC address */
  HAL_TIM_Base_Stop_IT(&htimeout);
  HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t*)&adc_value, 2);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if(1) { //adc_addr >= 8) {
    /* Invalid ADC number received, so go back to waiting for an address */
    HAL_I2C_Slave_Receive_IT(&hi2c1, &adc_addr, 1);
    return;
  }

  /* Set up to transmit from the i2c buffer, assume that the client
     will wait long enough before reading that it doesn't just say
     0xFFFF !!
*/
  adc_value = 1234;
  //HAL_ADC_Start_IT(&hadc);
  HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t*)&adc_value, 2);
  //HAL_TIM_Base_Start_IT(&htimeout);
}

void esp_adc_timeout_elapsed_callback(void)
{
  /* Cancel slave transmit (not sure how?), restart slave receive */
  HAL_TIM_Base_Stop_IT(&htimeout);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc_unused)
{
  adc_value = (uint16_t)HAL_ADC_GetValue(&hadc);
}

static void adc_init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
   */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC;
  hadc.Init.Resolution = ADC_RESOLUTION12b;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ENABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = OVR_DATA_PRESERVED;
  HAL_ADC_Init(&hadc);

  /**Configure for the selected ADC regular channel to be converted. 
   */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  NVIC_SetPriority(ADC1_IRQn, 2);
  NVIC_EnableIRQ(ADC1_IRQn);
}

static void i2c_init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0x50<<1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
  HAL_I2C_Init(&hi2c1);

  NVIC_SetPriority(I2C1_IRQn, 3);
  NVIC_EnableIRQ(I2C1_IRQn);

  /**Configure Analogue filter
  */
  //HAL_I2CEx_AnalogFilter_Config(&hi2c1, I2C_ANALOGFILTER_ENABLED);
}

void timeout_timer_init(void)
{
  /* Configure a timeout timer for resetting the i2c state machine */
  htimeout.Instance = TIM2;
  htimeout.Init.Period = 50000 - 1; /* approx 50ms, I think... */
  htimeout.Init.Prescaler = 48-1;
  htimeout.Init.ClockDivision = 0;
  htimeout.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&htimeout);

  __TIM2_CLK_ENABLE();
  NVIC_SetPriority(TIM2_IRQn, 2);
  NVIC_EnableIRQ(TIM2_IRQn);
}
