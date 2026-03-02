"""
Bridge NL2 telemetry to a microcontroller over USB serial.

Frame format (ASCII, one line per frame):
T,<seq>,<in_play>,<paused>,<braking>,<train_in_station>,<e_stop>,<can_dispatch>,<speed>,<gx>,<gy>,<gz>\n
"""
import argparse
import pathlib
import sys
import time

import serial

sys.path.append(str(pathlib.Path(__file__).absolute().parent))

from nl2telemetry import NoLimits2
from nl2telemetry.message import Answer, get_telemetry
from nl2telemetry.message.reply import TelemetryData, StationStateData
from nl2telemetry.message.request import (
    GetCurrentCoasterAndNearestStationMessage,
    GetStationStateMessage,
)


def parse_args():
    parser = argparse.ArgumentParser(description="Stream NL2 telemetry to USB serial.")
    parser.add_argument("--serial-port", required=True, help="Serial port (e.g. COM5)")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baudrate")
    parser.add_argument("--nl2-host", default="127.0.0.1", help="NL2 telemetry host")
    parser.add_argument("--nl2-port", type=int, default=15151, help="NL2 telemetry port")
    parser.add_argument("--hz", type=float, default=20.0, help="Output frequency")
    return parser.parse_args()


def bool_to_int(value: bool) -> int:
    return 1 if value else 0


def main():
    args = parse_args()
    period_s = 1.0 / max(args.hz, 1.0)

    get_station = GetStationStateMessage()
    get_current_station = GetCurrentCoasterAndNearestStationMessage()
    sequence = 0

    with serial.Serial(args.serial_port, args.baud, timeout=0.2) as ser:
        # Let the MCU CDC port settle after opening.
        time.sleep(2.0)

        with NoLimits2(args.nl2_host, args.nl2_port) as nl2:
            while True:
                cycle_start = time.time()

                nl2.send(get_telemetry)
                telemetry_raw = nl2.receive()
                telemetry = Answer.get_data(telemetry_raw)
                if not isinstance(telemetry, TelemetryData):
                    continue

                coaster_id = 0
                station_id = 0

                nl2.send(get_current_station)
                current_station = Answer.get_data(nl2.receive())
                if hasattr(current_station, "value0") and hasattr(current_station, "value1"):
                    coaster_id = int(current_station.value0)
                    station_id = int(current_station.value1)

                get_station.get_state_for(coaster_id, station_id)
                nl2.send(get_station)
                station = Answer.get_data(nl2.receive())

                train_in_station = 0
                e_stop = 0
                can_dispatch = 0
                if isinstance(station, StationStateData):
                    train_in_station = bool_to_int(station.train_in_station)
                    e_stop = bool_to_int(station.e_stop)
                    can_dispatch = bool_to_int(station.can_dispatch)

                line = (
                    f"T,{sequence},{bool_to_int(telemetry.in_play_mode)},"
                    f"{bool_to_int(telemetry.paused_state)},{bool_to_int(telemetry.braking)},"
                    f"{train_in_station},{e_stop},{can_dispatch},"
                    f"{telemetry.speed:.3f},{telemetry.gforce_x:.3f},"
                    f"{telemetry.gforce_y:.3f},{telemetry.gforce_z:.3f}\n"
                )
                ser.write(line.encode("ascii"))
                sequence += 1

                elapsed = time.time() - cycle_start
                if elapsed < period_s:
                    time.sleep(period_s - elapsed)


if __name__ == "__main__":
    main()
