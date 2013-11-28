
package com.atouch;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Usr {

    static final String BROADCAST_MSG = "12345678901234567890123456789012345678";
    static final String SETTING_MSG = "110415";
    static final String ALL_IP = "192.168.1.255";
    static final int UDP_PORT = 1500;
    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public enum Mode {
        TcpClient,
        Udp,
        UdpServer,
        TcpServer,
        Unknown
    }

    private String mMac = "00:00:00:00:00:00";
    private String mDestIp = "192.168.1.168";
    private int mDestPort = 2000;
    private String mHostIp = "192.168.1.31";
    private int mHostPort = 1470;
    private String mGatewayIp = "192.168.1.254";
    private Mode mMode = Mode.TcpServer;
    private int mBaud = 38401;

    public Usr(byte[] data) {
        mMac = extractMac(Arrays.copyOfRange(data, 0, 6));
        mDestIp = BinaryConversion.bytesToIp(Arrays.copyOfRange(data, 7, 11));
        mDestPort = BinaryConversion.bytesToNumber(Arrays.copyOfRange(data, 11, 13));
        mHostIp = BinaryConversion.bytesToIp(Arrays.copyOfRange(data, 13, 17));
        mHostPort = BinaryConversion.bytesToNumber(Arrays.copyOfRange(data, 17, 19));
        mGatewayIp = BinaryConversion.bytesToIp(Arrays.copyOfRange(data, 19, 23));
        int mode = (data[23] & 0xFF);
        if (mode < Mode.values().length)
            mMode = Mode.values()[mode];
        else
            mMode = Mode.Unknown;
        mBaud = BinaryConversion.bytesToNumber(Arrays.copyOfRange(data, 24, 27));
        System.out.println("[FOUND] " + BinaryConversion.bytesToHex(data));
        System.out.println("mac: " + mMac);
        System.out.println("dest: " + mDestIp + " " +
                Integer.toString(mDestPort));
        System.out.println("host: " + mHostIp + " " +
                Integer.toString(mHostPort));
        System.out.println("gateway: " + mGatewayIp);
        System.out.println("mode: " + mMode + " baud: " +
                Integer.toString(mBaud));
    }

    private String extractMac(byte[] data) {
        return BinaryConversion.bytesToHex(data);
    }

    public String getMac() {
        return mMac;
    }

    public String getIp() {
        return mHostIp;
    }

    public String getPort() {
        return Integer.toString(mHostPort);
    }

    /**
     * @return
     */
    private byte[] getBytes() {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        byte[] mac = BinaryConversion.hexStringToByteArray(mMac);
        byte[] destIp = BinaryConversion.ipToByteArray(mDestIp);
        byte[] destPort = BinaryConversion.intToByteArray(mDestPort, 2);
        byte[] hostIp = BinaryConversion.ipToByteArray(mHostIp);
        byte[] hostPort = BinaryConversion.intToByteArray(mHostPort, 2);
        byte[] gatewayIp = BinaryConversion.ipToByteArray(mGatewayIp);
        byte[] baud = BinaryConversion.intToByteArray(mBaud, 3);

        try {
            baos.write(mac);
            baos.write(SETTING_MSG.getBytes());
            baos.write(destIp);
            baos.write(destPort);
            baos.write(hostIp);
            baos.write(hostPort);
            baos.write(gatewayIp);
            baos.write((byte) mMode.ordinal());
            baos.write(baud);
            baos.write(SETTING_MSG.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }

        return baos.toByteArray();
    }

    public static List<Usr> getList() throws IOException {
        byte[] host_ip = InetAddress.getLocalHost().getAddress();
        DatagramSocket serverSocket = new DatagramSocket(UDP_PORT);
        byte[] buf = BROADCAST_MSG.getBytes();
        InetAddress address = InetAddress.getByName(ALL_IP);
        DatagramPacket packet = new DatagramPacket(buf, buf.length,
                address, UDP_PORT);
        serverSocket.send(packet);
        serverSocket.setSoTimeout(1000); // set the timeout in millisecounds.

        ArrayList<Usr> out = new ArrayList<Usr>();
        while (true) {
            try {
                byte[] rcv = new byte[32];
                packet = new DatagramPacket(rcv, rcv.length);
                serverSocket.receive(packet);
                byte[] ip = packet.getAddress().getAddress();
                if (Arrays.equals(ip, host_ip))
                    continue;
                Usr usr = new Usr(packet.getData());
                out.add(usr);
            } catch (SocketTimeoutException e) {
                break;
            }
        }

        serverSocket.close();
        return out;
    }

    public static void update(Usr u, String ip, String port) throws IOException {
        u.mHostIp = ip;
        u.mHostPort = Integer.valueOf(port);

        byte[] buf = u.getBytes();
        InetAddress address = InetAddress.getByName(ALL_IP);
        DatagramPacket packet = new DatagramPacket(buf, buf.length,
                address, UDP_PORT);
        DatagramSocket s = new DatagramSocket(UDP_PORT);
        s.send(packet);
        s.close();
        System.out.println("[UPDATE] " + BinaryConversion.bytesToHex(buf));
    }
}
