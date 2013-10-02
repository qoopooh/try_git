#!/usr/bin/env python
# licence.py - display GPL licence

# Copyright (c) 2010-2011 Algis Kabaila. All rights reserved.
# This program or module is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public Licence as published
# by the Free Software Foundation, either version 2 of the Licence, or
# version 3 of the Licence, or (at your option) any later version. It is
# provided for educational purposes and is distributed in the hope that
# it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
# the GNU General Public Licence for more details.

import sys

from PySide.QtGui import QApplication, QMainWindow, QTextEdit, QPushButton

from ui_licence import Ui_MainWindow

class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        '''Mandatory initialisation of a class.'''
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.showButton.clicked.connect(self.fileRead)
        
    def fileRead(self):
        '''Read and display GPL licence.'''
        self.textEdit.setText(open('COPYING.txt').read())
        
if __name__ == '__main__':
    app = QApplication(sys.argv)
    frame = MainWindow()
    frame.show()
    app.exec_()
    
