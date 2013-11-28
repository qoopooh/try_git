
package com.atouch;

public class BinaryConversion {
    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        int v;
        for (int j = 0; j < bytes.length; j++) {
            v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    public static String bytesToIp(byte[] bytes) {
        String ip = new String();

        int b = (bytes[bytes.length - 1] & 0xFF);
        ip = Integer.toString(b);
        for (int j = bytes.length - 2; j >= 0; j--) {
            b = (bytes[j] & 0xFF);
            ip += '.' + Integer.toString(b);
        }
        return ip;
    }

    /**
     * Little endian conversionz
     * 
     * @param bytes
     * @return number
     */
    public static int bytesToNumber(byte[] bytes) {
        int result = 0;
        int digit = 0;

        for (byte b : bytes) {
            result += (b & 0xFF) << (8 * digit++);
        }
        return result;
    }

    public static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    public static byte[] ipToByteArray(String s) {
        byte[] ip = new byte[4];
        int i = ip.length - 1;
        String[] addr = s.split("\\.");

        for (String dec : addr) {
            ip[i--] = (byte) (Integer.valueOf(dec) & 0xFF);
        }

        return ip;
    }

    public static byte[] intToByteArray(int number, int byteNr) {
        byte[] b = new byte[byteNr];

        for (int i = 0; i < byteNr; i++) {
            b[i] = (byte) (number & 0xFF);
            number >>= 8;
        }

        return b;
    }
}
