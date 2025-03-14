drop table if exists messages, users;

create table users
(
    id       serial primary key,
    name     varchar(30) unique,
    password varchar(20) not null check (length(password) >= 8)
);

create table messages
(
    id        serial primary key,
    sender    integer references users (id),
    recipient integer references users (id),
    subject   varchar(50),
    content   varchar(500),
    date_time timestamp
);

-- Inserting dummy data into the users table
INSERT INTO users (name, password)
VALUES ('john_doe', 'password123'),
       ('alice_smith', 'mysecret123'),
       ('bob_jones', 'securepass456'),
       ('carol_white', 'strongpass789'),
       ('david_black', 'topsecret999');

-- Inserting dummy data into the messages table
INSERT INTO messages (sender, recipient, subject, content, date_time)
VALUES (1, 2, 'Hello Alice', 'Hey Alice, how are you?', '2025-03-14 10:00:00'),
       (2, 1, 'Re: Hello Alice', 'I am good, thanks! How about you?', '2025-03-14 10:05:00'),
       (3, 4, 'Meeting Reminder', 'Just a reminder about our meeting at 3 PM today.', '2025-03-14 09:00:00'),
       (4, 3, 'Re: Meeting Reminder', 'Got it, I will be there on time.', '2025-03-14 09:15:00'),
       (5, 2, 'Project Update', 'Here is the latest update on the project...', '2025-03-13 15:00:00'),
       (2, 5, 'Re: Project Update', 'Thanks for the update! I will review it and get back to you.',
        '2025-03-13 15:05:00');
