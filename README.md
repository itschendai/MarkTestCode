# Mark the Smart Bookmark

## Get started

Frontend designed and made with Squareline Studio
Backend written and compiled with PlatformIO

Device:
T-Display S3 AMOLED V2.0/Touch Version (Basic Version No Touch)

Processor:
ESP32-S3

Display:
240*536 Pixels RM67162

Rotary Encoder:
Adafruit I2C Stemma QT Rotary Encoder Breakout with NeoPixel

## Setup

Run `setup.sh` to install PlatformIO and the required libraries:

```bash
./setup.sh
```

This script installs PlatformIO using `pip` (if needed) and fetches all project dependencies defined in `platformio.ini`.

## Code structure

The firmware is organized as a PlatformIO project for the LilyGo T‑Display AMOLED board. Important folders and files include:

- `src/` – main application sources written in C++.
  - `main.cpp` – initializes the display, LVGL, and ties in the rotary encoder and UI.
  - `ui_manager.*` – controls screen transitions and synchronizes the selected book name.
  - `book_manager.*` – reads/writes `/books.json` on SPIFFS and provides data for the UI.
  - `rotary_encoder.*` – interfaces with the Adafruit seesaw encoder and its NeoPixel.
- `lib/` – custom libraries and the SquareLine generated `ui/` directory for LVGL.
- `boards/` – board definition JSON for PlatformIO.
- `platformio.ini` – defines the ESP32 platform and required libraries.

Build with:

```bash
platformio run
```

Upload to the board with:

```bash
platformio run -t upload
```
