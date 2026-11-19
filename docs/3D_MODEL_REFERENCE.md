# OpenChess 3D model reference

Smart Chess references the community-made **OpenChess – Plastic PCB** model by **jojo** on MakerWorld.

- [Original MakerWorld model](https://makerworld.com/en/models/1953392-openchess-plastic-pcb?from=search#profileId-2099164)
- [OpenChess build guide](https://joojoooo.github.io/OpenChess/)
- [Creator profile](https://ko-fi.com/joojooo)

## Important file and license note

The MakerWorld listing identifies the download under the **Standard Digital File License**. This repository therefore links to the creator's original listing rather than copying or re-uploading the creator's STL/3MF files. Download the files from the original listing and follow the license terms shown there. The model remains the creator's work; it is not part of the SmartChess firmware or an original SmartChess design.

## What the design is

The creator describes this as a printable chessboard structure and a "plastic PCB" that holds the board's electronics and wiring. The build guide describes an 8×8 illuminated board using Hall-effect sensors, magnets, LED strips, and a shift-register-based sensor arrangement.

The model listing and build guide are useful mechanical references for:
- the board and square construction;
- LED placement and light separation;
- sensor positioning;
- cable routing and underside organization;
- the controller/electronics mounting area.

## Compatibility warning: OpenChess is not a drop-in SmartChess wiring design

The OpenChess guide describes **A3144 Hall-effect sensors** and a **74HC595 shift register** arrangement. The current SmartChess documentation describes a **64-reed-switch row/column matrix** with its own GPIO mapping.

These are different sensing and wiring approaches. Do not copy the OpenChess wiring diagram or connect its sensor assembly to the SmartChess ESP32 pinout without redesigning and checking the electronics first. The physical model may be useful as a mechanical reference, but dimensions, mounting points, sensor holders, and cable paths must be checked against the actual SmartChess components before printing or assembly.

## Attribution

3D model: **OpenChess – Plastic PCB**  
Creator: **jojo**  
Original source: [MakerWorld model page](https://makerworld.com/en/models/1953392-openchess-plastic-pcb?from=search#profileId-2099164)

All rights to the original model remain with its creator and are governed by the license on the original listing.
