CREATE TABLE house (
    id INTEGER PRIMARY KEY,
    house_name TEXT UNIQUE,
    head TEXT UNIQUE
);

CREATE TABLE students(
    id INTEGER PRIMARY KEY,
    student_name TEXT
);

CREATE TABLE house_assignments(
    id INTEGER PRIMARY KEY,
    s_id INTEGER,
    h_id INTEGER,
    FOREIGN KEY (s_id) REFERENCES students(id)
    FOREIGN KEY (h_id) REFERENCES house(id)
);

'INSERT INTO house (house_name, head) VALUES(?, ?)', house, head
'INSERT INTO students (student_name) VALUES(?, ?)', student_name
'INSERT INTO house_assignments (s_id, h_id) VALUES(?, ?)', s_id, h_id

"Implicit join"
SELECT student_name, house_name
FROM house, students, house_assignments
WHERE students.id = house_assignments.s_id
AND house.id = house_assignments.h_id
LIMIT 5;

"Explicit join"
SELECT students.student_name, house.house_name
FROM house_assignments
JOIN house on house_assignments.h_id = house.id
JOIN students on house_assignments.s_id = students.id;