#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Tutorial from http://zetcode.com/gui/pysidetutorial/menusandtoolbars/
try on: 2013-09-29
"""

import sys
from PySide import QtGui, QtCore

class Example(QtGui.QMainWindow):
    def __init__(self):
        super(Example, self).__init__()
        self.initUI()

    def initUI(self):
        QtGui.QToolTip.setFont(QtGui.QFont('SansSerif', 10))
        self.setToolTip('This is a <b>QWidget</b> widget')

        btnQuit = QtGui.QPushButton('Quit', self)
        btnQuit.clicked.connect(QtCore.QCoreApplication.instance().quit)
        btnQuit.setToolTip('Terminate app')
        btnQuit.resize(btnQuit.sizeHint())
        btnQuit.move(50, 50)

        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Message box')
        self.setWindowIcon(QtGui.QIcon('ksig.png'))

        self.show()

    def closeEvent(self, event):
        reply = QtGui.QMessageBox.question(self,
                'Confirmation', u'ต้องการจะปิดจริงหรือ?',
                QtGui.QMessageBox.Yes | QtGui.QMessageBox.No,
                QtGui.QMessageBox.No)
        if reply == QtGui.QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()

def main():
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

