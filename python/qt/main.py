#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Tutorial from http://zetcode.com/gui/pysidetutorial/
try on: 2013-09-29
"""

import sys
from PySide import QtGui, QtCore

class MainApp(QtGui.QMainWindow):
    count = 0

    def __init__(self):
        super(MainApp, self).__init__()
        self.initUI()

    def initUI(self):
        QtGui.QToolTip.setFont(QtGui.QFont('SansSerif', 10))
        self.setToolTip('This is a <b>QWidget</b> widget')

        exitAction = QtGui.QAction(QtGui.QIcon('exit.png'), 'E&xit', self)
        exitAction.setShortcut('Ctrl+Q')
        exitAction.setStatusTip('Exit Application')
        exitAction.triggered.connect(self.close)

        menuBar = self.menuBar()
        fileMenu = menuBar.addMenu('&File')
        fileMenu.addAction(exitAction)

        self.toolbar = self.addToolBar('Exit')
        self.toolbar.addAction(exitAction)

        self.statusBar().showMessage("ready")

        btnCount = QtGui.QPushButton('Count', self)
        btnCount.clicked.connect(self.buttonClicked)
        btnCount.setToolTip('Count pressing button')
        btnCount.resize(btnCount.sizeHint())
        btnCount.move(50, 75)
    
        btnQuit = QtGui.QPushButton('Quit', self)
        btnQuit.clicked.connect(QtCore.QCoreApplication.instance().quit) # no confirmation
        btnQuit.setToolTip('Terminate app')
        btnQuit.resize(btnQuit.sizeHint())
        btnQuit.move(150, 75)

        self.setGeometry(300, 300, 250, 150)
        self.center()
        self.setWindowTitle('Message box')
        self.setWindowIcon(QtGui.QIcon('ksig.png'))

        self.show()

    def center(self):
        qr = self.frameGeometry()
        cp = QtGui.QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

    def closeEvent(self, event):
        reply = QtGui.QMessageBox.question(self,
                'Confirmation', u'ต้องการจะปิดจริงหรือ?',
                QtGui.QMessageBox.Yes | QtGui.QMessageBox.No,
                QtGui.QMessageBox.No)
        if reply == QtGui.QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()

    def buttonClicked(self):
        sender = self.sender()
        self.count += 1
        self.statusBar().showMessage(sender.text() + ' was pressed '
                + str(self.count) + ' time(s)')

def main():
    app = QtGui.QApplication(sys.argv)
    ex = MainApp()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

