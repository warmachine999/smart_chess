# Test checklist

Mark a test complete only after performing it on the stated target.

## Build and connectivity

- [ ] Compile the exact firmware with the selected ESP32 board and partition.
- [ ] Record program storage and global memory figures.
- [ ] Connect to the local access point.
- [ ] Load both `/app` and `/` routes.
- [ ] Confirm online/offline indicators follow real API responses.

## Sensors and LEDs

- [ ] Test each of the 64 reed switches individually.
- [ ] Confirm H8 is active and stable.
- [ ] Check that one magnet does not activate neighboring cells.
- [ ] Test all 64 LEDs and verify logical square orientation.
- [ ] Verify serpentine/reversed-row mapping.
- [ ] Verify saved mapping survives a restart.
- [ ] Verify sensitivity and settle-time controls affect scanning.

## Chess behavior

- [ ] Test legal moves for every piece type.
- [ ] Test castling, en passant, and promotion.
- [ ] Test check, checkmate, stalemate, and draw handling.
- [ ] Confirm illegal moves are rejected and recoverable.
- [ ] Confirm the king cannot be captured.

## Interface and persistence

- [ ] Test the dashboard on desktop and phone.
- [ ] Test English and Arabic directionality.
- [ ] Test themes, brightness, and animation controls.
- [ ] Test calibration pause/resume and persistence.
- [ ] Test custom animation save/load/delete.
- [ ] Test diagnostics and error messages.

## External services

- [ ] Test Stockfish only with a real configured engine.
- [ ] Test Chess.com public-data access through a supported route.
- [ ] Test multiplayer only with a real reachable Hub/server.

**Do not mark untested items as passed.**
