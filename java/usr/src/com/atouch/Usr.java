
package com.atouch;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.List;

public class Usr {

    static final String BROADCAST_MSG = "12345678901234567890123456789012345678";
    static final String SETTING_MSG = "110415";
    static final String ALL_IP = "192.168.1.255";

    static final int UDP_PORT = 1500;

    public Usr(byte[] data) {
        // TODO Auto-generated constructor stub
    }

    public static List<Usr> getList() throws IOException {
        String hostname = InetAddress.getLocalHost().getHostName();
        InetAddress host = InetAddress.getByName(hostname);
        DatagramSocket serverSocket = new DatagramSocket(UDP_PORT);
        byte[] buf = BROADCAST_MSG.getBytes();
        InetAddress address = InetAddress.getByName(ALL_IP);
        DatagramPacket packet = new DatagramPacket(buf, buf.length,
                address, UDP_PORT);
        serverSocket.send(packet);
        serverSocket.setSoTimeout(500); // set the timeout in millisecounds.

        byte[] rcv = new byte[32];
        packet = new DatagramPacket(rcv, rcv.length);
        ArrayList<Usr> out = new ArrayList<Usr>();
        while (true) {
            try {
                serverSocket.receive(packet);
                if (packet.getAddress().getHostName().equals(hostname))
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
}
