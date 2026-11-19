# Hardware overview

Smart Chess is built around an 8×8 physical chessboard.

## Core hardware

- ESP32-WROOM-32U controller
- 64 reed switches arranged as an 8-row × 8-column matrix
- 64 WS2812B RGB LEDs
- Magnetic chess pieces

## Signal flow

1. A magnet in a chess piece changes the state of the reed switch below its square.
2. The ESP32 scans the matrix and maps physical readings to logical chess squares.
3. Firmware state and chess logic interpret the board changes.
4. The LED layer can provide visual feedback.
5. A browser connects to the ESP32's local web interface.

## Power and wiring notes

Use a suitable regulated supply for the LED strip. Ensure the ESP32 and LED supply share a common ground. Do not power a large LED array directly from an ESP32 GPIO or assume the board's USB supply can safely drive all LEDs. Confirm the LED supply voltage/current requirements and the actual assembled wiring before powering the board.

The firmware's LED orientation is serpentine/reversed-row mapped. Test the full 64-square mapping before permanently fixing the strip orientation.

## Known engineering cautions

- Keep all 64 sensor positions active, including H8.
- Check for adjacent-switch activation and matrix ghosting.
- Calibration cannot repair shorts or magnetic coupling that activates multiple switches.
- Confirm the exact board pin mapping in firmware before rewiring.
- The Wi-Fi access point is local-only by default.
