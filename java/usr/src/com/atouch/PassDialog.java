
package com.atouch;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

public class PassDialog extends JDialog
        implements ActionListener,
        PropertyChangeListener {
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    private String typedText = null;
    private JTextField passField;
    private String magicWord;
    private JOptionPane optionPane;

    private String btnString1 = "Enter";
    private String btnString2 = "Cancel";
    private boolean pass;

    /**
     * Returns null if the typed string was invalid; otherwise, returns the
     * string as the user entered it.
     */
    public String getValidatedText() {
        return typedText;
    }

    /** Creates the reusable dialog. */
    public PassDialog(JFrame aFrame, String aWord) {
        super(aFrame, true);
        pass = false;
        // magicWord = aWord.toUpperCase();
        magicWord = aWord;
        setTitle("Check pass key");

        passField = new JPasswordField(10);

        // Create an array of the text and components to be displayed.
        String msgString1 = "Please type the pass key";
        // String msgString2 = "(The answer is \"" + magicWord + "\".)";
        Object[] array = {
                msgString1, passField
        };

        // Create an array specifying the number of dialog buttons
        // and their text.
        Object[] options = {
                btnString1, btnString2
        };

        // Create the JOptionPane.
        optionPane = new JOptionPane(array,
                JOptionPane.QUESTION_MESSAGE,
                JOptionPane.YES_NO_OPTION,
                null,
                options,
                options[0]);

        // Make this dialog display it.
        setContentPane(optionPane);

        // Handle window closing correctly.
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent we) {
                /*
                 * Instead of directly closing the window, we're going to change
                 * the JOptionPane's value property.
                 */
                optionPane.setValue(new Integer(
                        JOptionPane.CLOSED_OPTION));
            }
        });

        // Ensure the text field always gets the first focus.
        addComponentListener(new ComponentAdapter() {
            public void componentShown(ComponentEvent ce) {
                passField.requestFocusInWindow();
            }
        });

        // Register an event handler that puts the text into the option pane.
        passField.addActionListener(this);

        // Register an event handler that reacts to option pane state changes.
        optionPane.addPropertyChangeListener(this);
        this.setSize(300, 150);
    }

    /** This method handles events for the text field. */
    public void actionPerformed(ActionEvent e) {
        optionPane.setValue(btnString1);
    }

    /** This method reacts to state changes in the option pane. */
    public void propertyChange(PropertyChangeEvent e) {
        String prop = e.getPropertyName();

        if (isVisible()
                && (e.getSource() == optionPane)
                && (JOptionPane.VALUE_PROPERTY.equals(prop) ||
                JOptionPane.INPUT_VALUE_PROPERTY.equals(prop))) {
            Object value = optionPane.getValue();

            if (value == JOptionPane.UNINITIALIZED_VALUE) {
                // ignore reset
                return;
            }

            // Reset the JOptionPane's value.
            // If you don't do this, then if the user
            // presses the same button next time, no
            // property change event will be fired.
            optionPane.setValue(
                    JOptionPane.UNINITIALIZED_VALUE);

            if (btnString1.equals(value)) {
                typedText = passField.getText();
                // String ucText = typedText.toUpperCase();
                if (magicWord.equals(typedText)) {
                    // we're done; clear and dismiss the dialog
                    clearAndHide();
                    pass = true;
                } else {
                    // text was invalid
                    passField.selectAll();
                    JOptionPane.showMessageDialog(
                            this,
                            "Sorry, the pass key does not match.",
                            "Try again",
                            JOptionPane.ERROR_MESSAGE);
                    typedText = null;
                    passField.requestFocusInWindow();
                }
            } else { // user closed dialog or clicked cancel
                // parent.setLabel("It's OK.  "
                // + "We won't force you to type "
                // + magicWord + ".");
                typedText = null;
                clearAndHide();
            }
        }
    }

    /** This method clears the dialog and hides it. */
    public void clearAndHide() {
        passField.setText(null);
        setVisible(false);
    }

    public boolean getResult() {
        return pass;
    }
}
