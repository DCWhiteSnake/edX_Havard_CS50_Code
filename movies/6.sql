SELECT SUM(ratings.rating)/COUNT(*) from movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2012;