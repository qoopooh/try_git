#/* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
# * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
# * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
# * THE AUTHORS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
# * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. USE IT AT YOUR OWN RISK */

ARCH = arm-none-eabi
MBED_VOLUME = /Volumes/MBED

# Tool definitions
CC      = $(ARCH)-gcc
LD      = $(ARCH)-ld
AR      = $(ARCH)-ar
AS      = $(ARCH)-as
CP      = $(ARCH)-objcopy
OD	= $(ARCH)-objdump
SIZE	= $(ARCH)-size
RM	= rm -rf
Q	= # @./quiet "$@"

# Flags
#CFLAGS   = -W -Wall -Os --std=gnu99 -fgnu89-inline -mcpu=cortex-m3 -mthumb
CFLAGS   = -W -Wall -O0 --std=gnu99 -fgnu89-inline -mcpu=cortex-m3 -mthumb
CFLAGS  += -ffunction-sections -fdata-sections
ASFLAGS  =
LDFLAGS  = --gc-sections
CPFLAGS  =
ODFLAGS  = -x --syms
PRFLAGS ?=

# Source files
LINKER_SCRIPT = LPC17xx.ld
#CSRCS  = startup_LPC17xx.c $(wildcard *.c)
#CSRCS += main.c
ASRCS  = 

NAME            = blink
OBJDIR          = obj
OBJS            = $(addprefix $(OBJDIR)/, \
                  main.o \
                  core_cm3.o \
                  system_LPC17xx.o \
                  startup_LPC17xx.o \
    )
DBG             = $(addprefix $(OBJDIR)/, $(NAME).d)
BIN             = $(addprefix $(OBJDIR)/, $(NAME).bin)
HEX             = $(addprefix $(OBJDIR)/, $(NAME).hex)
ELF             = $(addprefix $(OBJDIR)/, $(NAME).elf)
LST             = $(addprefix $(OBJDIR)/, $(NAME).lst)


#OBJS   = $(CSRCS:.c=.o) $(ASRCS:.s=.o)

.PHONY: all size clean nuke

#all: main.bin main.hex 
all: ${OBJS} ${ELF} ${BIN} ${HEX} ${LST}
#all: ${BIN} ${HEX}

isp: ${BIN}
#	@./quiet $< cp $^ $(MBED_VOLUME)/

size: ${ELF}
	@$(SIZE) $<

${HEX}: ${ELF}
	$Q $(CP) $(CPFLAGS) -O ihex $< $@

${BIN}: ${ELF}
	$Q $(CP) $(CPFLAGS) -O binary $< $@

${LST}: ${ELF}
	${OD} -dSt $^ >$@


$(ELF): $(LINKER_SCRIPT) $(OBJS)
	$Q $(LD) -Map $(@:.elf=.map) $(LDFLAGS) -T $^ -o $@ 
	$Q $(OD) $(ODFLAGS) $@ > $(@:.elf=.dump)
	@$(SIZE) $@

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	@$(CC) -MM $< -MF ${DBG} -MP
	$Q $(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.S
	$Q $(AS) $(ASFLAGS) $< -o $@

clean:
	@-rm -rf *.elf
	@-\
for D in "." "**"; do \
  $(RM) $$D/*.o $$D/*.d $$D/*.lst $$D/*.dump $$D/*.map ${OBJDIR}; \
done

nuke: clean
	$(RM) *.hex *.bin

-include $(CSRCS:.c=.d)

