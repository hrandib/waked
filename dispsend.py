# This Python file uses the following encoding: utf-8

# if __name__ == "__main__":
#     pass

import subprocess
import sys
import os
import time

def get_uptime():
    with open('/proc/uptime', 'r') as f:
        uptime_seconds = float(f.readline().split()[0])

    return int(uptime_seconds)

intervals = (
    ('M', 2592000),  # 60 * 60 * 24 * 30
    ('d', 86400),    # 60 * 60 * 24
    ('h', 3600),    # 60 * 60
    ('m', 60),
    ('s', 1),
)

def display_time(seconds, granularity=2):
    result = []

    for name, count in intervals:
        value = seconds // count
        if value:
            seconds -= value * count
            result.append("{}{}".format(value, name))
    return ' '.join(result[:granularity])

prefix = "./waked -a 127 -c 47"
while True:
    line1 = str.format("{:.2f} {:.2f} {:.2f}", *os.getloadavg()).rjust(16)
    line2 = (display_time(get_uptime(), 4)).ljust(16)

    out_chars = list(line1 + line2)
    out_codes = list(map(ord, out_chars))
    out_codes.insert(0, 0)
    out_codes = list(map(str, out_codes))
    result = subprocess.run(prefix.split() + out_codes, capture_output=True, text=True)
    time.sleep(1)

