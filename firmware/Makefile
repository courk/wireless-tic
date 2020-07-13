######################################
# target
######################################
TARGET = wireless_tic


######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -O3


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/spi.c \
Src/usart.c \
Src/tx_loop.c \
Src/rx_loop.c \
Src/stm32f0xx_it.c \
Src/stm32f0xx_hal_msp.c \
Src/nrf24.c \
Src/nrf24_hal.c \
Src/aes.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi_ex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_exti.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c \
Src/system_stm32f0xx.c  

# ASM sources
ASM_SOURCES =  \
startup_stm32f030x6.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F030x6


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32F0xx_HAL_Driver/Inc \
-IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F0xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -DDEBUG
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# AES CTR mode only
CFLAGS += -DCBC=0 -DECB=0 -DCTR=1

RX_CFLAGS += $(CFLAGS) -DRX_VERSION
TX_CFLAGS += $(CFLAGS) -DTX_VERSION

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F030F4Px_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
RX_LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/rx/$(TARGET).map,--cref -Wl,--gc-sections
TX_LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/tx/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET)_rx.elf $(BUILD_DIR)/$(TARGET)_tx.elf


#######################################
# build the application
#######################################
# list of objects
RX_OBJECTS = $(addprefix $(BUILD_DIR)/rx/,$(notdir $(C_SOURCES:.c=.o)))
TX_OBJECTS = $(addprefix $(BUILD_DIR)/tx/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
RX_OBJECTS += $(addprefix $(BUILD_DIR)/rx/,$(notdir $(ASM_SOURCES:.s=.o)))
TX_OBJECTS += $(addprefix $(BUILD_DIR)/tx/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/rx/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -DRX_VERSION -c $(RX_CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/rx/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/tx/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -DTX_VERSION -c $(TX_CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/tx/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/rx/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(RX_CFLAGS) $< -o $@

$(BUILD_DIR)/tx/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(TX_CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET)_rx.elf: $(RX_OBJECTS) Makefile
	$(CC) $(RX_OBJECTS) $(RX_LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/$(TARGET)_tx.elf: $(TX_OBJECTS) Makefile
	$(CC) $(TX_OBJECTS) $(TX_LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR):
	mkdir -p $@/rx
	mkdir -p $@/tx

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
rx_flash:
	-openocd -f interface/jlink.cfg -f target/stm32f0x.cfg -c "program $(BUILD_DIR)/$(TARGET)_rx.elf verify reset exit"

tx_flash:
	-openocd -f interface/jlink.cfg -f target/stm32f0x.cfg -c "program $(BUILD_DIR)/$(TARGET)_tx.elf verify reset exit"

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
