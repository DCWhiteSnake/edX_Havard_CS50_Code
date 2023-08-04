SELECT people.name
FROM stars
JOIN movies on movies.id = stars.movie_id
JOIN people on stars.person_id = people.id
WHERE movies.title = 'Toy Story';