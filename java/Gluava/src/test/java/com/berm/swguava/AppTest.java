package com.berm.swguava;

import java.util.ArrayList;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import com.google.common.base.CharMatcher;
import com.google.common.collect.Lists;

/**
 * Unit test for simple App.
 */
public class AppTest 
    extends TestCase
{
    String string;
    CharMatcher matcher;

    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public AppTest( String testName ) {
        super( testName );
        string = "Scream 4";
        matcher = CharMatcher.JAVA_LETTER_OR_DIGIT;
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite() {
        return new TestSuite( AppTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testCountStringMatcher() {
        int count = matcher.countIn(string);
        assertTrue(count == 7);
    }

    public void testMachesAllOf() {
        assertTrue(matcher.matchesAllOf("scream"));
        assertFalse(matcher.matchesAllOf("scream "));
        assertTrue(matcher.matchesNoneOf("_?=)("));
    }

    public void testRemoveFrom() {
        String review = "Scream 4 is the #1 teen-slasher!";
        CharMatcher whitespacerMatcher = CharMatcher.WHITESPACE;

        String result = whitespacerMatcher.removeFrom(review);
        assertTrue(result.equals("Scream4isthe#1teen-slasher!"));

        String result2 = CharMatcher.DIGIT.retainFrom(review);
        assertTrue(result2.equals("41"));

        int indexOfDigit = CharMatcher.DIGIT.indexIn(review);
        assertTrue(indexOfDigit == 7);
    }

    public void testNumberMatcher() {
        CharMatcher onlyEvenNumbersMatcher = CharMatcher.anyOf("2468");
        CharMatcher noEvenNumbersMatcher = CharMatcher.noneOf("2468");
        CharMatcher largeAtoZ = CharMatcher.inRange('A', 'Z');
        CharMatcher aToZ = CharMatcher.inRange('a', 'z').or(largeAtoZ);

        assertFalse(onlyEvenNumbersMatcher.matchesAllOf("1354"));
        assertFalse(onlyEvenNumbersMatcher.matchesAllOf("starwars"));
        assertTrue(onlyEvenNumbersMatcher.matchesAllOf("6624"));
        assertFalse(noEvenNumbersMatcher.matchesAllOf("1354"));
        assertTrue(noEvenNumbersMatcher.matchesAllOf("1337"));
        assertTrue(noEvenNumbersMatcher.matchesAllOf("supermario"));
        assertTrue(aToZ.matchesAllOf("sezin"));
        assertTrue(aToZ.matchesAllOf("SeZIn"));
        assertFalse(aToZ.matchesAllOf("scream4"));
    }

    public void TestJoiner() {
        ArrayList<String> charList = Lists.newArrayList("a", "b", "c", "d");
    }
}
