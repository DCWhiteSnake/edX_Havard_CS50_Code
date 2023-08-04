SELECT DISTINCT(p2.name) FROM stars s1
JOIN people p1 ON p1.id = s1.person_id AND p1.name LIKE 'Kevin Bacon' AND p1.birth = 1958
JOIN movies m1 ON m1.id = s1.movie_id
JOIN stars s2 ON m1.id = s2.movie_id
JOIN people p2 ON p2.id = s2.person_id WHERE p2.name NOT LIKE 'Kevin Bacon';