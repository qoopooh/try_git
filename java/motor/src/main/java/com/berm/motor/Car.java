package com.berm.motor;

/**
 * Car
 */
public class Car extends MotorVehicle 
{
    protected int doors;        // e.g. 4, 2

    // constructor
    Car(String make, String model, int year, int seats, int doors) {
        super(make, model, year, seats);
        this.doors = doors;
    }

    public int getDoors() {
        return doors;
    }

    public String toString() {
        StringBuffer buffer = new StringBuffer();
        buffer.append(super.getMake() + ", " + getModel() + ", " + getYear()
                + ", seating: " + getSeats() + ", " + doors + " doors.");
        return new String(buffer);
    }
}
