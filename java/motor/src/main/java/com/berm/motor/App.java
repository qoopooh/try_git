package com.berm.motor;

import java.lang.InterruptedException;

/**
 * Hello world!
 *
 */
public class App 
{
    public static String getHondaCity() {
        Compact honda = new Compact("Honda", "City", 2003, 4, 4);
        return honda.toString();
    }

    public static void main( String[] args ) throws InterruptedException
    {
        MyThread t = new MyThread();
        Thread y = new Thread(t);
        y.start();
        Thread.sleep(2);
        y.interrupt();
        System.out.print("Main thread: I've send the interrupt (shutdown) signal\n");
    }
}

class MyThread implements Runnable {
    int i=1;

    @Override
    public void run() {
        while(Thread.currentThread().isInterrupted() == false) {
            double a=0.0D;
            for (int k=100;k>i;--k) {
                a=k*k*2/5*Math.pow(2, k);
                a=Math.sqrt(a);
            }
            System.out.print("Res (" + i + "): " + a + "\n");
            i += 1;
        }
        System.out.println("I've got an interrupt signal :(\n");
    }
}

