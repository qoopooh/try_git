#!/usr/bin/env python
# -*- coding: utf-8 -*-

from sqlalchemy import create_engine, Column, Integer, String, DateTime, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship, backref

engine = create_engine('sqlite:///:memory:', echo=False)
#engine = create_engine('mysql+mysqldb://root:sddba@192.168.1.57/test')
#engine = create_engine('mysql://root:sddba@192.168.1.57/test?water=utf8', echo=False)
Base = declarative_base()

class Employee(Base):
    '''สำหรับเก็บข้อมูลผู้ใช้งานระบบ
    '''

    __tablename__ = 'Employee'

    Empid = Column(String(5), primary_key=True)
    Cardno = Column(String(17))
    Prefixid = Column(Integer, ForeignKey('Prefix.Pre_id'))     # doc: Prefixed
    Fullname = Column(String(50))
    Address = Column(String(50))
    Tel = Column(String(50))
    Posid = Column(Integer, ForeignKey('Positions.Posid'))
    Username = Column(String(30))
    Password = Column(String(30))

    prename = relationship('Prefix', foreign_keys='Employee.Prefixid')
    posname = relationship('Positions', foreign_keys='Employee.Posid')
    #user = relationship("User", backref=backref('addresses', order_by=Empid))

    def __repr__(self):
        """Optinal"""
        fullname = ''
        if self.Fullname is not None:
            fullname = self.Fullname.encode('utf8')
        return "<Employee(id='{0}', name='{1}', fullname='{2}, {3}, {4}')>".format(
                self.Empid, self.Username, fullname, self.prename, self.posname)

class WaterMeter(Base):
    '''สำหรับเก็บข้อมูลมิเตอร์ค่าน้ำ
    Mt_no ...
    Meterte_ID is Date?
    Beformeternum & Nowformeternum are String?
    Is it the right table to count meter number?
    Does employee calculate amount?
    '''
    __tablename__ = 'Water_meter'

    Meterte_ID = Column(Integer, primary_key=True) # doc: Date, no need Mt_no
    Adddate = Column(DateTime)          # doc: Integer
    Beformeternum = Column(String(6))
    Nowformeternum = Column(String(6))
    Amount = Column(Integer)
    Cus_id = Column(Integer, ForeignKey('Customers.Cus_id'))
    Dates_money = Column(DateTime)
    Payment_Status = Column(Integer)

class Prefix(Base):
    '''สำหรับเก็บข้อมูลคำนำหน้าชื่อ'''
    __tablename__ = 'Prefix'

    Pre_id = Column(Integer, primary_key=True)
    prename = Column(String(25))

    def __repr__(self):
        return "<Prefix(id='{0}', name='{1}')>".format(self.Pre_id, self.prename.encode('utf8'))

class Positions(Base):
    '''สำหรับเก็บข้อมูลตำแหน่ง'''
    __tablename__ = 'Positions'

    Posid = Column(Integer, primary_key=True)   # doc: Varchar(3)
    posname = Column(String(50))

    def __repr__(self):
        return "<Positions(id='{0}', name='{1}')>".format(self.Posid, self.posname.encode('utf8'))

class Customers(Base):
    '''สำหรับเก็บข้อมูลลูกค้า'''
    __tablename__ = 'Customers'

    Cus_id = Column(Integer, primary_key=True, autoincrement=True)
    Pre_id = Column(Integer, ForeignKey('Prefix.Pre_id'))
    Fullname = Column(String(50))
    Card_id = Column(String(17))
    Cus_Address = Column(String(50))
    Cus_Phone = Column(String(15))

    prename = relationship('Prefix', foreign_keys='Customers.Pre_id')

    def __repr__(self):
        """Optinal"""
        fullname = ''
        if self.Fullname is not None:
            fullname = self.Fullname.encode('utf8')
        return "<Customers(id='{0}, Card_id='{1}', prename='{2}', fullname='{3}')>".format(
                self.Cus_id, self.Card_id, self.prename, fullname)


