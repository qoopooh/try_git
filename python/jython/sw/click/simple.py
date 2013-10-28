#!/usr/bin/env jython
# -*- coding: utf-8 -*-

import sys
from javax.swing import JFrame, JPanel, JButton

class Example(JFrame):

    def __init__(self):
        super(Example, self).__init__()

        self.initUI()

    def initUI(self):

        panel = JPanel()
        panel.setLayout(None)
        panel.setToolTipText('A Panel container')

        button = JButton('Click')
        button.setBounds(120, 60, 100, 30)
        button.setToolTipText('A button component')
        panel.add(button)

        qbutton = JButton('Quit', actionPerformed=self.onQuit)
        qbutton.setBounds(10, 60, 80, 30)
        panel.add(qbutton)

        self.getContentPane().add(panel)
        self.setTitle('Simple')
        self.setSize(250, 200)
        self.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE)
        self.setLocationRelativeTo(None)
        self.setVisible(True)

    def onQuit(self, e):
        sys.exit(0)

if __name__ == '__main__':
    Example()

