package com.berm.serial;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        try {
            ( new TwoWaySerialComm() ).connect( "/dev/ttyUSB0" );
        } catch( Exception e ) {
            e.printStackTrace();
        }
        System.out.println( "Hello World!" );
    }
}
