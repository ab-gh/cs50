import csv
import cs50
import sys

open("students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")
db.execute("CREATE TABLE students (id INTEGER PRIMARY KEY AUTOINCREMENT, first VARCHAR(255), middle VARCHAR(255), last VARCHAR(255), house VARCHAR(10), birth INTEGER);")
student_id = 0

if len(sys.argv) != 2:
    print("Please provide a CSV file to import")
    sys.exit()

with open(sys.argv[1], "r") as csv_file:
    reader = csv.reader(csv_file)
    next(reader)
    for person in reader:
        names = person[0].split(" ")
        fname = names[0]
        lname = names[len(names) - 1]
        mname = names[1] if len(names) == 3 else None
        print(person)
        db.execute("INSERT INTO students (id, first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?, ?)", 
                   student_id, fname, mname, lname, person[1], person[2])
        student_id += 1
