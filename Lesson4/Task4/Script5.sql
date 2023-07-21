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
