#!/usr/bin/env python

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
    " `date` date,"
    " PRIMARY KEY (`id`)"
    ") ENGINE=InnoDB"
)
TABLES['aiaprice'] = (
    "CREATE TABLE `aiaprice` ("
    " `id` int(11) NOT NULL AUTO_INCREMENT,"
    " `code_id` int NOT NULL,"
    " `price` varchar(24) NOT NULL,"
    " `date` date NOT NULL,"
    " PRIMARY KEY (`id`)"
    ") ENGINE=InnoDB"
)


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
            print("Database {} exists!".format(DB_NAME))
        else:
            print("Failed creating database: {}".format(err[1]))
        #exit(1)

if __name__ == "__main__":
    preparedb()

