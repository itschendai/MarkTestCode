#!/usr/bin/env bash

set -e

# Ensure Python 3 and pip are available
if ! command -v python3 >/dev/null 2>&1; then
    echo "Installing Python3 and pip..."
    sudo apt-get update
    sudo apt-get install -y python3 python3-pip
fi

# Install PlatformIO using pip if it's not installed
if ! command -v platformio >/dev/null 2>&1; then
    echo "Installing PlatformIO..."
    python3 -m pip install --user -U platformio
    export PATH="$PATH:$HOME/.local/bin"
fi

# Install the project libraries defined in platformio.ini
platformio pkg install

echo "Setup complete. You can now build the firmware using 'platformio run'."

