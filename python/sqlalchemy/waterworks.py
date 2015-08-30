#!/usr/bin/env python
# -*- coding: utf-8 -*-

from sqlalchemy import create_engine, Column, Integer, String, Date, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship, backref

engine = create_engine('sqlite:///:memory:', echo=False)
Base = declarative_base()

class Employee(Base):
    __tablename__ = 'Employee'

    Empid = Column(String(5), primary_key=True)
    Cardno = Column(String(17))
    Prefixid = Column(Integer, ForeignKey('Prefix.Pre_id'))     # doc: Prefixed
    Fullname = Column(String(50))
    Address = Column(String(50))
    Tel = Column(String(50))
    Posid = Column(Integer, ForeignKey('Positions.Posid'))     # doc: Prefixed
    Username = Column(String(30))
    Password = Column(String(30))


    prename = relationship('Prefix', foreign_keys='Employee.Prefixid')
    posname = relationship('Positions', foreign_keys='Employee.Posid')
    #user = relationship("User", backref=backref('addresses', order_by=Empid))

    def __repr__(self):
        """Optinal"""
        return "<Employee(name='{0}', fullname='{1}')>".format(self.Empid, self.Fullname.encode('utf8'))

#class WaterMeter(Base):
    #__tablename__ = 'Water_meter'

    #Mt_no = Column(Integer)
    #Adddate = Column(Date)          # doc: Integer
    #Meterte_ID = Column(Integer)    # doc: Date
    #Beformeternum = Column(String(6))
    #Nowformeternum = Column(String(6))
    #Amount = Column(Integer)
    #Cus_id = Column(String)         # doc: Auto_Increment

class Prefix(Base):
    __tablename__ = 'Prefix'

    Pre_id = Column(Integer, primary_key=True)
    prename = Column(String(25))

    def __repr__(self):
        return "<Prefix(id='{0}', name='{1}')>".format(self.Pre_id, self.prename.encode('utf8'))

class Positions(Base):
    __tablename__ = 'Positions'

    Posid = Column(Integer, primary_key=True)
    posname = Column(String(50))

    def __repr__(self):
        return "<Prefix(id='{0}', name='{1}')>".format(self.Posid, self.posname.encode('utf8'))

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

def add_prename(session):
    prename = [u'นาย', u'นาง', u'นางสาว']
    for i, name in enumerate(prename):
        item = Prefix(Pre_id=i,prename=name)
        session.add(item)
        session.commit()

def add_position(session):
    array = [u'พนักงาน', u'ผู้บริหาร', u'ผู้ดูแลระบบ']
    for i, name in enumerate(array):
        item = Positions(Posid=i, posname=name)
        session.add(item)
        session.commit()

def add_employee(session):
    pranee = Employee(Empid='0003', Cardno='1559900155611', Prefixid=1,
            Fullname=u'ปรานี ดีมา', Address=u'12/4 หมู่ 8 อ.เมือง จ.น่าน',
            Tel='0837628281', Posid=0, Username='Pranee', Password='222599')
    manop = Employee(Empid='0005', Cardno='1559900155613', Prefixid=0,
            Fullname=u'มานพ มาดี', Address=u'15/3 หมู่ 7 อ.เมือง จ.น่าน',
            Tel='0861908633', Posid=1, Username='Manop', Password='333599')
    teera = Employee(Empid='0006', Posid=2, Username='Teera', Password='123456')

    session.add(pranee)
    session.commit()
    session.add(manop)
    session.commit()
    session.add(teera)
    session.commit()

Session = sessionmaker()
Session.configure(bind=engine)
session = Session()

add_prename(session)
add_position(session)
#item = session.query(Positions).filter_by(Posid=1).first()
#print 'item', item

add_employee(session)

#our_user = session.query(Employee).filter_by(Username='Pranee').first()
our_user = session.query(Employee).filter_by(Username='Manop').first()
print "EMP", our_user
print "PRE", our_user.prename
print "POS", our_user.posname

