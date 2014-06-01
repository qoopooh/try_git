package com.berm.motor;

/**
 * Compact
 */
public class Compact extends Car 
{
    // constructor
    Compact(String make, String model, int year, int seats, int doors) {
        super(make, model, year, seats, doors);
    }

    public String toString() {
        StringBuffer buffer = new StringBuffer();
        buffer.append("Compact car: ");
        buffer.append(super.toString());

        return new String(buffer);
    }
}
