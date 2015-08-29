import java.nio.charset.Charset;
//STEP 1. Import required packages
import java.sql.*;

public class FirstExample {
    // JDBC driver name and database URL
    static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
    //static final String DB_URL = "jdbc:mysql://192.168.1.57/test";
    static final String DB_URL = "jdbc:mysql://192.168.1.57/test?useUnicode=true&characterEncoding=utf-8";

    //  Database credentials
    static final String USER = "root";
    static final String PASS = "sddba";

    static final Charset UTF8_CHARSET = Charset.forName("UTF-8");

    public static void main(String[] args) {
        Connection conn = null;
        Statement stmt = null;
        try{
            //STEP 2: Register JDBC driver
            Class.forName("com.mysql.jdbc.Driver");

            //STEP 3: Open a connection
            System.out.println("Connecting to database...");
            conn = DriverManager.getConnection(DB_URL,USER,PASS);

            //STEP 4: Execute a query
            System.out.println("Creating statement...");
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM aiacode";
            ResultSet rs = stmt.executeQuery(sql);

            //STEP 5: Extract data from result set
            while(rs.next()){
                //Retrieve by column name
                int id  = rs.getInt("id");
                String code = rs.getString("code");
                String name = rs.getString("name");

                //Display values
                System.out.print("ID: " + id);
                System.out.print(", code: " + code);
                System.out.println(", name: "
                        //+ name);
                        //+ new String(name.getBytes("UTF-8"), "UTF-8"));
                        //+ new String(name.getBytes("US-ASCII"), "UTF-8"));
                        //+ new String(name.getBytes("ISO-8859-1"), UTF8_CHARSET));
                        //+ new String(name.getBytes("ISO8859-1"), UTF8_CHARSET));
                        //+ new String(name.getBytes("ISO8859-1"), "UTF-8"));
                    + new String(name.getBytes("US-ASCII"), "UTF-8"));
                        //+ new String(name.getBytes("CP874"), "TIS-620"));
                        //+ new String(name.getBytes("ISO8859-1"), "TIS-620"));
                        //+ new String(name.getBytes("ISO8859-1"), "CP874"));
                        //+ new String(name.getBytes(), "CP874"));
                        //+ new String(name.getBytes("UTF-8"), "ISO8859-1"));
                        //+ new String(name.getBytes("ISO8859-1"), "UTF-8"));
                        //+ new String(name.getBytes("TIS-620"), "UTF-8"));
                //System.out.println(", name: " + "ภาษาไทย");
            }
            //STEP 6: Clean-up environment
            rs.close();
            stmt.close();
            conn.close();
        }catch(SQLException se){
            //Handle errors for JDBC
            se.printStackTrace();
        }catch(Exception e){
            //Handle errors for Class.forName
            e.printStackTrace();
        }finally{
            //finally block used to close resources
            try{
                if(stmt!=null)
                    stmt.close();
            }catch(SQLException se2){
            }// nothing we can do
            try{
                if(conn!=null)
                    conn.close();
            }catch(SQLException se){
                se.printStackTrace();
            }//end finally try
        }//end try
        System.out.println("Goodbye!");
    }//end main
}//end FirstExample
