#!/usr/bin/env python

import sys, serial
from PySide import QtGui, QtCore
from ui_main import Ui_Main

class SerialThread(QtCore.QThread):
    dataReady = QtCore.Signal(str)

    def __init__(self):
        self.s = serial.Serial('/dev/ttyUSB0')

    def run(self):
        while True:
            self.dataReady.emit(s.readline())      

class Main(QtGui.QMainWindow):
    def __init__(self):
        super(Main, self).__init__()
        self.ui = Ui_Main()
        self.ui.setupUi(self)

        self.st = SerialThread()
        self.st.dataReady.connect(self.show_string, QtCore.Qt.QueuedConnection)

        self.show()

    def show_string(self, string):
        self.ui.lineEdit.setText(string)

    def main():
        app = QtGui.QApplication(sys.argv)
        m = Main()
        sys.exit(app.exec_())

if __name__ == '__main__':
    main()

