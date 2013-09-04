# makfile configuration
NAME            = blink
OBJDIR          = obj
OBJS            = $(addprefix $(OBJDIR)/, \
                  main.o \
                  init.o \
    )
BIN             = $(addprefix $(OBJDIR)/, $(NAME).bin)
HEX             = $(addprefix $(OBJDIR)/, $(NAME).hex)
ELF             = $(addprefix $(OBJDIR)/, $(NAME).elf)
LST             = $(addprefix $(OBJDIR)/, $(NAME).lst)

#CPU             = msp430g2231
CPU             = msp430g2452

#CFLAGS          = -mmcu=${CPU} -O2 -Wall -g
CFLAGS          = -mmcu=${CPU} -O2 -Wall

#switch the compiler (for the internal make rules)
CC              = msp430-gcc

.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is run without args
all: ${OBJS} ${ELF} ${HEX} ${LST}

#confgigure the next line if you want to use the serial download
download: download-jtag
#download: download-bsl

#project dependencies
$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

#additional rules for files
${ELF}: ${OBJS}
	${CC} -mmcu=${CPU} -o $@ ${OBJS}

${HEX}: ${ELF}
	msp430-objcopy -O ihex $^ $@

${BIN}: ${ELF}
	msp430-objcopy -O binary $^ $@

${LST}: ${ELF}
	msp430-objdump -dSt $^ >$@

download-jtag: all
	msp430-jtag -e ${ELF}

download-bsl: all
	msp430-bsl -e ${ELF}

clean:
	rm -rf ${NAME}.elf ${NAME}.hex ${NAME}.lst ${OBJDIR}

#backup archive
dist:
	tar czf dist.tgz *.c *.h *.txt makefile

#dummy target as dependecy if something has to be build everytime
FORCE:


