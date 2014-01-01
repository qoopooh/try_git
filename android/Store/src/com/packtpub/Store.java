package com.packtpub;

import com.packtpub.exception.*;

public class Store {
    static {
        System.loadLibrary("store");
    }

    public native int getInteger(String pKey) throws NotExistingKeyException, InvalidTypeException;
    public native void setInteger(String pKey, int pInt);
    public native String getString(String pKey) throws NotExistingKeyException, InvalidTypeException;
    public native void setString(String pKey, String pString);
    public native Color getColor(String pKey) throws NotExistingKeyException, InvalidTypeException;
    public native void setColor(String pKey, Color pColor);
    public native int[] getIntegerArray(String pKey) throws NotExistingKeyException;
    public native void setIntegerArray(String pKey, int[] pIntArray);
    public native Color[] getColorArray(String pKey) throws NotExistingKeyException;
    public native void setColorArray(String pKey, Color[] pColorArray);
}

