import csv
import sys
import re


seq = open(sys.argv[2], "r").read()

counts = {}
db = {}
suspects = []

with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    rows = list(csv_reader)
    for str_find in rows[0][1:]:
        counts[str_find] = seq.count(str_find)
    for row in rows[1:]:
        db[row[0]] = {}
        for column, i in enumerate(row[1:]):
            db[row[0]][rows[0][column+1]] = i
    line_count = 0

for suspect in db:
    suspects.append(suspect)
print("counts")
print(counts)
print("db")
for i in db:
    print(db[i])
for suspect in db:
    for match in db[suspect]:
        if int(db[suspect][match]) != int(counts[match]):
            suspects.remove(suspect)
            break
        else:
            print(suspect)


#if suspects:
#    for i in suspects:
#        print(i)
#else:
#    print("No match")


