-- get users
select * from users;

-- create user
INSERT INTO users(name, password) 
VALUES 
('john.doe@example.com', 'securePass123');

-- get user by id
select * from users where id = 1;

-- get by name
select * from users where name='jane.smith@example.com'

--  extract password
select password from users where name = 'jane.smith@example.com';


