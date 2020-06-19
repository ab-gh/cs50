import csv
import cs50
import sys


db = cs50.SQL("sqlite:///students.db")

if len(sys.argv) != 2:
    print("Please provide a hosue to select")
    sys.exit()
    
house = sys.argv[1].capitalize()

results = db.execute("SELECT * FROM students WHERE house=(?) ORDER BY last, first", house)

for student in results:
    if student["middle"] == None:
        mname = ""
    else:
        mname = student["middle"] + " "
    fname = student["first"] + " "
    lname = student["last"]
    print(fname + mname + lname + ", born " + str(student["birth"]))