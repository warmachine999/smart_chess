# Installation

## Requirements

- Arduino IDE
- Espressif ESP32 board package
- ESP32-WROOM-32U-compatible board selection
- USB data cable
- FastLED library if required by the selected sketch

## Firmware

The V12 single-file candidate belongs in `firmware/`. Open it in Arduino IDE, select the correct ESP32 board and serial port, then use **Verify** before uploading.

The exact flash size and partition menu depend on the actual ESP32 module and installed board package. Do not copy a partition setting from another board without checking the hardware. A previous V11 build exceeded one selected application partition; V12 must be compiled independently.

## Connect to the board

Default development access point:

- SSID: `SmartChess-Test`
- Password: `12345678`
- IP: `192.168.4.1`

After connecting, open:

- Main app: `http://192.168.4.1/app`
- Hardware console: `http://192.168.4.1/` or `http://192.168.4.1/hardware`

The ESP32 access point does not automatically provide internet access. External services require a separate reachable network/server.

## First power-on checks

1. Verify the board and LED power wiring.
2. Confirm common ground between ESP32 and LED supply.
3. Connect over USB and open Serial Monitor at the baud rate configured in the sketch.
4. Verify that the access point starts and note the reported IP.
5. Test each LED square using a safe low-brightness test.
6. Test all 64 sensor positions individually.
7. Run calibration only after checking that a single magnet activates the expected sensor.
