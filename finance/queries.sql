CREATE table shares
(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
user_id INTEGER  NOT NULL,
stock_id INTEGER NOT NULL,
count INTEGER NOT NULL,
FOREIGN KEY (user_id) REFERENCES users(id),
FOREIGN KEY (stock_id) REFERENCEs stocks(id)
);

CREATE table stocks(
id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL DEFAULT 0,
name TEXT NOT NULL UNIQUE
);
SELECT shares.id
FROM shares
JOIN users ON users.id = shares.user_id AND shares.user_id = 1
JOIN stocks ON stocks.id = shares.stock_id AND shares.stock_id = 1;


CREATE TABLE users(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, hash TEXT not NULL, cash NUMERIC not NULL DEFAULT 10000.00);
CREATE TABLE sqlite_sequence(name,seq);


CREATE TABLE transactions
(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
user_id INTEGER NOT NULL,
stock_id INTEGER NOT NULL,
share_id INTEGER NOT NULL,
transaction_type TEXT NOT NULL,
count INTEGER NOT NULL DEFAULT 0,
FOREIGN KEY (user_id) REFERENCES users(id),
FOREIGN KEY (stock_id) REFERENCEs stocks(id),
FOREIGN KEY (share_id) REFERENCES shares(id));


/*Stocks owned */
SELECT stocks.name as stock_name, shares.count as share_count, users.cash as users_balance, users.username as user_name FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = 1 JOIN stocks on stocks.id = shares.stock_id

SELECT stocks.name as stock_name, shares.count as share_count FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = 1 JOIN stocks on stocks.id = shares.stock_id;
SELECT stocks.id as stock_id, shares.id as share_id FROM shares JOIN users on users.id = shares.user_id JOIN stocks on stocks.id = shares.stock_id  AND stocks.name = 'nflx';

SELECT stocks.name as stock_name, shares.count as share_count FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = 1 JOIN stocks on stocks.id = shares.stock_id

SELECT t.transaction_type, stocks.name, t.count FROM transactions as t JOIN stocks on stocks.id = t.stock_id and t.user_id = 1;

ALTER TABLE transactions
ADD COLUMN transaction_time DATETIME;


CREATE TRIGGER update_transaction_time
AFTER INSERT ON transactions
BEGIN
    UPDATE transactions
    SET transaction_time = CURRENT_TIMESTAMP
    WHERE id = NEW.id;
END;
ALTER TABLE transactions
ADD COLUMN transaction_value NUMERIC DEFAULT 0.00;



SELECT stocks.name as stock_name, shares.count as share_count, users.cash as user_balance, users.username as user_name FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = ? JOIN stocks on stocks.id = shares.stock_id AND shares.count > 0
SELECT stocks.name as stock_name, shares.count as share_count, users.cash as user_balance, users.username as user_name FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = 2 JOIN stocks on stocks.id = shares.stock_id;
