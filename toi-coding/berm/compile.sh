#!/bin/bash

# Cross-platform compile script for Mac, Windows (Git Bash/WSL), and Linux

# Detect OS
OS="$(uname -s)"
case "${OS}" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    CYGWIN*)    MACHINE=Windows;;
    MINGW*)     MACHINE=Windows;;
    MSYS*)      MACHINE=Windows;;
    *)          MACHINE="UNKNOWN:${OS}"
esac

# Set source and output files (allow command-line override)
if [ -n "$1" ]; then
    SOURCE="$1"
    # Generate output name from source (remove .cpp extension)
    OUTPUT="out-${SOURCE%.cpp}"
else
    SOURCE="A1-001.cpp"
    OUTPUT="A1-001"
fi

# Find g++ compiler
if [ "$MACHINE" = "Windows" ]; then
    # Windows: Try common paths for g++ (Git Bash, MinGW, MSYS2)
    if command -v g++ &> /dev/null; then
        GPP="g++"
    elif [ -f "/usr/bin/g++" ]; then
        GPP="/usr/bin/g++"
    elif [ -f "/mingw64/bin/g++.exe" ]; then
        GPP="/mingw64/bin/g++.exe"
    elif [ -f "/c/msys64/mingw64/bin/g++.exe" ]; then
        GPP="/c/msys64/mingw64/bin/g++.exe"
    else
        echo "Error: g++ compiler not found. Please install MinGW or use Git Bash."
        exit 1
    fi
else
    # Mac and Linux: Use system g++ or find it
    if command -v g++ &> /dev/null; then
        GPP="g++"
    elif [ -f "/usr/bin/g++" ]; then
        GPP="/usr/bin/g++"
    else
        echo "Error: g++ compiler not found. Please install g++."
        exit 1
    fi
fi

# Detect compiler type and set appropriate flags
COMPILER_TYPE=""
if [ "$MACHINE" = "Mac" ]; then
    # On macOS, check if we're using clang (common) or real g++
    if $GPP --version 2>&1 | grep -q "clang"; then
        COMPILER_TYPE="clang"
    else
        COMPILER_TYPE="gcc"
    fi
elif [ "$MACHINE" = "Linux" ]; then
    COMPILER_TYPE="gcc"
elif [ "$MACHINE" = "Windows" ]; then
    COMPILER_TYPE="gcc"
fi

# Set compilation flags based on compiler and OS
if [ "$COMPILER_TYPE" = "clang" ] || [ "$MACHINE" = "Mac" ]; then
    # macOS/clang: Remove -static and -s flags (not supported)
    FLAGS="-DEVAL -std=gnu++17 -O2 -pipe"
else
    # Linux/Windows with g++: Use all flags including -static and -s
    FLAGS="-DEVAL -std=gnu++17 -O2 -pipe -static -s"
fi

# Check if source file exists
if [ ! -f "$SOURCE" ]; then
    echo "Error: Source file '$SOURCE' not found."
    exit 1
fi

# Compile
echo "Compiling $SOURCE with $GPP..."
echo "Command: $GPP $FLAGS -o $OUTPUT $SOURCE"

$GPP $FLAGS -o "$OUTPUT" "$SOURCE"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful! Output: $OUTPUT"
    # Make output executable on Unix-like systems
    if [ "$MACHINE" != "Windows" ]; then
        chmod +x "$OUTPUT"
    fi
else
    echo "Compilation failed!"
    exit 1
fi
