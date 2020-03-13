#!/usr/bin/python

import random

f = open("datafile.txt", "w+")


ints = ['<1>', '<9>', '<34>', '<56>', '<24>', '<37>', '<92>', '<45>', '<54>', '<20>', '<34>', '<28>', '<10>', '<32>', '<72>', '<123>']

line = ""
lines = []

for x in range(0, 10):

    for i in range(0, 10):
        line += ints[random.randint(0, 15)]
        line += " "
    lines.append(line)
    line = ""

for i in range(0, 10):
    f.write(lines[random.randint(0, 9)] + "\r")

f.close()