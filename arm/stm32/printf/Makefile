TEMPLATEROOT = ..

## compilation flags for gdb
#CFLAGS  = -O1 -g
CFLAGS  = -g
ASFLAGS = -g 

## object files
OBJS = $(addprefix $(OBJDIR)/, \
    $(STARTUP) \
    main.o \
    newlib_stubs.o \
    stm32f10x_gpio.o \
    stm32f10x_rcc.o \
    stm32f10x_usart.o \
    core_cm3.o \
)

## include common make file
include $(TEMPLATEROOT)/Makefile.common

