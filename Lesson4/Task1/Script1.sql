

drop table if exists tracks cascade;
drop table if exists tracks_digests cascade;
drop table if exists albums cascade;
drop table if exists artists_albums cascade;
drop table if exists artists_genres cascade;



drop table if exists artist cascade;


drop table if exists genre cascade;
drop table if exists digest cascade;



CREATE TABLE IF NOT EXISTS Artist(id INTEGER PRIMARY KEY NOT NULL,
name VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS Albums(
id INTEGER PRIMARY KEY NOT NULL,
name VARCHAR(40),
year INTEGER
);

CREATE TABLE IF NOT EXISTS Genre(
id INTEGER PRIMARY KEY NOT NULL,
name VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS Tracks(
id INTEGER PRIMARY KEY NOT NULL,
album_id INTEGER REFERENCES Albums(id),
duration TIME,
name VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS Artists_genres(
id INTEGER PRIMARY KEY NOT NULL,
artist_id INTEGER REFERENCES Artist(id),
genre_id INTEGER REFERENCES Genre(id)
);

CREATE TABLE IF NOT EXISTS Artists_albums(
id INTEGER PRIMARY KEY NOT NULL,
artist_id INTEGER REFERENCES Artist(id),
album_id INTEGER REFERENCES Albums(id)
);

CREATE TABLE IF NOT EXISTS Digest(
id INTEGER PRIMARY KEY NOT NULL,
release_year INTEGER,
name VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS Tracks_digests(
id INTEGER PRIMARY KEY NOT NULL,
track_id INTEGER REFERENCES Tracks(id),
digest_id INTEGER REFERENCES Digest(id)
);


