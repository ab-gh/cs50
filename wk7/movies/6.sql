SELECT AVG(rating) FROM (SELECT rating, year FROM movies JOIN ratings on movies.id = ratings.movie_id) WHERE year=2012;
