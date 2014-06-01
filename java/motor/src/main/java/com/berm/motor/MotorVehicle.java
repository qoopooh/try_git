package com.berm.motor;

/**
 * MotorVehicle
 */
public class MotorVehicle 
{
    protected String make;      // e.g. Ford, Honda
    protected String model;     // e.g. Civic, Almera
    protected int year;         // e.g. 2013
    protected int seats;        // e.g. 4, 2

    // constructor
    MotorVehicle(String make, String model, int year, int seats) {
        this.make = make;
        this.model = model;
        this.year = year;
        this.seats = seats;
    }

    public String getMake() {
        return make;
    }

    public String getModel() {
        return model;
    }

    public int getYear() {
        return year;
    }

    public int getSeats() {
        return seats;
    }
}
