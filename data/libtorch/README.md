## Install Dependencies

First, install Homebrew if you haven't already, then install LibTorch:

```bash
# Install Homebrew (if needed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install LibTorch via Homebrew
brew install libtorch
```

## Build and Run

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (pointing to Homebrew's LibTorch)
cmake -DCMAKE_PREFIX_PATH=$(brew --prefix libtorch) ..

# Build
make

# Run
./example
```
