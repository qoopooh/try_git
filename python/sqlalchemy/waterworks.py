#!/usr/bin/env python
# -*- coding: utf-8 -*-

from datetime import datetime

from sqlalchemy import create_engine, Column, Integer, String, DateTime, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship, backref

# CREATE DATABASE `water` CHARACTER SET utf8 COLLATE utf8_general_ci;

#engine = create_engine('sqlite:///:memory:', echo=False)
#engine = create_engine('mysql+mysqldb://root:sddba@localhost/water?charset=utf8')
engine = create_engine('mysql://root:sddba@192.168.1.57/water?charset=utf8', echo=False)

Base = declarative_base()

class Employee(Base):
    '''สำหรับเก็บข้อมูลผู้ใช้งานระบบ
    '''

    __tablename__ = 'Employee'

    Empid = Column(Integer, primary_key=True)
    Cardno = Column(String(17))
    Prefixid = Column(Integer, ForeignKey('Prefix.Pre_id'))
    Fullname = Column(String(50))
    Address = Column(String(50))
    Tel = Column(String(50))
    Posid = Column(Integer, ForeignKey('Positions.Posid'))
    Username = Column(String(30))
    Password = Column(String(30))

    prefix = relationship('Prefix', foreign_keys='Employee.Prefixid')
    posname = relationship('Positions', foreign_keys='Employee.Posid')
    #user = relationship("User", backref=backref('addresses', order_by=Empid))

    def __repr__(self):
        """Optinal"""
        fullname = ''
        if self.Fullname is not None:
            fullname = self.Fullname.encode('utf8')
        return "<Employee(id='{0}', name='{1}', fullname='{2}, {3}, {4}')>".format(
                self.Empid, self.Username, fullname, self.prefix, self.posname)

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

    Posid = Column(Integer, primary_key=True)
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

    prefix = relationship('Prefix', foreign_keys='Customers.Pre_id')

    def __repr__(self):
        """Optinal"""
        fullname = ''
        if self.Fullname is not None:
            fullname = self.Fullname.encode('utf8')
        return "<Customers(id='{0}', name='{2} {3}', address='{4}')>".format(
                self.Cus_id, self.Card_id, self.prefix.prename.encode('utf8'),
                fullname, self.Cus_Address.encode('utf8'))


#class Payment(Base):
    '''ชำระเงินค่าติดตั้ง สำหรับเก็บข้อมูลชำระเงิน
    Customer has to do on first registration

    Meterte_ID is PK?
    Meterte_ID is Date?
    Install_price = 1500
    '''
    #__tablename__ = 'Payment'

class Meter(Base):
    '''สำหรับเก็บข้อมูลมิเตอร์ (Payment)
    ชำระเงินค่าติดตั้ง
    '''
    __tablename__ = 'Meter'

    Meter_ID = Column(Integer, primary_key=True)
    Cus_id = Column(Integer, ForeignKey('Customers.Cus_id'))
    Empid = Column(Integer, ForeignKey('Employee.Empid'))
    Address = Column(String(50), nullable=False, unique=True)
    MeterType = Column(String(10), default="1", info='"1": 1 inch, "1.5": 1 1/2 inches')
    PaymentDate = Column(DateTime, default=datetime.utcnow)
    Status = Column(Integer, default=1, info='1: active, 0: suspend')
    InstallPrice = Column(Integer, default=1500)

    customer = relationship('Customers', foreign_keys='Meter.Cus_id')
    employee = relationship('Employee', foreign_keys='Meter.Empid')

    def __repr__(self):
        customer_name = self.customer.prefix.prename + ' ' + self.customer.Fullname
        employee_name = self.employee.prefix.prename + ' ' + self.employee.Fullname
        return "<Meter(id='{0}', customer='{1}', address='{2}', employee='{3}')>".format(
                self.Meter_ID,
                customer_name.encode('utf8'),
                self.Address.encode('utf8'),
                employee_name.encode('utf8'))

