package com.berm.swguava;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class OrderTest extends TestCase
{
    public OrderTest(String testName) {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite() {
        return new TestSuite( AppTest.class );
    }

    public void testCompare() {
        assertTrue(true);
    }
}
