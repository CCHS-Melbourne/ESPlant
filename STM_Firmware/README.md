# Summary

This is a firmware for STM32F042P6, which provides two functions

* USB CDC ACM "USB to serial" interface to MCU peripheral UART2. Data sent from USB is sent out from the UART, and vice versa.
* I2C slave device ADC, providing multiple channels on MCU peripheral IRC1. Simple I2C protocol allows for reading ADC data on each channel.

# USB CDC Usage

* Should "just work" on OS X, Linux and Windows 10.

Older Windows versions TBD...

# I2C ADC Usage

Device slave address for the STM32F042 is 0x50.

Master begins an ADC conversion by performing a 1-byte write of the ADC channel number (0-6, mappings near top of i2c_adc_if.c source file). For ESPlant ADC channels 0-4 are external pin connections, channel 5 is battery voltage, channel 6 is the internal temperature sensor.

Master then performs a new I2C read transaction with length two bytes, these are the result of an ADC read on that channel (little endian order).

If an error occurs or the conversion is not completed, a result value of 0xFFFF is read. When using 10kHz i2c no special delay is necessary between the write and read transactions. For higher i2c data rates it may be necessary to add a short delay between the two steps.

If the slave NAKs either request, delay at least 2ms and start again.

# Architecture

This firmware was created using the STM32Cube libraries from ST. The `drivers` & `middlewares` directories are both taken directly from STM32Cube & [stm32cubemx](http://www.st.com/stm32cubemx). However you do not need stm32cubemx to build or modify this firmware.

stm32cubemx was used to create the original project and hardware configuration boilerplate. Then the IDE project file was conerted to a Makefile using baoshi's [CubeMX2Makefile.py](https://github.com/baoshi/CubeMX2Makefile).

The actual code structure runs almost entirely in interrupt handlers.

# Build Dependencies

"gcc-arm-none-eabi" toolchain, newlib, GNU make.

On Debian/Ubuntu the packages to install are `build-essential`, `gcc-arm-none-eabi`, `libnewlib-arm-none-eabi`, and `binutils-arm-none-eabi`.

Development versions were:
* gcc 4.9.3 (4.9 series or newer recommended as some 4.9 features are used to fit the current firmware inside 16kB of flash.)
* binutils 2.25
* newlib 2.2.0

# Uploading/Debugging

Use [openocd](http://openocd.org/). Other tools will probably work too.

The source tree has an openocd.cfg and some hardcoded openocd commands in the Makefile. These will work if you're using openocd and an STLINK debugger/programmer.

`make flash` to flash the image. Debugger will keep running, Ctrl-C to quit. MCU will automatically reset into new firmware.

`make gdb` to attach gdb (if installed) to a running openocd. `mon halt reset` will probably be required as a first command, to synchronise gdb with target.

# Known Issues

* Still some ugly boilerplate STM32Cube generated code that can be refactored out.

* STM32Cube is pretty resource-intensive for a small MCU. LTO (Link Time Optimisation) is required to fit the firmware within 16kB of flash, by collapsing lots of hierarchical calls & abstractions.

* When transmitting large amounts of data from USB to serial, it's possible to starve out the i2c device for CPU resources. Symptoms of this will be large numbers of NAKed i2c packets to the i2c master.

* No low power modes yet. Due to interrupt-driven architecture is should be fairly easy to use WFE or STOP mode in the main loop, rather than a busy wait.

