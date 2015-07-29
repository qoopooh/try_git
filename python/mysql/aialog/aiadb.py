#!/usr/bin/env python

import time
import MySQLdb
from _mysql_exceptions import DatabaseError, OperationalError

DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASS = 'sddba'
DB_NAME = 'test'
SQL_TIME_FORMAT = '%Y-%m-%d %H:%M:%S'
JSON_TIME_FORMAT = '%a, %d %b %Y %H:%M:%S +0000'

TABLES = {}
TABLES['aiacode'] = (
    "CREATE TABLE `aiacode` ("
    " `id` int NOT NULL AUTO_INCREMENT,"
    " `code` varchar(32) NOT NULL,"
    " `name` text,"
    " `date` datetime,"
    " PRIMARY KEY (`id`)"
    ") ENGINE=InnoDB DEFAULT CHARSET=UTF8;"
)
TABLES['aiaprice'] = (
    "CREATE TABLE `aiaprice` ("
    " `id` int(11) NOT NULL AUTO_INCREMENT,"
    " `code_id` int NOT NULL,"
    " `price` varchar(24) NOT NULL,"
    " `date` datetime NOT NULL,"
    " UNIQUE KEY (`id`)"
    ") ENGINE=InnoDB DEFAULT CHARSET=UTF8;"
)


SELECT_CID = """
SELECT id FROM aiacode
WHERE code = %s OR name = %s
"""

SELECT_PRICE = """
SELECT price FROM aiaprice
WHERE code_id = %s
ORDER BY date DESC
LIMIT 1;
"""

INSERT_CID = "INSERT INTO aiacode (`code`, `name`, `date`) VALUES (%s, %s, %s)"
INSERT_PRICE = "INSERT INTO aiaprice (`code_id`, `price`, `date`) VALUES (%s, %s, %s)"

cursor = None

def preparedb():
    db = MySQLdb.connect(host=DB_HOST, user=DB_USER,
            passwd=DB_PASS)
    cursor = db.cursor()

    create_database(cursor)
    cursor.close()
    db.close()

    db = MySQLdb.connect(host=DB_HOST, user=DB_USER,
            passwd=DB_PASS, db=DB_NAME)
    cursor = db.cursor()

    for name, ddl in TABLES.iteritems():
        try:
            print "Creating table {}: ".format(name),
            cursor.execute(ddl)
        except OperationalError as err:
            if err[0] == 1050:
                print("already exists.")
            else:
                print(err[1])
        else:
            print("OK")

    cursor.close()
    db.close()

def create_database(cursor):
    try:
        cursor.execute(
            "CREATE DATABASE {} DEFAULT CHARACTER SET 'utf8'".format(DB_NAME))
    except DatabaseError as err:
        if err[0] == 1007:
            #print("Database {} exists!".format(DB_NAME))
            pass
        else:
            print("Failed creating database: {}".format(err[1]))
        #exit(1)

def get_code_id(c, db, code, name=None):
    c.execute(SELECT_CID, (code, name))
    row = c.fetchone()
    now = time.strftime(SQL_TIME_FORMAT)
    if row == None:
        c.execute(INSERT_CID, (code, name, now))
        db.commit()
        c.execute(SELECT_CID, (code, name))
        row = c.fetchone()
        return row[0]
    return row[0]

def update_price(code, name, price, ts):
    db = MySQLdb.connect(host=DB_HOST, user=DB_USER,
            passwd=DB_PASS, db=DB_NAME)
    cursor = db.cursor()
    cid = get_code_id(cursor, db, code, name.encode('utf8'))
    if cid == None:
        return False

    t = time.strptime(ts, JSON_TIME_FORMAT)
    tt = time.strftime(SQL_TIME_FORMAT, t)
    cursor.execute(SELECT_PRICE, (cid, ))
    row = cursor.fetchone()
    if row != None or row[0] == price:
        return False
    cursor.execute(INSERT_PRICE, (cid, price, tt))
    db.commit()
    cursor.close()
    db.close()
    return True

if __name__ == "__main__":
    preparedb()

