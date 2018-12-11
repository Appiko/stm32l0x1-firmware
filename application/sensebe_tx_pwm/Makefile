
BOARD           := BOARD_SENSEBE_TX_REV1
DEBUGGER        := BMP
FW_VER_VAL      := 0


DOC_DIR         = ../../doc
PLATFORM_DIR    = ../../platform
CODEBASE_DIR    = ../../codebase

INCLUDEDIRS	= .
INCLUDEDIRS += $(CODEBASE_DIR)/CMSIS/Include
INCLUDEDIRS	+= $(CODEBASE_DIR)/STM32L0x
INCLUDEDIRS	+= $(PLATFORM_DIR)
INCLUDEDIRS += $(CODEBASE_DIR)/LL
INCLUDEDIRS += $(CODEBASE_DIR)/inits
INCLUDEDIRS += $(CODEBASE_DIR)/peripheral_modules
INCLUDEDIRS += $(CODEBASE_DIR)/STM32CubeMX

C_SRC_DIRS = .
C_SRC_DIRS += $(CODEBASE_DIR)/STM32L0x
C_SRC_DIRS += $(CODEBASE_DIR)/LL
C_SRC_DIRS += $(CODEBASE_DIR)/inits
C_SRC_DIRS += $(CODEBASE_DIR)/peripheral_modules
C_SRC_DIRS += $(CODEBASE_DIR)/STM32CubeMX

C_SRC = main.c
C_SRC += uart_printf.c
C_SRC += tinyprintf.c
C_SRC += pwm.c
C_SRC += stm32l0xx_ll_gpio.c
C_SRC += stm32l0xx_ll_tim.c 
C_SRC += stm32l0xx_ll_rcc.c
C_SRC += stm32l0xx_ll_pwr.c
C_SRC += stm32l0xx_ll_utils.c
C_SRC += stm32l0xx_ll_usart.c
C_SRC += stm32l0xx_ll_exti.c

#Gets the name of the application folder
APPLN = $(shell basename $(PWD))

CFLAGS_APP = -DDEBUG
CFLAGS_APP += -DUSE_FULL_LL_DRIVER
#Lower case of BOARD
BOARD_HEADER  = $(shell echo $(BOARD) | tr A-Z a-z)
include $(PLATFORM_DIR)/Makefile.$(BOARD_HEADER)

include ../Makefile.common
