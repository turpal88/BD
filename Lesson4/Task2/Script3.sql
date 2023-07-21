--Задание 2. Пункт 1
select name, duration from tracks
where duration=(select max(duration) from tracks);

--Задание 2. Пункт 2
select name from tracks
where duration >= '3:30';

--Задание 2. Пункт 3
select name from digest
where release_year between 2018 and 2020
order by release_year;

--Задание 2. Пункт 4
select name from artist
where name not like '% %';

--Задание 2. Пункт 5
select name from tracks
where name like '%My%';

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


--Задание 4. Пункт 1
--сделал вывод исполнителей, исполняющих только в одном жанре. Прошу подсказать как сделать чтобы нормально выводились те исполнители которые исполняют в более чем одном жанре
select al.name, aral.artist_id from albums al
inner join artists_albums aral on al.id=aral.album_id
inner join artists_genres arg on aral.artist_id=arg.artist_id
group by al.name, aral.artist_id
having count(aral.artist_id) = 1; 

--Задание 4. Пункт 2
select t.name from tracks t
left join tracks_digests td on t.id=td.track_id
where td.track_id is not null
group by t.name;

--Задание 4. Пункт 3
--не уверен что это нормальное решение
select ar.name, t.duration from artist ar
inner join artists_albums aa on ar.id=aa.artist_id 
inner join tracks t on t.album_id=aa.artist_id 
group by ar.name, t.duration
having min(t.duration)=(select min(duration) from tracks);

--Задание 4. Пункт 4
--По этому заданию прошу подсказать, не имею представления как оно может быть реализовано
select a.name from albums a
inner join tracks t on t.album_id=a.id 
group by a.name
having count(t.id)=
(select min(tcount) from
(select count(id) tcount from tracks
group by tracks.album_id) as group_t
)