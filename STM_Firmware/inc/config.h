/* Some top-level firmware configuration aspects */
#pragma once

/* Version numbering

   Something like semver - major version increments when functionality/interface changes,
   minor version increments when bugs get fixed.

   Version number is reported in the USB Device "serial number" field.
*/
#define FIRMWARE_VERSION_MAJOR 0
#define FIRMWARE_VERSION_MINOR 1
#define __STR(X) #X
#define _STR(X) __STR(X)
#define FIRMWARE_VERSION_STR _STR(FIRMWARE_VERSION_MAJOR) "." _STR(FIRMWARE_VERSION_MINOR) " git " GIT_REVISION

/* IRQ Priorities */
#define ADC1_IRQ_PRIORITY      1 /* higher priority than i2c, prevents race when copying value */
#define USART_IRQ_PRIORITY     1
#define USB_IRQ_PRIORITY       2
#define USB_TIMER_IRQ_PRIORITY 2
#define I2C1_IRQ_PRIORITY      3

