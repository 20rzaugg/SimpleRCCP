# SimpleRCCP
Simple Roller Coaster Control Panel running on an Arduino Giga R1 Wifi

## NL2 Telemetry over USB Serial
This project now supports streaming NoLimits 2 telemetry from a PC Python script to the Giga R1 over USB CDC serial.

### Microcontroller side
- Firmware listens on `Serial` at `115200` baud.
- Expected frame format (ASCII, newline-delimited):
`T,<seq>,<in_play>,<paused>,<braking>,<train_in_station>,<e_stop>,<can_dispatch>,<speed>,<gx>,<gy>,<gz>`
- If no valid frame is received for 1 second, firmware falls back to blink/test behavior.

1. Install dependency:
`pip install pyserial`
2. Start NL2 with telemetry enabled (`--telemetry`).
3. Run bridge (example):
`python py_nl2telemetry/serial_bridge.py --serial-port COM5 --baud 115200 --hz 20`

Adjust `COM5` to your Arduino USB serial port.
