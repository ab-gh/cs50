import csv
import sys
import re


def pattern_detect(string, sub):
    running_pattern_count = 0
    pattern_max = 0
    i = 0
    while i < len(string):
        if string[i:i+len(sub)] == sub:
            running_pattern_count += 1
            i += len(sub)-1
        elif string[i:i+len(sub)] != sub:
            if running_pattern_count > pattern_max:
                pattern_max = running_pattern_count
            running_pattern_count = 0
        i += 1
    return pattern_max
    

seq = open(sys.argv[2], "r").read()
counts = {}
db = {}
suspects = []

with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    rows = list(csv_reader)
    for str_find in rows[0][1:]:
        counts[str_find] = pattern_detect(seq, str_find)
    for row in rows[1:]:
        db[row[0]] = {}
        for column, i in enumerate(row[1:]):
            db[row[0]][rows[0][column+1]] = i
    line_count = 0

for suspect in db:
    suspects.append(suspect)

for suspect in db:
    for match in db[suspect]:
        if int(db[suspect][match]) != int(counts[match]):
            suspects.remove(suspect)
            break


if suspects:
    for i in suspects:
        print(i)
else:
    print("No match")