#!/usr/bin/python

import random

f = open("datafile.txt", "w+")


ints = [1, 9, 34, 56, 24, 37, 92, 45, 54, 20, 34, 28, 10, 32, 72, 123]

line = ""
for i in range(0, 100):
    line += str(ints[random.randint(0, 15)])
    line += " "

for i in range(0, 1000000):
    f.write(line + "\r")

f.close()