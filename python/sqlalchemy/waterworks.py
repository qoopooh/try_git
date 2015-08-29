#!/usr/bin/env python
# -*- coding: utf-8 -*-

from sqlalchemy import create_engine, Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

engine = create_engine('sqlite:///:memory:', echo=True)
Base = declarative_base()

class Employee(Base):
    __tablename__ = 'Employee'

    Empid = Column(String(5), primary_key=True)
    Cardno = Column(String(17))
    Prefixid = Column(Integer)          # doc: Prefixed
    Fullname = Column(String(50))
    Address = Column(String(50))
    Tel = Column(String(50))
    Posid = Column(Integer)             # doc: String 3
    Username = Column(String(30))
    Password = Column(String(30))

    def __repr__(self):
        """Optinal"""
        return "<Employee(name='{0}', fullname='{1}')>".format(self.Empid, self.Fullname.encode('utf8'))

#class WaterMeter(Base):
    #__tablename__ = 'Water_meter'

#class Prefix(Base):
    #__tablename__ = 'Prefix'

#class Positions(Base):
    #__tablename__ = 'Positions'

#class Customers(Base):
    #__tablename__ = 'Customers'

#class Payment(Base):
    #__tablename__ = 'Payment'

#class WaterUse(Base):
    #__tablename__ = 'Water_use'

#class Extensions(Base):
    #__tablename__ = 'Extensions'

#class Billnotpay(Base):
    #__tablename__ = 'Billnotpay'

#class Bill(Base):
    #__tablename__ = 'Bill'

#class Uninstall(Base):
    #__tablename__ = 'Uninstall'

Base.metadata.create_all(engine) 

Session = sessionmaker()
Session.configure(bind=engine)
session = Session()

pranee = Employee(Empid='0003', Cardno='1559900155611', Prefixid=1, Fullname=u'ปรานี ดีมา', Address=u'12/4 หมู่ 8 อ.เมือง จ.น่าน', Tel='0837628281', Posid=0, Username='Pranee', Password='222599')
manop = Employee(Empid='0005', Cardno='1559900155613', Prefixid=0, Fullname=u'มานพ มาดี', Address=u'15/3 หมู่ 7 อ.เมือง จ.น่าน', Tel='0861908633', Posid=1, Username='Manop', Password='333599')
teera = Employee(Empid='0006', Posid=2, Username='Teera', Password='123456')

session.add(pranee)
session.commit()
session.add(manop)
session.commit()
session.add(teera)
session.commit()

our_user = session.query(Employee).filter_by(Username='Pranee').first()
print our_user

