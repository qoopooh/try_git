package com.berm.swguava;

import com.google.common.base.CharMatcher;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        String string = "Scream 4";
        CharMatcher matcher = CharMatcher.JAVA_LETTER_OR_DIGIT;
        int count = matcher.countIn(string);
        System.out.println( "Letter or digit count: " + count );
    }
}
