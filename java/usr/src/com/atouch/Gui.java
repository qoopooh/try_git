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
import javax.swing.Timer;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class Gui extends javax.swing.JFrame {

	public static final String VERSION = "1.3";
	public static final String PASSKEY = "A-touch";

	private static final long serialVersionUID = 1L;
  private static int TIMER_CYCLE = 3000;
	private JButton updateButton;
	private JLabel ipLabel;
	private JTextField ipTextField;
	private JLabel portLabel;
	private JTextField portTextField;
	private DefaultListModel listModel;
	private JList list;
	private List<Usr> mUsrList;
	private Usr mSelectedUsr;
	private Timer mTimer;

	private boolean pass;
	private JLabel modeLabel;
	private JLabel modeValue;
	private JLabel baudLabel;
	private JLabel baudValue;

	public Gui() {
		initComponents();
    mTimer = new Timer(TIMER_CYCLE, new java.awt.event.ActionListener() {
      public void actionPerformed(java.awt.event.ActionEvent evt) {
        refreshList();
      }
    });
    refreshList();
		pass = false;
	}

	private void initComponents() {
		updateButton = new javax.swing.JButton();
		ipLabel = new javax.swing.JLabel();
		ipTextField = new javax.swing.JTextField();
		portLabel = new javax.swing.JLabel();
		portTextField = new javax.swing.JTextField();
		modeLabel = new javax.swing.JLabel();
		modeValue = new javax.swing.JLabel();
		baudLabel = new javax.swing.JLabel();
		baudValue = new javax.swing.JLabel();

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

		modeLabel.setText("Work Mode:");
		modeValue.setText("-");
		baudLabel.setText("Baud Rate:");
		baudValue.setText("-");

		listModel = new DefaultListModel();
		// Create the list and put it in a scroll pane.
		list = new JList(listModel);
		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		list.setSelectedIndex(0);
		list.setVisibleRowCount(4);
		list.addListSelectionListener(mLsl);
		JScrollPane listScrollPane = new JScrollPane(list);

		URL iconURL = getClass().getResource(
				"/com/atouch/logo-domotics-icon.png");
		ImageIcon icon = new ImageIcon(iconURL);
		setIconImage(icon.getImage());
		setLocationByPlatform(true);

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
																		modeLabel)
																.addPreferredGap(
																		javax.swing.LayoutStyle.ComponentPlacement.RELATED)
																.addComponent(
																		modeValue))
												.addGroup(
														layout.createSequentialGroup()
																.addComponent(
																		baudLabel)
																.addPreferredGap(
																		javax.swing.LayoutStyle.ComponentPlacement.RELATED)
																.addComponent(
																		baudValue))
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
				new java.awt.Component[] { updateButton, ipLabel, portLabel });

		layout.setVerticalGroup(layout
				.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
				.addGroup(
						layout.createSequentialGroup()
								.addContainerGap()
								.addGroup(
										layout.createParallelGroup(
												javax.swing.GroupLayout.Alignment.BASELINE)
												.addComponent(ipLabel)
												.addComponent(ipTextField))
								.addGroup(
										layout.createParallelGroup(
												javax.swing.GroupLayout.Alignment.BASELINE)
												.addComponent(
														portLabel,
														javax.swing.GroupLayout.PREFERRED_SIZE,
														javax.swing.GroupLayout.DEFAULT_SIZE,
														javax.swing.GroupLayout.PREFERRED_SIZE)
												.addComponent(portTextField))
								.addGroup(
										layout.createParallelGroup(
												javax.swing.GroupLayout.Alignment.BASELINE)
												.addComponent(modeLabel)
												.addComponent(modeValue))
								.addGroup(
										layout.createParallelGroup(
												javax.swing.GroupLayout.Alignment.BASELINE)
												.addComponent(baudLabel)
												.addComponent(baudValue))
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
    mTimer.start();
	}

	private void setUsr() {
		if (mSelectedUsr == null) {
			JOptionPane.showMessageDialog(this,
					"Please select the module first.", "Try again",
					JOptionPane.ERROR_MESSAGE);
			return;
		}
		try {
			Usr.update(mSelectedUsr, ipTextField.getText(),
					portTextField.getText());
		} catch (IOException e) {
			e.printStackTrace();
		}
    modeValue.setText("?");
    baudValue.setText("?");
		//try {
			//Thread.sleep(3000);
		//} catch (InterruptedException ex) {
			//Thread.currentThread().interrupt();
		//}
    listModel.clear();
    listModel.addElement("Please wait for 4-5 seconds");
	}

	private void refreshList() {
    modeValue.setText("-");
    baudValue.setText("-");
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
    mTimer.stop();
	}

	private ListSelectionListener mLsl = new ListSelectionListener() {
		@Override
		public void valueChanged(ListSelectionEvent evt) {
			if (evt.getValueIsAdjusting()) {
				// System.out.println("Eventhandler called evt.getValueIsAdjusting() true");
				return;
			} else {
				String val = (String) list.getSelectedValue();
				if ((mUsrList == null) || (val == null)
						|| (mUsrList.size() < 1))
					return;
				System.out.println("[SELECT] " + val);
				String mac = val.split(" ")[0];

        if ((mac == null) || (mac.length() != 12))
          return;
				for (Usr u : mUsrList) {
					if (!u.getMac().equals(mac))
						continue;
					ipTextField.setText(u.getIp());
					portTextField.setText(u.getPort());
					modeValue.setText(u.getMode());
					baudValue.setText(String.valueOf(u.getBaud()));
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
