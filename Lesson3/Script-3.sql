DELETE FROM artists_genres;
DELETE FROM artists_albums;
DELETE FROM tracks_digests;
DELETE FROM genre;
DELETE FROM artist;
DELETE FROM tracks;
DELETE FROM albums;
DELETE FROM digest;

INSERT INTO artist
VALUES(1, 'Pendulum');
INSERT INTO artist
VALUES(2, 'Chase & Status');
INSERT INTO artist
VALUES(3, 'Netsky');
INSERT INTO artist
VALUES(4, 'Sub Focus');
INSERT INTO artist
VALUES(5, 'Noisia');
INSERT INTO artist
VALUES(6, 'DJ Fresh');
INSERT INTO artist
VALUES(7, 'High Contrast');
INSERT INTO artist
VALUES(8, 'Black Sun Empire');


INSERT INTO genre
VALUES(1, 'Drum''n''Bass');
INSERT INTO genre
VALUES(2, 'Trance');
INSERT INTO genre
VALUES(3, 'Techno');
INSERT INTO genre
VALUES(4, 'House');
INSERT INTO genre
VALUES(5, 'Break Beat');


INSERT INTO albums
VALUES(1, 'Immersion', 2010);
INSERT INTO albums
VALUES(2, 'Hold Your Color', 2005);
INSERT INTO albums
VALUES(3, 'In Silico', 2008);
INSERT INTO albums
VALUES(4, 'No More Idols', 2011);
INSERT INTO albums
VALUES(5, 'Split The Atom', 2018);
INSERT INTO albums
VALUES(6, 'Home', 2013);
INSERT INTO albums
VALUES(7, 'Join The Q', 2018);
INSERT INTO albums
VALUES(8, 'Cross The Line', 2011);


INSERT INTO tracks
VALUES(1, 4, '1:09', 'Genesis');
INSERT INTO tracks
VALUES(2, 7, '6:39', 'Salt in the Wounds');
INSERT INTO tracks
VALUES(3, 5, '5:02', 'Set Me On Fire');
INSERT INTO tracks
VALUES(4, 1, '4:13', 'Crush');
INSERT INTO tracks
VALUES(5, 2, '4:55', 'Under the Waves');
INSERT INTO tracks
VALUES(6, 4, '5:20', 'The Island - Pt. I (Dawn)');
INSERT INTO tracks
VALUES(7, 8, '4:09', 'The Island - Pt. II');
INSERT INTO tracks
VALUES(8, 6, '2:48', 'Comprachicos');
INSERT INTO tracks
VALUES(9, 7, '4:03', 'The Vulture');
INSERT INTO tracks
VALUES(10, 1, '5:21', 'My Encoder');
INSERT INTO tracks
VALUES(11, 3, '3:18', 'Machine Gun');
INSERT INTO tracks
VALUES(12, 5, '3:45', 'Thursday');
INSERT INTO tracks
VALUES(13, 2, '5:58', 'Hand Gestures');
INSERT INTO tracks
VALUES(14, 8, '3:27', 'My Red Heat');
INSERT INTO tracks
VALUES(15, 3, '3:50', 'Shellshock');

INSERT INTO digest 
VALUES(1, 2020, 'Soundsystem');
INSERT INTO digest 
VALUES(2, 2005, 'Reality Checkpoint');
INSERT INTO digest 
VALUES(3, 2015, 'Stronger');
INSERT INTO digest 
VALUES(4, 2007, 'Martian Arts');
INSERT INTO digest 
VALUES(5, 2018, 'It''s A Gee Thing');
INSERT INTO digest 
VALUES(6, 2015, 'Drop');
INSERT INTO digest 
VALUES(7, 2019, 'Power Ballads');
INSERT INTO digest 
VALUES(8, 2013, 'Draining Love Story');

