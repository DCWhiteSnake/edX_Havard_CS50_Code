SELECT SUM(songs.energy)/COUNT(songs.energy)
FROM artists
JOIN songs on artists.id = songs.artist_id
WHERE artists.name LIKE 'drake';