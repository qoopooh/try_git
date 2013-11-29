
package com.atouch;

public class Console {

    /**
     * @param args
     */
    public static void main(String[] args) {
        int i = 1;
        for (String s : args) {
            System.out.println("Args " + Integer.toString(i++) + ": " + s);
        }
    }
}