INSERT INTO artists_albums 
VALUES(1, 1, 3);
INSERT INTO artists_albums 
VALUES(2, 2, 8);
INSERT INTO artists_albums 
VALUES(3, 3, 6);
INSERT INTO artists_albums 
VALUES(4, 4, 2);
INSERT INTO artists_albums 
VALUES(5, 5, 3);
INSERT INTO artists_albums 
VALUES(6, 2, 6);
INSERT INTO artists_albums 
VALUES(7, 1, 4);
INSERT INTO artists_albums 
VALUES(8, 2, 2);
INSERT INTO artists_albums 
VALUES(9, 6, 1);
INSERT INTO artists_albums 
VALUES(10, 6, 8);
INSERT INTO artists_albums 
VALUES(11, 7, 3);
INSERT INTO artists_albums 
VALUES(12, 7, 5);
INSERT INTO artists_albums 
VALUES(13, 8, 2);
INSERT INTO artists_albums 
VALUES(14, 8, 8);

INSERT INTO artists_genres 
VALUES(1, 1, 3);
INSERT INTO artists_genres 
VALUES(2, 2, 5);
INSERT INTO artists_genres 
VALUES(3, 3, 4);
INSERT INTO artists_genres 
VALUES(4, 4, 2);
INSERT INTO artists_genres 
VALUES(5, 5, 3);
INSERT INTO artists_genres 
VALUES(6, 2, 1);
INSERT INTO artists_genres 
VALUES(7, 1, 5);
INSERT INTO artists_genres 
VALUES(8, 2, 2);
INSERT INTO artists_genres 
VALUES(9, 6, 1);
INSERT INTO artists_genres 
VALUES(10, 6, 3);
INSERT INTO artists_genres 
VALUES(11, 7, 3);
INSERT INTO artists_genres 
VALUES(12, 7, 5);
INSERT INTO artists_genres 
VALUES(13, 8, 2);
INSERT INTO artists_genres 
VALUES(14, 8, 1);

INSERT INTO tracks_digests 
VALUES(1, 1, 3);
INSERT INTO tracks_digests 
VALUES(2, 2, 5);
INSERT INTO tracks_digests 
VALUES(3, 15, 4);
INSERT INTO tracks_digests 
VALUES(4, 4, 2);
INSERT INTO tracks_digests 
VALUES(5, 5, 8);
INSERT INTO tracks_digests 
VALUES(6, 2, 1);
INSERT INTO tracks_digests 
VALUES(7, 1, 5);
INSERT INTO tracks_digests 
VALUES(8, 2, 2);
INSERT INTO tracks_digests 
VALUES(9, 14, 1);
INSERT INTO tracks_digests 
VALUES(10, 6, 3);
INSERT INTO tracks_digests 
VALUES(11, 7, 3);
INSERT INTO tracks_digests 
VALUES(12, 7, 7);
INSERT INTO tracks_digests 
VALUES(13, 8, 2);
INSERT INTO tracks_digests 
VALUES(14, 9, 1);
INSERT INTO tracks_digests 
VALUES(15, 11, 3);
INSERT INTO tracks_digests 
VALUES(16, 2, 7);
INSERT INTO tracks_digests 
VALUES(17, 7, 4);
INSERT INTO tracks_digests 
VALUES(18, 4, 2);
INSERT INTO tracks_digests 
VALUES(19, 15, 3);
INSERT INTO tracks_digests 
VALUES(20, 2, 1);
INSERT INTO tracks_digests 
VALUES(21, 9, 5);
INSERT INTO tracks_digests 
VALUES(22, 2, 2);
INSERT INTO tracks_digests 
VALUES(23, 6, 7);
INSERT INTO tracks_digests
VALUES(24, 9, 3);
INSERT INTO tracks_digests 
VALUES(25, 7, 3);
INSERT INTO tracks_digests 
VALUES(26, 7, 5);
INSERT INTO tracks_digests 
VALUES(27, 8, 2);
INSERT INTO tracks_digests 
VALUES(28, 8, 1);