SELECT title, movies.year
FROM movies
WHERE title LIKE 'Harry Potter%'
ORDER BY movies.year;