SELECT DISTINCT people.name FROM (SELECT * FROM stars JOIN people ON person_id=people.id WHERE movie_id IN (SELECT movie_id FROM stars JOIN people ON person_id = people.id WHERE name="Kevin Bacon" AND birth=1958) AND NOT people.name="Kevin Bacon") JOIN people ON person_id = people.id;