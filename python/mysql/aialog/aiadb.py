#!/usr/bin/env python
# -*- coding: utf-8; -*- 

import time
import pymysql

DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASS = 'sddba'
DB_NAME = 'aia'
SQL_TIME_FORMAT = '%Y-%m-%d %H:%M:%S'
JSON_TIME_FORMAT = '%a, %d %b %Y %H:%M:%S +0000'

DEBUG = False

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

SELECT_PRICE_DUP = """
SELECT price FROM aiaprice
WHERE code_id = %s AND date = %s
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
    db = pymysql.connect(host=DB_HOST, user=DB_USER,
            passwd=DB_PASS)
    cursor = db.cursor()

    create_database(cursor)
    cursor.close()
    db.close()

    db = pymysql.connect(host=DB_HOST, user=DB_USER,
            passwd=DB_PASS, db=DB_NAME)
    cursor = db.cursor()

    for name, ddl in TABLES.iteritems():
        try:
            cursor.execute(ddl)
        except Exception as e:
            if e[0] == 1050:
                if DEBUG:
                    print("aiadb: already exists.")
            else:
                raise e

    cursor.close()
    db.close()

def create_database(cursor):
    try:
        cursor.execute(
            "CREATE DATABASE {} DEFAULT CHARACTER SET 'utf8'".format(DB_NAME))
    except Exception as e:
        if e[0] == 1007:
            if DEBUG:
                print("aiadb: Database {} exists!".format(DB_NAME))
        else:
            raise e

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
    db = pymysql.connect(host=DB_HOST, user=DB_USER,
            charset='utf8', # phpmyadmin read Thai with this
            passwd=DB_PASS, db=DB_NAME)
    cursor = db.cursor()
    cid = get_code_id(cursor, db, code, name.encode('utf8'))
    if cid == None:
        if DEBUG:
            print 'aiadb: Cannot get cid'
        return False

    t = time.strptime(ts, JSON_TIME_FORMAT)
    tt = time.strftime(SQL_TIME_FORMAT, t)
    cursor.execute(SELECT_PRICE_DUP, (cid, tt))
    row = cursor.fetchone()
    if row != None:
        if DEBUG:
            print 'aiadb: There is redundant information', cid, tt
        return False
    cursor.execute(SELECT_PRICE, (cid, ))
    row = cursor.fetchone()
    if row == None:
        if DEBUG:
            print 'aiadb: There is no price of code_id', cid
        return False
    if row[0] == price:
        if DEBUG:
            print 'aiadb: There is no price change', price
        return True
    cursor.execute(INSERT_PRICE, (cid, price, tt))
    db.commit()
    cursor.close()
    db.close()
    return True

if __name__ == "__main__":
    preparedb()

