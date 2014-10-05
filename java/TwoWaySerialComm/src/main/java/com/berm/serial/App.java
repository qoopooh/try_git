package com.berm.serial;

import java.util.List;

import com.berm.serial.util.UartUtil;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        List<String> ports = UartUtil.getAvailablePorts();
        System.out.println( "Found COM port: " + ports.size() );
        if (ports.size() > 0) {
            for (String str: ports) {
                System.out.println( "* " + str );
            }
        }

        try {
            ( new TwoWaySerialComm() ).connect( "/dev/ttyUSB0" );
        } catch( Exception e ) {
            e.printStackTrace();
        }
        System.out.println( "Hello World!" );
    }
}
