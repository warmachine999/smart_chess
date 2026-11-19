# Pinout and logical mapping

These are the documented V12 candidate assignments. Confirm them against the exact firmware and physical board before making wiring changes.

## LED output

| Signal | Assignment |
|---|---|
| Data GPIO | GPIO 2 |
| LED count | 64 |
| LED type | WS2812B |
| Color order | GRB |

## Matrix row outputs

| Row | GPIO |
|---|---:|
| R1 | 13 |
| R2 | 14 |
| R3 | 16 |
| R4 | 17 |
| R5 | 18 |
| R6 | 19 |
| R7 | 21 |
| R8 | 22 |

## Physical column inputs

| Physical column | GPIO |
|---|---:|
| C1 | 23 |
| C2 | 25 |
| C3 | 26 |
| C4 | 27 |
| C5 | 32 |
| C6 | 33 |
| C7 | 4 |
| C8 | 5 |

## Logical files A–H

The documented logical-to-physical column order is **4, 7, 8, 1, 5, 3, 6, 2**.

| Logical file | Physical column | GPIO |
|---|---:|---:|
| A | 4 | 27 |
| B | 7 | 4 |
| C | 8 | 5 |
| D | 1 | 23 |
| E | 5 | 32 |
| F | 3 | 26 |
| G | 6 | 33 |
| H | 2 | 25 |

Do not replace this with a simple 1-to-1 mapping unless the board has been physically rewired and recalibrated. Validate the complete 8×8 board after any changes.
