package com.berm.swguava;

import java.util.ArrayList;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import com.google.common.base.CharMatcher;
import com.google.common.base.Joiner;
import com.google.common.base.Strings;
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

    public void testJoiner() {
        ArrayList charList = Lists.newArrayList("a", "b", "c", "d");
        StringBuilder buffer = new StringBuilder();

        buffer = Joiner.on("|").appendTo(buffer, charList);
        assertTrue(buffer.toString().equals("a|b|c|d"));

        String joinedCharList = Joiner.on(", ").join(charList);
        assertTrue(joinedCharList.equals("a, b, c, d"));

        charList.add(null);
        String join4 = Joiner.on(" - ").skipNulls().join(charList);
        assertTrue(join4.equals("a - b - c - d"));

        join4 = Joiner.on(" - ").useForNull("defaultValue").join(charList);
        assertTrue(join4.equals("a - b - c - d - defaultValue"));

        join4 = Joiner.on("|").join("first", "second", "third");
        assertTrue(join4.equals("first|second|third"));
    }

    public void testNullString() {
        String arg = "";
        assertTrue(Strings.isNullOrEmpty(arg));
        arg = null;
        assertTrue(Strings.isNullOrEmpty(arg));
        arg = "something";
        assertFalse(Strings.isNullOrEmpty(arg));
    }

    public void testRepeatedString() {
        String repeat = Strings.repeat("beetlejuice", 3);
        assertTrue(repeat.equals("beetlejuicebeetlejuicebeetlejuice"));
    }

    public void testPadding() {
        String padEnd = Strings.padEnd("star wars", 15, 'X');
        String padStart = Strings.padStart("star wars", 15, 'X');

        assertTrue(padEnd.equals("star warsXXXXXX"));
        assertTrue(padEnd.length() == 15);
        assertTrue(padStart.equals("XXXXXXstar wars"));
        assertTrue(padStart.length() == 15);
    }
}
