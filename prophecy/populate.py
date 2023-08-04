import csv
from cs50 import SQL

def main():
    db = SQL("sqlite:///roster.db")
    with  open('students.csv') as csv_file:
        reader = csv.DictReader(csv_file)
        for row in reader:
            student_name = row["student_name"]
            house = row["house"]
            head = row["head"]
            try:
                h_id = db.execute('INSERT INTO house (house_name, head) VALUES(?, ?)', house, head)
            except ValueError:
                pass
            s_id = db.execute('INSERT INTO students (student_name) VALUES(?)', student_name)
            db.execute('INSERT INTO house_assignments (s_id, h_id) VALUES(?, ?)', s_id, h_id)

main()