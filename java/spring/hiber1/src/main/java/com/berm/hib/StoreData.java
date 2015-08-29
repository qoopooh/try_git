package com.berm.hib;

import java.sql.Timestamp;
import java.util.Date;
import java.util.List;

import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;

public class StoreData {
    public static void main(String[] args) {

        //creating configuration object
        Configuration cfg=new Configuration();
        cfg.configure("hibernate.cfg.xml");//populates the data of the configuration file

        //creating seession factory object
        SessionFactory factory=cfg.buildSessionFactory();

        //creating session object
        Session session=factory.openSession();

        //testAddCode(session);
        testReadCode(session);

        session.close();

        System.out.println("successfully saved");
    }

    private static void testReadCode(Session s) {
        //String hql = "FROM aiacode WHERE id < :max AND id > :min";
        String hql = "FROM AiaCode";

        //creating transaction object
        Query q=s.createQuery(hql);
        //q.setInteger("min", 5);
        //q.setInteger("max", 9);
        //q.serFirstResult(2);
        //q.setMaxResult(5);
        
        List<?> list = q.list();
        for (int i=0; i<list.size(); i++) {
            AiaCode c = (AiaCode) list.get(i);
            System.out.println(String.format("%d: %s", i, c));
        }
    }

    private static void testAddCode(Session s) {
        //creating transaction object
        Transaction t=s.beginTransaction();

        int id = 111;
        AiaCode c=new AiaCode();
        c.setId(id);
        c.setCode("BERM" + id);
        c.setName("เบิ้ม " + id);
        Date date = new Date();
        c.setDate(new Timestamp(date.getTime()));

        s.persist(c);//persisting the object

        t.commit();//transaction is commited
    }
}
