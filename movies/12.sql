SELECT movies.title FROM movies
JOIN stars s1 ON s1.movie_id = movies.id
JOIN people p1 on p1.id = s1.person_id AND p1.name LIKE 'Johnny Depp'
JOIN stars s2 ON s2.movie_id = movies.id
JOIN people p2 on p2.id = s2.person_id AND p2.name LIKE 'Helena Bonham Carter';