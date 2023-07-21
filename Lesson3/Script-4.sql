select name, year from albums
where year=2018;
select name, duration from tracks
order by duration desc
LIMIT 1;
select name from tracks
where duration >= '3:30';
select name from digest
where release_year >= 2018 and release_year <= 2020;
select name from artist
where name not like '% %';
select name from tracks
where name like '%My%';