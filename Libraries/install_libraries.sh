#!/bin/bash

# Run this script to install the modules (as symbolic links) in your local Arduino directory
# inside your home directory.
#
# The links mean that changes in this repository directory will
# automatically update the libraries that Arduino uses.

LIB_DIR="`dirname $0`"

cd "${LIB_DIR}"
git submodule init
git submodule update

# Try both the likely Arduino home directories
ARDUINO_DIR=$(realpath "${HOME}/Documents/Arduino")
if ! [ -d "$ARDUINO_DIR" ]; then
    ARDUINO_DIR="$(realpath ${HOME}/Arduino)"
    if ! [ -d "$ARDUINO_DIR" ]; then
	echo "Arduino directory not found at <home>/Documents/Arduino or <home>/Arduino. Check your Arduino install?"
	exit 1
    fi
fi

ARDUINO_LIB_DIR="${ARDUINO_DIR}/libraries"
mkdir -p ${ARDUINO_LIB_DIR}

for lib in *; do
    if ! [ -d "$lib" ]; then
	continue
    fi
    DEST="${ARDUINO_LIB_DIR}/$lib"
    if ! [ -d $(realpath "${DEST}") ]; then
	echo "Installing $lib..."
	ln -s "$(realpath $lib)" "${DEST}"
    else
	echo "$lib is already installed, skipping..."
    fi
done

echo "****"
echo "Done. You will need to restart the Arduino IDE before it sees the new libraries."
