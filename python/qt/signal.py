#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Signal & Slot
Also event handlers
http://zetcode.com/gui/pysidetutorial/eventsandsignals/
"""
import sys
from PySide import QtGui, QtCore
from PySide.QtGui import QLCDNumber, QSlider, QVBoxLayout
from PySide.QtCore import Qt

class Lcd(QtGui.QWidget):

    def __init__(self):
        super(Lcd, self).__init__()
        self.initUI()

    def initUI(self):
        lcd = QLCDNumber(self)
        sld = QSlider(Qt.Horizontal, self)
        sld.valueChanged.connect(lcd.display)

        vbox = QVBoxLayout()
        vbox.addWidget(lcd)
        vbox.addWidget(sld)

        self.setLayout(vbox)
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Signal & Slot')
        self.show()

    def keyPressEvent(self, e):
        if e.key() == Qt.Key_Escape:
            self.close()

def main():
    app = QtGui.QApplication(sys.argv)
    ex = Lcd()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

