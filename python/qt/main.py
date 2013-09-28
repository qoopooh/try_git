#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Tutorial from http://zetcode.com/gui/pysidetutorial/menusandtoolbars/
try on: 2013-09-29
"""

import sys
from PySide import QtGui

class Example(QtGui.QMainWindow):
    def __init__(self):
        super(Example, self).__init__()
        self.initUI()

    def initUI(self):
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Icon')
        self.setWindowIcon(QtGui.QIcon('ksig.png'))

        self.show()

def main():
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

