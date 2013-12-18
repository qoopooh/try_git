
package th.co.aae;

import org.hibernate.Session;
import org.hibernate.cfg.Configuration;

public class HibernateMain {

    /**
     * @param args
     */
    public static void main(String[] args) {
        Configuration configuration = new Configuration();
        configuration.configure();
        User user1 = new User();
        user1.setUserName("Arpit");
        user1.setUserMessage("Hello world from arpit");

        User user2 = new User();
        user2.setUserName("Ankita");
        user2.setUserMessage("Hello world from ankita");
        Session ss = HibernateUtil.getSessionFactory().getCurrentSession();
        ss.beginTransaction();
        // saving objects to session
        ss.save(user1);
//        ss.save(user2);
        ss.getTransaction().commit();
//        ss.close();

    }
}
