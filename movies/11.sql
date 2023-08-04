SELECT movies.title FROM stars
JOIN people ON people.id = stars.person_id
JOIN movies ON movies.id = stars.movie_id
JOIN ratings ON ratings.movie_id = movies.id
WHERE people.name LIKE 'Chadwick Boseman'
ORDER BY ratings.rating DESC
LIMIT 5;