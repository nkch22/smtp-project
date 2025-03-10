-- get all messages
select * from messages;

-- add message to db
INSERT INTO messages(sender, recepient, content) 
VALUES 
(1, 2, 'Meeting Reminder. Don’t forget about our meeting tomorrow at 10 AM.')

-- get messages from
select recepient, subject, body, date_time from messages where sender = 2;

-- get messages to
select sender, subject, body, date_time from messages where recepient = 3;

