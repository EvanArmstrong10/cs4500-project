#!/usr/bin/env python

import random
import sys

path = ""

if len(sys.argv) == 2:
    path = sys.argv[1]
else:
    print("Please provide path where datafile should be created")
    exit(0)

f = open(path, "w")


ints = [1, 9, 34, 56, 24, 37, 92, 45, 54, 20, 34, 28, 10, 32, 72, 123]

lines = []
num_unique_lines = 10

# creates 10 unique lines
for i in range(num_unique_lines):
    line = ""
    for j in range(0, 100):
        line += str(ints[random.randint(0, 15)])
        line += " "
    lines.append(line + "\n")

# randomly writes one of the 10 unique lines to file
for i in range(0, 1000000):
    f.write(lines[random.randint(0, num_unique_lines-1)])


f.close()
