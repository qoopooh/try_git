
package com.atouch;

import java.awt.event.ActionEvent;
import java.io.IOException;
import java.net.URL;
import java.util.List;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class Gui extends javax.swing.JFrame {

    public static final String VERSION = "1.1";
    public static final String PASSKEY = "A-touch";

    private static final long serialVersionUID = 1L;
    private JButton updateButton;
    private JLabel ipLabel;
    private JTextField ipTextField;
    private JLabel portLabel;
    private JTextField portTextField;
    private DefaultListModel listModel;
    private JList list;
    private List<Usr> mUsrList;
    private Usr mSelectedUsr;

    private boolean pass;

    public Gui() {
        initComponents();
        refreshList();
        pass = false;
    }

    private void initComponents() {
        updateButton = new javax.swing.JButton();
        ipLabel = new javax.swing.JLabel();
        ipTextField = new javax.swing.JTextField();
        portLabel = new javax.swing.JLabel();
        portTextField = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Domotics Gateway Setting V." + VERSION);

        updateButton.setText("Update");
        updateButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                updateButtonActionPerformed(evt);
            }
        });

        ipLabel.setText("Module IP:");
        portLabel.setText("Module Port:");
        ipTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                updateButtonActionPerformed(evt);
            }
        });
        portTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                updateButtonActionPerformed(evt);
            }
        });

        listModel = new DefaultListModel();
        listModel.addElement("0057C7294D62 192.168.1.254");
        listModel.addElement("0069C92A4D5C 255.255.255.255");
        // Create the list and put it in a scroll pane.
        list = new JList(listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setSelectedIndex(0);
        list.setVisibleRowCount(4);
        list.addListSelectionListener(mLsl);
        JScrollPane listScrollPane = new JScrollPane(list);

        URL iconURL = getClass().getResource("/com/atouch/logo-domotics-icon.png");
        ImageIcon icon = new ImageIcon(iconURL);
        setIconImage(icon.getImage());

        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (InstantiationException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (UnsupportedLookAndFeelException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(
                getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(layout
                .createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(
                        layout.createSequentialGroup()
                                .addContainerGap()
                                .addGroup(
                                        layout.createParallelGroup(
                                                javax.swing.GroupLayout.Alignment.LEADING)
                                                .addGroup(
                                                        layout.createSequentialGroup()
                                                                .addComponent(
                                                                        ipLabel,
                                                                        javax.swing.GroupLayout.PREFERRED_SIZE,
                                                                        javax.swing.GroupLayout.DEFAULT_SIZE,
                                                                        javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addPreferredGap(
                                                                        javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(
                                                                        ipTextField))
                                                .addGroup(
                                                        layout.createSequentialGroup()
                                                                .addComponent(
                                                                        portLabel,
                                                                        javax.swing.GroupLayout.PREFERRED_SIZE,
                                                                        javax.swing.GroupLayout.DEFAULT_SIZE,
                                                                        javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addPreferredGap(
                                                                        javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(
                                                                        portTextField))
                                                .addGroup(
                                                        layout.createSequentialGroup()
                                                                .addComponent(
                                                                        updateButton)
                                                                .addPreferredGap(
                                                                        javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(
                                                                        listScrollPane)))
                                .addContainerGap(27, Short.MAX_VALUE)));

        layout.linkSize(javax.swing.SwingConstants.HORIZONTAL,
                new java.awt.Component[] {
                        updateButton, ipLabel, portLabel
                });

        layout.setVerticalGroup(layout
                .createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(
                        layout.createSequentialGroup()
                                .addContainerGap()
                                .addGroup(
                                        layout.createParallelGroup(
                                                javax.swing.GroupLayout.Alignment.BASELINE)
                                                .addComponent(
                                                        ipLabel,
                                                        javax.swing.GroupLayout.PREFERRED_SIZE,
                                                        javax.swing.GroupLayout.DEFAULT_SIZE,
                                                        javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addComponent(ipTextField))
                                .addPreferredGap(
                                        javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(
                                        layout.createParallelGroup(
                                                javax.swing.GroupLayout.Alignment.BASELINE)
                                                .addComponent(
                                                        portLabel,
                                                        javax.swing.GroupLayout.PREFERRED_SIZE,
                                                        javax.swing.GroupLayout.DEFAULT_SIZE,
                                                        javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addComponent(portTextField))
                                .addPreferredGap(
                                        javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(
                                        layout.createParallelGroup(
                                                javax.swing.GroupLayout.Alignment.BASELINE)
                                                .addComponent(updateButton)
                                                .addComponent(listScrollPane))
                                .addContainerGap(21, Short.MAX_VALUE)));
        pack();
    }

    protected void updateButtonActionPerformed(ActionEvent evt) {
        String cmd = evt.getActionCommand();
        System.out.println("[ACTION] " + cmd);
        if (!pass) {
            String aWord = PASSKEY;
            PassDialog pd = new PassDialog(this, aWord);
            pd.setVisible(true);
            if (!pd.getResult())
                return;
            pass = true;
        }
        setUsr();
        refreshList();
    }

    private void setUsr() {
        if (mSelectedUsr == null) {
            JOptionPane.showMessageDialog(
                    this,
                    "Please select the module first.",
                    "Try again",
                    JOptionPane.ERROR_MESSAGE);
            return;
        }
        try {
            Usr.update(mSelectedUsr, ipTextField.getText(), portTextField.getText());
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            Thread.sleep(3000);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    private void refreshList() {
        try {
            listModel.clear();
            mSelectedUsr = null;
            mUsrList = Usr.getList();
            if (mUsrList.size() < 1)
                return;
            for (Usr u : mUsrList) {
                String item = u.getMac() + " " + u.getIp();
                listModel.addElement(item);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private ListSelectionListener mLsl = new ListSelectionListener()
    {
        @Override
        public void valueChanged(ListSelectionEvent evt)
        {
            if (evt.getValueIsAdjusting())
            {
                // System.out.println("Eventhandler called evt.getValueIsAdjusting() true");
                return;
            }
            else
            {
                String val = (String) list.getSelectedValue();
                if ((mUsrList == null) || (val == null) || (mUsrList.size() < 1))
                    return;
                System.out.println("[SELECT] " + val);
                String mac = val.split(" ")[0];

                for (Usr u : mUsrList) {
                    if (!u.getMac().equals(mac))
                        continue;
                    ipTextField.setText(u.getIp());
                    portTextField.setText(u.getPort());
                    mSelectedUsr = u;
                    break;
                }
            }
        }
    };

    /**
     * @param args
     */
    public static void main(String[] args) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Gui().setVisible(true);
            }
        });
    }

}
