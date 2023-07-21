CREATE TABLE IF NOT EXISTS Employee(
id INTEGER NOT NULL,
name VARCHAR(40),
department VARCHAR(40),
boss_id integer references Employee(id),
primary key(id, boss_id)
);