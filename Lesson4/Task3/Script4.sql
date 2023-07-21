--Задание 3. Пункт 1
select g.name, count(ag.artist_id) from genre g
inner join artists_genres ag on g.id=ag.genre_id
inner join artist a on a.id=ag.artist_id 
group by g.name;

--Задание 3. Пункт 2
select count(t.id) from tracks t
inner join albums a on t.album_id=a.id 
where a.year between 2019 and 2020;

--Задание 3. Пункт 3
select a.name, avg(t.duration) from albums a
inner join tracks t on t.album_id=a.id 
group by a.name;

--Задание 3. Пункт 4
select name from albums
where year != 2020
order by year;

--Задание 3. Пункт 5
select d.name from digest d
inner join tracks_digests td on td.digest_id=d.id 
inner join tracks t on td.track_id=t.id 
inner join artists_albums aa on aa.album_id=t.album_id 
inner join artist a on a.id=aa.artist_id 
where a.name='Netsky'
group by d.name;



