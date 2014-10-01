package com.berm.swing;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class CreditCardGUI implements ActionListener {

    public static final String VERIFY_NONE = "Not yer verified";
    public static final String VERIFY_VALID = "Valid number!";
    public static final String VERIFY_INVALID = "Invalid number.";

    private JFrame frame;
    private JTextField numberField;
    private JLabel validLabel;
    private JButton verifyButton;

    public CreditCardGUI() {
        numberField = new JTextField(16);
        validLabel = new JLabel(VERIFY_NONE);
        verifyButton = new JButton("Verify CC Number");
        verifyButton.addActionListener(this);

        frame = new JFrame("Credit card number verification");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(new Dimension(350, 100));
        frame.setLayout(new FlowLayout());
        frame.add(numberField);
        frame.add(verifyButton);
        frame.add(validLabel);
        frame.setVisible(true);
    }

    public boolean isValidCC(String text) {
        int sum = 0;
        for (int i = text.length() - 1; i >= 0; --i) {
            int digit = Integer.parseInt(text.substring(i, i + 1));
            if (i % 2 == 0) {
                digit *= 2;
            }
            sum += (digit / 10) + (digit % 10);
        }

        return sum % 10 ==  0 && text.startsWith("4");
    }

    public void actionPerformed(ActionEvent event) {
        String text  = numberField.getText();
        if (isValidCC(text)) {
            validLabel.setText(VERIFY_VALID);
        } else {
            validLabel.setText(VERIFY_INVALID);
        }
    }
}
