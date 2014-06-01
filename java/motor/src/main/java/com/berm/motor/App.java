package com.berm.motor;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        Car ford = new Car("Ford", "Taurus", 2001, 5, 5);
        Car honda = new Car("Honda", "City", 2003, 4, 4);
        System.out.println( "Hello World!" );
        System.out.println(ford);
        System.out.println(honda);
    }
}
