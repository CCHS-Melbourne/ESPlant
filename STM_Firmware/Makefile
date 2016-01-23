######################################
# Makefile by CubeMX2Makefile.py
######################################

######################################
# target
######################################
TARGET = stm_fw_cube

######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -Os

#######################################
# pathes
#######################################
# source path
# Build path
BUILD_DIR = build

######################################
# source
######################################
C_SOURCES = \
  drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
  drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c \
  middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
  middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
  middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
  middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
  src/main.c \
  src/i2c_adc_if.c \
  src/stm32f0xx_hal_msp.c \
  src/stm32f0xx_it.c \
  src/usb_device.c \
  src/usbd_cdc_if.c \
  src/usbd_conf.c \
  src/usbd_desc.c
ASM_SOURCES = \
  drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f042x6.s

#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
AR = arm-none-eabi-ar
SZ = arm-none-eabi-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# macros for gcc
AS_DEFS =
C_DEFS = -DUSE_HAL_DRIVER -DSTM32F042x6
# includes for gcc
AS_INCLUDES =
C_INCLUDES = -Iinc
C_INCLUDES += -Idrivers/STM32F0xx_HAL_Driver/Inc
C_INCLUDES += -Idrivers/STM32F0xx_HAL_Driver/Inc/Legacy
C_INCLUDES += -Imiddlewares/ST/STM32_USB_Device_Library/Core/Inc
C_INCLUDES += -Imiddlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
C_INCLUDES += -Idrivers/CMSIS/Include
C_INCLUDES += -Idrivers/CMSIS/Device/ST/STM32F0xx/Include
# compile gcc flags
ASFLAGS = -mthumb -mcpu=cortex-m0 $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections
CFLAGS = -mthumb -mcpu=cortex-m0 $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections
CFLAGS += -g -gdwarf-2
# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d

# Link Time Optimisation
CFLAGS += -flto
ASFLAGS += -flto

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F042F4Px_FLASH.ld
# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = -g -mthumb -mcpu=cortex-m0 -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# this flash target is very hacky and probably should be replaced
flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink-v2-1.cfg -c "transport select hla_swd" -f target/stm32f0x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf reset"

# so is this gdb target
gdb:
	arm-none-eabi-gdb -ex "target extended-remote :3333" "$(BUILD_DIR)/$(TARGET).elf"

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
