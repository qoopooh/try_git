TEMPLATEROOT = ..

## compilation flags for gdb
#CFLAGS  = -O1 -g
CFLAGS  = -g
ASFLAGS = -g 

## object files
OBJS = $(addprefix $(OBJDIR)/, \
    $(STARTUP) \
    main.o \
    stm32f10x_gpio.o \
    stm32f10x_rcc.o \
)

## include common make file
include $(TEMPLATEROOT)/Makefile.common

