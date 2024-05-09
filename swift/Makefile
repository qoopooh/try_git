# Swift Makefile
# Targets for building Swift files for macOS and Linux

SWIFT_FILES := $(wildcard *.swift)
EXECUTABLES := $(patsubst %.swift,out-%,$(SWIFT_FILES))
LINUX_EXECUTABLES := $(patsubst %.swift,out-%-linux,$(SWIFT_FILES))
BOOKWORM_EXECUTABLES := $(patsubst %.swift,out-%-bookworm,$(SWIFT_FILES))

# Default target builds for macOS
all: $(EXECUTABLES)

# Build for macOS
out-%: %.swift
	#swiftc -static-stdlib $< -o $@
	swiftc $< -o $@

# Build all for Linux using Docker
linux: $(LINUX_EXECUTABLES)

out-%-linux: %.swift
	docker run --rm --platform linux/amd64 -v "$(PWD)":/src -w /src swift:latest swiftc $< -static-executable -o $@

bookworm: $(BOOKWORM_EXECUTABLES)

out-%-bookworm: %.swift
	docker run --rm --platform linux/amd64 -v "$(PWD)":/src -w /src swift:bookworm swiftc $< -o $@ -static-stdlib

# Clean up executables
clean:
	rm -f $(EXECUTABLES) $(LINUX_EXECUTABLES) $(BOOKWORM_EXECUTABLES)

# List available files for building
list:
	@echo "Available Swift files:"
	@ls -1 *.swift

.PHONY: all linux clean list
