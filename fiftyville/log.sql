-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Scan through interviews table to get a feel of what is happening
SELECT name
FROM interviews;
ORDER BY interviews.year ASC, interviews.month ASC, interviews.day ASC;
-- too many interviews
-- lets look at the crime scene reports and use that to select whose
-- testimony to scrutinize.
SELECT COUNT(*) FROM crime_scene_reports;
-- still too many
-- let's look for a specific key word then.
SELECT * FROM crime_scene_reports
WHERE description LIKE '%duck%';

-- Let's get reports on that day
SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28

-- I got jaywalking, burglary, shop_lifting, theft, jaywalking
-- Lets Filter interviews by the keywords, theft or steal or %10:
SELECT name as interviewee, transcript FROM interviews
WHERE transcript LIKE '%theft%' OR transcript LIKE '%steal%'
OR transcript LIKE '%10:%';

-- Ruth - about 10:15 +/- 10mins saw the thief drive away in a vehicle
-- I have reckless driving by 20:31 from crime scene reports tho but that too late in the day
-- Get license plate of exit's at 10 and the join on people to get possible suspects name
SELECT people.name, activity, people.license_plate, hour, minute FROM bakery_security_logs
JOIN people ON people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021 AND  month = 7 AND day = 28 AND hour = 10 AND activity LIKE 'exit';

-- Suspects - Vanessa, Bruce, Barry, Luca, Sofia, Iman, Diana, Kelsey, Taylor
-- It's probably Kelsey or Taylor because of their exit time look like what was described by Ruth
-- Let's do a join on vehicle plates, interviews and the bakery security
-- logs, to find the testimonies of the people that left the bakery after the crime.
SELECT p1.name, interviews.transcript, interviews.id
FROM bakery_security_logs AS b_log
JOIN people p1 ON p1.license_plate = b_log.license_plate AND b_log.year = 2021 AND  b_log.month = 7 AND b_log.day = 28 AND b_log.hour = 10 AND b_log.activity = 'exit'
JOIN interviews on interviews.name LIKE p1.name;

-- Bruce =< his friend name unknown
-- Merryweather => Merryweather - A bank director interested in the matter
-- Jones => A good fellow who is bad at his job but quite brave and tenacious
-- Diana => talks to "Marm" talking about bringing someone in
-- Let's check who the suspects called and then get their testimonies
SELECT p1.name caller, p2.name as receiver, pc1.day, pc1.duration
FROM bakery_security_logs AS b_log
JOIN people p1 ON p1.license_plate = b_log.license_plate AND b_log.year = 2021 AND  b_log.month = 7 AND b_log.day = 28 AND b_log.hour = 10 AND b_log.activity = 'exit'
JOIN phone_calls pc1 ON  p1.phone_number = pc1.caller AND pc1.duration < 60
JOIN people p2 ON p2.phone_number = pc1.receiver
ORDER BY pc1.day;

SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;

-- Create a subset of people made a withdrawal on the atm at leggett street
-- And also made a call less than a minute
SELECT people.name FROM atm_transactions a_t
JOIN bank_accounts b1 on b1.account_number = a_t.account_number
JOIN people on people.id = b1.person_id
WHERE a_t.year = 2021 and a_t.month = 7 AND a_t.day = 28
and a_t.transaction_type LIKE 'withdraw' and a_t.atm_location like '%Legg%'
ORDER BY people.name;

-- Get a probable name for the airport
-- origin_id = 8, abbrev CSF, FULL_NAME = Fiftyville, city = fiftyville
SELECT * FROM airports

-- Look for the earliest flight out of the town and get it's id
-- so we can inspect the passengers on the flight.
-- flight id = 36, destination_airport_id = 4;
SELECT * FROM flights
WHERE origin_airport_id = 8 AND day = 29
ORDER BY day, hour
LIMIT 1;


-- Inpect the passenger's list for people who match the id above
-- by doing a join on people and passengers
-- Bruce, Taylor
-- Bruce, because the time of his call is closer to 10 mins.
SELECT name from people
JOIN passengers on passengers.passport_number = people.passport_number AND flight_id = 36;

-- The destination the suspect flew to
SELECT * FROM airports
WHERE id = 4;