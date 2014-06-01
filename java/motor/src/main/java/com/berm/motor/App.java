package com.berm.motor;

/**
 * Hello world!
 *
 */
public class App 
{
    public static String getHondaCity() {
        Compact honda = new Compact("Honda", "City", 2003, 4, 4);
        return honda.toString();
    }

    public static void main( String[] args )
    {
        Car ford = new Car("Ford", "Taurus", 2001, 5, 5);
        System.out.println( "Hello World!" );
        System.out.println(ford);
        System.out.println(getHondaCity());
    }
}
