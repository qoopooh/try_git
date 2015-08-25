package com.berm.db;

import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
 
/**
 *  * This program demonstrates making JDBC connection to a SQLite database.
 *  * @author www.codejava.net
 *  *
 *  */
public class JdbcSQLiteConnection {
   
    public static void main(String[] args) {
        if (args.length > 0) {
            System.out.println("\nWe have some parameters:");
            int i = 0;
            for (String arg: args) {
                System.out.println("arg " + i++ + ": " + arg);
            }
        }

        try {
            String dbURL = "jdbc:sqlite:/tmp/berm.db";

            if (args.length > 0) {
                dbURL = "jdbc:sqlite:" + args[0];
            }

            Class.forName("org.sqlite.JDBC");
            Connection conn = DriverManager.getConnection(dbURL);
            if (conn != null) {

                DatabaseMetaData dm = conn.getMetaData();

                System.out.println("\nConnected to the database: " + dbURL);
                System.out.println("Driver name: " + dm.getDriverName());
                System.out.println("Driver version: " + dm.getDriverVersion());
                System.out.println("Product name: " + dm.getDatabaseProductName());
                System.out.println("Product version: " + dm.getDatabaseProductVersion() + "\n");

                ResultSet rs = dm.getTables(null, null, "%", null);
                int count = 0;
                while (rs.next()) {
                    System.out.println("table " + ++count + ": " + rs.getString(3));
                }

                conn.close(); // have to close after iterator as well
            }
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        } catch (SQLException ex) {
            ex.printStackTrace();
        }
    }
}
