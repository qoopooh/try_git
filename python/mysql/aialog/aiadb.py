#!/usr/bin/env python

import time
import MySQLdb
from _mysql_exceptions import DatabaseError, OperationalError

DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASS = 'sddba'
DB_NAME = 'test'

TABLES = {}
TABLES['aiacode'] = (
    "CREATE TABLE `aiacode` ("
    " `id` int NOT NULL AUTO_INCREMENT,"
    " `code` varchar(32) NOT NULL,"
    " `name` varchar(96) NOT NULL,"
    " `date` date,"
    " PRIMARY KEY (`id`)"
    ") ENGINE=InnoDB DEFAULT CHARSET=UTF8;"
)
TABLES['aiaprice'] = (
    "CREATE TABLE `aiaprice` ("
    " `id` int(11) NOT NULL AUTO_INCREMENT,"
    " `code_id` int NOT NULL,"
    " `price` varchar(24) NOT NULL,"
    " `date` date NOT NULL,"
    " PRIMARY KEY (`id`)"
    ") ENGINE=InnoDB DEFAULT CHARSET=UTF8;"
)


SELECT_CID = """
SELECT id FROM aiacode
WHERE code = %s OR name = %s
"""

INSERT_CID = "INSERT INTO aiacode (`code`, `name`, `date`) VALUES (%s, %s, %s)"

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
    now = time.strftime('%Y-%m-%d %H:%M:%S')
    if row == None:
        c.execute(INSERT_CID, (code, name, now))
        db.commit()
        c.execute(SELECT_CID, (code, name))
        row = c.fetchone()
        return row[0]
    return row[0]

def update_price(code, name, price, time):
    db = MySQLdb.connect(host=DB_HOST, user=DB_USER,
            passwd=DB_PASS, db=DB_NAME)
    cursor = db.cursor()
    cid = get_code_id(cursor, db, code, name.encode('utf8'))
    if cid == None:
        return False
    print cid
    cursor.close()
    db.close()

if __name__ == "__main__":
    preparedb()

