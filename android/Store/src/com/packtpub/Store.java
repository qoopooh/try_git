package com.packtpub;

public class Store {
    static {
        System.loadLibrary("store");
    }

    public native int getInteger(String pKey);
    public native void setInteger(String pKey, int pInt);
    public native String getString(String pKey);
    public native void setString(String pKey, String pString);
    public native Color getColor(String pKey);
    public native void setColor(String pKey, Color pColor);
}

