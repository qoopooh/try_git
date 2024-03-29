package com.berm.swguava;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import com.google.common.base.Joiner;
import com.google.common.base.Joiner.MapJoiner;
import com.google.common.base.Splitter;
import com.google.common.base.Strings;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;

class MJoiner
{
    Map<String, Long> employeeToNumber;

    public MJoiner() {
        employeeToNumber = Maps.newHashMap();
        employeeToNumber.put("obi wan", 1L);
        employeeToNumber.put("bobba", 2L);
    }

    public String toString() {
        MapJoiner mapJoiner= Joiner.on("|").withKeyValueSeparator("->");
        return mapJoiner.join(employeeToNumber); // obi wan->1|bobba->2
    }
}

class Order
{
    private List < Employee > employeeList;
    public Order() {
        Employee sezinKarli = new Employee(4, "Sezin Karli", 4);
        Employee darthVader =  new Employee(3, "Darth Vader", 5);
        Employee hanSolo =  new Employee(2, "Han Solo", 10);
        employeeList = Lists.newArrayList(sezinKarli, hanSolo, darthVader);
    }

    public List<Employee> getList() {
        return employeeList;
    }
}

/**
 * Test guava libraries
 *
 */
public class App 
{
    private static void StringTest() {
        MJoiner joiner = new MJoiner();
        System.out.println(joiner.toString());

        String text = "I have to test my string splitter, for this purpose I'm writing this text, ";
        System.out.println(text);
        String[] split = text.split(",");
        System.out.println(Arrays.asList(split));

        Iterable<String> split2 = Splitter.on(",").omitEmptyStrings().trimResults().split(text);
        System.out.println(Lists.newArrayList(split2));

        Iterable<String> split3 = Splitter.fixedLength(5).split(text);
        System.out.println(Lists.newArrayList(split3));

        String emptyToNull = Strings.emptyToNull("Test");
        System.out.println(emptyToNull);

        emptyToNull = Strings.emptyToNull("");
        System.out.println(emptyToNull);
    }

    private static void OrderTest() {
        Order order = new Order();
        System.out.println(order.getList());
    }

    public static void main( String[] args ) {
        //StringTest();
        OrderTest();
    }
}
