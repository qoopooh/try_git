package com.berm.swing;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class RectangleAreaCalculator extends JFrame
{
    private JLabel lengthLabel, widthLabel, areaLabel;
    private JTextField lengthText, widthText, areaText;
    private JButton calculateButton, exitButton;

    private static final int HEIGHT = 400;
    private static final int WIDTH = 700;

    private CalculateButtonHandler calculateButtonHandler;
    private ExitButtonHandler exitButtonHandler;

    public RectangleAreaCalculator()
    {
        lengthLabel = new JLabel("Please enter the rectangle's length: ");
        widthLabel = new JLabel("Please enter the rectangle's width: ");
        areaLabel = new JLabel("ค่าพื้นที่คือ: ", SwingConstants.RIGHT);

        lengthText = new JTextField("40", 12);
        widthText = new JTextField("30", 12);
        areaText = new JTextField(12);

        calculateButton = new JButton("What's the Area?");
        calculateButtonHandler = new CalculateButtonHandler();
        calculateButton.addActionListener(calculateButtonHandler);
        exitButton = new JButton("ปิด");
        exitButtonHandler = new ExitButtonHandler();
        exitButton.addActionListener(exitButtonHandler);


        // Add icon does not work

        //Toolkit toolkit = Toolkit.getDefaultToolkit();
        //MediaTracker tracker = new MediaTracker(this);
        //Image image = toolkit.getImage("resources/images/d-mcu.png");
        //tracker.addImage(image, 0);
        //try {
            //tracker.waitForAll();
        //} catch (InterruptedException e) {
            //e.printStackTrace();
        //}
        //setIconImage(image);

        setTitle("Rectangle Area Calculator จ้า");
        Container pane = getContentPane();
        pane.setLayout(new GridLayout(4, 2));

        pane.add(lengthLabel);
        pane.add(lengthText);
        pane.add(widthLabel);
        pane.add(widthText);
        pane.add(areaLabel);
        pane.add(areaText);
        pane.add(calculateButton);
        pane.add(exitButton);

        //setSize(WIDTH, HEIGHT);
		pack();
        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    private class CalculateButtonHandler implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent e) {
            double length, width, area;

            try {
                length = Double.parseDouble(lengthText.getText());
            } catch (NumberFormatException ex) {
                length = 0;
            }
            try {
                width = Double.parseDouble(widthText.getText());
            } catch (NumberFormatException ex) {
                width = 0;
            }
            area = length * width;

            areaText.setText("" + area);
        }
    }

    private class ExitButtonHandler implements ActionListener
    {
        public void actionPerformed(ActionEvent e) {
            String name = JOptionPane.showInputDialog(null, "คุณชื่ออะไรหรอครับ?");

            if (name == null)
                return;

            int choice = JOptionPane.showConfirmDialog(null,
                    String.format("คุณ%s คุณต้องการจะปิดโปรแกรมนี้จริงๆหรอ?", name));
            if (choice == JOptionPane.YES_OPTION) {
                JOptionPane.showMessageDialog(null, "เจอกันเมื่อชาติต้องการ, " + name);
                System.exit(0);
            }
        }
    }
}
