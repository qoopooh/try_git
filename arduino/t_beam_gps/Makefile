BOARD?=esp32:esp32:t-beam
PORT?=/dev/cu.SLAB_USBtoUART

.PHONY: default lint all flash clean

default: all flash

lint:
	cpplint --extensions=ino --filter=-legal/copyright *.ino

all:
	arduino-cli compile --fqbn $(BOARD) ./

flash:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD)

clean:
	rm -r build
