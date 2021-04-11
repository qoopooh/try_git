# arduino-cli core list
# export BOARD=esp8266:esp8266:generic
# export BOARD=esp32:esp32:t-beam
# export PORT=/dev/cu.SLAB_USBtoUART
#

.PHONY: default lint all flash clean

default: all flash

lint:
	cpplint --extensions=ino --filter=-legal/copyright *.ino

all:
	arduino-cli compile --fqbn ${BOARD} ./

flash:
	arduino-cli upload -p ${PORT} --fqbn ${BOARD}

clean:
	rm -r build