#class Payment(Base):
    '''ชำระเงินค่าติดตั้ง สำหรับเก็บข้อมูลชำระเงิน
    Meterte_ID is PK?
    Meterte_ID is Date?
    Where we can get value of Install_price?
    '''
    #__tablename__ = 'Payment'

#class WaterUse(Base):
    '''ข้อมูลการใช้น้ำ สำหรับเก็บข้อมูลการใช้น้ำ
    Here Meterte_ID is String from WaterMeter table?
    Do we get Register_Before & Register_After from Beformeternum & Nowformeternum of WaterMeter table?
    '''
    #__tablename__ = 'Water_use'

#class Extensions(Base):
    '''สำหรับเก็บข้อมูลการแจ้งชำระเงิน
    Is it the same amount as WaterMeter table?
    Where does the Free (Fee) come from?
    '''

    #__tablename__ = 'Extensions'

#class Billnotpay(Base):
    '''สำหรับเก็บข้อมูลบิลที่ยังไม่ได้ชำระเงิน
    '''
    #__tablename__ = 'Billnotpay'

#class Bill(Base):
    '''สำหรับเก็บข้อมูลใบเสร็จรับเงิน
    '''
    #__tablename__ = 'Bill'

#class Uninstall(Base):
    '''สำหรับเก็บข้อมูลยกเลิกใช้น้ำ
    '''
    #__tablename__ = 'Uninstall'

Base.metadata.create_all(engine) 

def add_prefix(session):
    array = [u'นาย', u'นาง', u'นางสาว']
    for i, name in enumerate(array):
        item = Prefix(prename=name)
        session.add(item)
        session.commit()

def add_position(session):
    array = [u'ผู้บริหาร', u'พนักงาน', u'ผู้ดูแลระบบ']
    for i, name in enumerate(array):
        item = Positions(posname=name)
        session.add(item)
        session.commit()

def add_employee(session):
    pranee = Employee(Empid='0003', Cardno='1559900155611', Prefixid=2,
            Fullname=u'ปรานี ดีมา', Address=u'12/4 หมู่ 8 อ.เมือง จ.น่าน',
            Tel='0837628281', Posid=2, Username='Pranee', Password='222599')
    manop = Employee(Empid='0005', Cardno='1559900155613', Prefixid=1,
            Fullname=u'มานพ มาดี', Address=u'15/3 หมู่ 7 อ.เมือง จ.น่าน',
            Tel='0861908633', Posid=1, Username='Manop', Password='333599')
    teera = Employee(Empid='0006', Posid=3, Username='Teera', Password='123456',
            Prefixid=1)

    session.add(pranee)
    session.commit()
    session.add(manop)
    session.commit()
    session.add(teera)
    session.commit()

def test_employee(session):
    employees = ['Pranee', 'Manop', 'Teera']
    for name in employees:
        user = session.query(Employee).filter_by(Username=name).first()
        print "EMP", user

def add_customers(session):
    cus1 = Customers(Pre_id=1,
            Fullname=u'นวล  ใจงาม', Card_id='1559900155634',
            Cus_Address=u'56/7 ต.ไชยสถาน อ.เมือง จ.น่าน',
            Cus_Phone='0847638382') # doc: has Cus_Age
    cus2 = Customers(Pre_id=2,
            Fullname=u'มะลิ  ดำดี', Card_id='1559900155677',
            Cus_Address=u'34/5 ต.ไชยสถาน อ.เมือง จ.น่าน',
            Cus_Phone='0876543567') # doc: has Cus_Age

    session.add(cus1)
    session.commit()
    session.add(cus2)
    session.commit()

def test_customers(session):
    q = session.query(Customers).all()
    for i in q:
        print "CUS", i

Session = sessionmaker()
Session.configure(bind=engine)
session = Session()

add_prefix(session)
add_position(session)
add_employee(session)
test_employee(session)
add_customers(session)
test_customers(session)