class WaterUse(Base):
    '''ข้อมูลการใช้น้ำ สำหรับเก็บข้อมูลการใช้น้ำ
    Here Meterte_ID is String from WaterMeter table?
    Do we get Register_Before & Register_After from Beformeternum & Nowformeternum of WaterMeter table?
    '''
    __tablename__ = 'Water_use'

    Water_use_id = Column(Integer, primary_key=True)
    Meter_ID = Column(Integer, ForeignKey('Meter.Meter_ID'))

    # Wather unit use
    Amount = Column(Integer, default=0, info='Water unit')

    # Calculated payment
    Fee = Column(Integer, default=0, info='Delay/renewal fee (Baht)')
    Total = Column(Integer, info='Total payment (Baht)')

    # Transaction history
    RecordDate = Column(DateTime, info='Measure meter date')
    DueDate = Column(DateTime, info='Payment Due Date')
    PaidDate = Column(DateTime, info='Actual paid date')

    meter = relationship('Meter', foreign_keys='WaterUse.Meter_ID')

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

class Uninstall(Base):
    '''สำหรับเก็บข้อมูลยกเลิกใช้น้ำ
    '''
    __tablename__ = 'Uninstall'

    UN_No = Column(Integer, primary_key=True)
    UN_Location = Column(String(50), default=u'องค์การบริหารส่วนตำบลไชยสถาน')
    UN_Date = Column(DateTime, default=datetime.utcnow)
    UN_Reson = Column(String(50))
    UN_Status = Column(Integer, default=1, info='0: approved, 1: waiting')

    Meter_ID = Column(Integer, ForeignKey('Meter.Meter_ID'))
    Officer_id = Column(Integer, ForeignKey('Employee.Empid'))
    Management_id = Column(Integer, ForeignKey('Employee.Empid'))

    meter = relationship('Meter', foreign_keys='Uninstall.Meter_ID')
    officer = relationship('Employee', foreign_keys='Uninstall.Officer_id')
    management = relationship('Employee', foreign_keys='Uninstall.Management_id')

Base.metadata.create_all(engine) 

def add_prefix(session):
    array = [u'นาย', u'นาง', u'นางสาว']
    for i, name in enumerate(array):
        item = Prefix(prename=name)
        session.add(item)
        session.commit()

def add_position(session):
    array = [u'เจ้าหน้าที่', u'ผู้บริหาร', u'ผู้ดูแลระบบ']
    for i, name in enumerate(array):
        item = Positions(posname=name)
        session.add(item)
        session.commit()

def add_employee(session):
    pranee = Employee(Cardno='1559900155611', Prefixid=2,
            Fullname=u'ปรานี ดีมา', Address=u'12/4 หมู่ 8 อ.เมือง จ.น่าน',
            Tel='0837628281', Posid=1, Username='Pranee', Password='\'')
    manop = Employee(Cardno='1559900155613', Prefixid=1,
            Fullname=u'มานพ มาดี', Address=u'15/3 หมู่ 7 อ.เมือง จ.น่าน',
            Tel='0861908633', Posid=2, Username='Manop', Password='\'')
    teera = Employee(Posid=3, Username='Teera', Password='\'',
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
        print user

def add_customers(session):
    cus1 = Customers(Pre_id=1,
            Fullname=u'นวล  ใจงาม', Card_id='1559900155634',
            Cus_Address=u'56/7 ต.ไชยสถาน อ.เมือง จ.น่าน',
            Cus_Phone='0847638382')
    cus2 = Customers(Pre_id=2,
            Fullname=u'มะลิ  ดำดี', Card_id='1559900155677',
            Cus_Address=u'34/5 ต.ไชยสถาน อ.เมือง จ.น่าน',
            Cus_Phone='0876543567')

    session.add(cus1)
    session.commit()
    session.add(cus2)
    session.commit()

def test_customers(session):
    q = session.query(Customers).all()
    for i in q:
        print i

def add_meters(session):
    m1 = Meter(Cus_id=1, Empid=1, Address='89/4')
    session.add(m1)
    session.commit()

    m2 = Meter(Cus_id=1, Empid=1, Address=u'อรสิริน 6')
    session.add(m2)
    session.commit()

    m3 = Meter(Cus_id=2, Empid=1, Address='Ornsirn 6')
    session.add(m3)
    session.commit()

def test_meters(session):
    q = session.query(Meter).all()
    for i in q:
        print i


if __name__ == '__main__':
    Session = sessionmaker()
    Session.configure(bind=engine)
    session = Session()

    add_prefix(session)
    add_position(session)
    add_employee(session)
    add_customers(session)
    add_meters(session)
    test_meters(session)
