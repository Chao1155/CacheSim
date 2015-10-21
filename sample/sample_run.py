#!/usr/bin/python

# t means type, including 0 basic cache, 1 racetrack memory cache.
# b means the bank number
# m means the bank index move left number.

import os
cmd = "../cacheSim -i ./sample_trace.input -s 4M -w 8 -t 0 -b 4 -m 4 > ./sample_trace.output"
os.system(cmd)
