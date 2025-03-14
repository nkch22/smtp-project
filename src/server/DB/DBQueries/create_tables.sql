drop table if exists messages, users;

create table users (
	id serial primary key,
	name varchar(30) unique,
	password varchar(20) not null check(length(password)>=8)
);

create table messages (
	id serial primary key,
	sender integer references users(id),
	recepient integer references users(id),
--	subject varchar(50),
	content varchar(500)
--	date_time timestamp
);
