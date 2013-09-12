TEMPLATEROOT = ..

## compilation flags for gdb
CFLAGS  = -Os -g
#CFLAGS  = -g
ASFLAGS = -g 
LDFLAGS += -Os -nostartfiles -Wl,--gc-sections

## object files
OBJS = $(addprefix $(OBJDIR)/, \
    $(STARTUP) \
    main.o \
    stm32f10x_gpio.o \
    stm32f10x_rcc.o \
    stm32f10x_usart.o \
    mini-printf.o \
)

## include common make file
include $(TEMPLATEROOT)/Makefile.common

