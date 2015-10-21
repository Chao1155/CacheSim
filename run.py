#!/usr/bin/python

# t means type, including 0 basic cache, 1 racetrack memory cache.
# b means the bank number
# m means the bank index move left number.

import os
#cmd = "./cacheSim -i inputs/small_test.txt -s 4M -w 8 -t 0 -b 4 -m 0"
#cmd = "./cacheSim -i inputs/medium_test.txt -s 4M -w 8 -t 0 -b 4 -m 0"
cmd = "./cacheSim -i inputs/trace_l2bus2l2.txt -s 4M -w 8 -t 0 -b 4 -m 4 > results/bank4move4.txt"
os.system(cmd)
#cmd = "./cacheSim -i inputs/medium_test.txt -s 4M -w 8 -t 0"
#cmd = "./cacheSim -i inputs/trace_l2bus2l2.txt.bak -s 4M -w 8 -t 0"
#cmd = "./cacheSim -i inputs/trace_l2bus2l2.txt -s 4M -w 8 -t 0 -b 4"
#print cmd
# 
