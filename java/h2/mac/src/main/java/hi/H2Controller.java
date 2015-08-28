package hi;

import java.lang.ClassNotFoundException;
import java.sql.*;

import org.springframework.boot.*;
import org.springframework.boot.autoconfigure.*;
import org.springframework.stereotype.*;
import org.springframework.web.bind.annotation.*;

@Controller
@EnableAutoConfiguration
public class H2Controller {

    //private final static String DB_URL = "jdbc:h2:~/test";
    private final static String DB_URL = "jdbc:h2:tcp://localhost/~/test";
    private final static String USER = "sa";
    private final static String SELECT_ALL = "SELECT * FROM TEST";

    @RequestMapping("/")
    @ResponseBody
    String index() {
        try {
            Class.forName("org.h2.Driver");
        } catch (ClassNotFoundException e) {
            return e.toString();
        }

        StringBuilder sb = new StringBuilder("ID, NAME\n");
        try {
            Connection conn = DriverManager.getConnection(DB_URL, USER, USER);

            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery(SELECT_ALL);

            while (rs.next()) {
                int id = rs.getInt("ID");
                String name = rs.getString("NAME");
                sb.append(String.format("%d, %s", id, name));
            }

            conn.close();
        } catch (SQLException e) {
            return "ERROR: " + e.toString();
        }
        return sb.toString();
    }

    public static void main(String[] args) throws Exception {
        SpringApplication.run(H2Controller.class, args);
    }
}
