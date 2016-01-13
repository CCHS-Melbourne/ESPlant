/* Some firmware configuration aspects */
#pragma once

/* IRQ Priorities */
#define ADC1_IRQ_PRIORITY      1 /* higher priority than i2c, prevents race when copying value */
#define USART_IRQ_PRIORITY     1
#define USB_IRQ_PRIORITY       2
#define USB_TIMER_IRQ_PRIORITY 2
#define I2C1_IRQ_PRIORITY      3

