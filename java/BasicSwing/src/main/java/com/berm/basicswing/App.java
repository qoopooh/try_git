package com.berm.basicswing;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JButton;

/**
 * Hello world!
 *
 */
public class App extends JFrame
{
    JPanel p = new JPanel();
    JButton b = new JButton("Hello");

    public static void main( String[] args )
    {
        new App();
    }

    public App() {
        super("Basic Swing App");

        setSize(400, 300);
        setResizable(true);

        p.add(b);
        add(p);

        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}
