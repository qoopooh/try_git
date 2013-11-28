
package com.atouch;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.List;

public class Usr {

    static final String BROADCAST_MSG = "12345678901234567890123456789012345678";
    static final String SETTING_MSG = "110415";
    static final String ALL_IP = "192.168.1.255";

    static final int UDP_PORT = 1500;

    public static List<Usr> getList() throws IOException {
        InetAddress IPAddress = InetAddress.getByName("localhost");

        DatagramSocket serverSocket = new DatagramSocket(UDP_PORT);
        // DatagramPacket receivePacket = null;
        // serverSocket.receive(receivePacket);
        byte[] buf = BROADCAST_MSG.getBytes();
        InetAddress address = InetAddress.getByName(ALL_IP);
        DatagramPacket packet = new DatagramPacket(buf, buf.length,
                address, UDP_PORT);
        serverSocket.send(packet);

        byte[] rcv = new byte[32];
        packet = new DatagramPacket(rcv, rcv.length);
        serverSocket.receive(packet);
        serverSocket.receive(packet);

        serverSocket.close();
        return null;
    }
}
