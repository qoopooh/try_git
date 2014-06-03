package com.berm.basicswing;

import javax.swing.JFrame;
import javax.swing.JPanel;

import javax.swing.JButton;
import javax.swing.JTextField;
import javax.swing.JTextArea;
import javax.swing.JLabel;
import javax.swing.JComboBox;

/**
 * Hello world!
 * mvn exec:java
 */
public class App extends JFrame
{
    JPanel p1 = new JPanel();
    //JPanel p2 = new JPanel(); // cannot have 2 panels
    JButton b = new JButton("Hello ");
    JTextField t = new JTextField("Hi", 20);
    JTextArea ta = new JTextArea("How\nare\nYou?", 5, 20);
    JLabel label = new JLabel("What's up");
    String choices[] = {
        "Hallow",
        "Bonjour",
        "Conichuwa"
    };

    JComboBox cb = new JComboBox(choices);

    public static void main( String[] args )
    {
        new App();
    }

    public App() {
        super("Basic Swing App");

        setSize(400, 300);
        setResizable(true);

        p1.add(b);
        p1.add(t);
        p1.add(ta);
        p1.add(label);
        p1.add(cb);
        add(p1);
        //add(p2);

        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}
