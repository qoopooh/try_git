#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Editor form
"""
import sys
from PySide import QtGui
from PySide.QtGui import QApplication, QLabel, QLineEdit, QTextEdit, QGridLayout

class Editor(QtGui.QWidget):
    def __init__(self):
        super(Editor, self).__init__()
        self.initUI()

    def initUI(self):
        title = QLabel('Title')
        author = QLabel('Author')
        review = QLabel('Review')

        titleEdit = QLineEdit()
        authorEdit = QLineEdit()
        reviewEdit = QTextEdit()

        btnOk = QtGui.QPushButton('OK')
        btnCancel = QtGui.QPushButton('Cancel')
        btnCancel.clicked.connect(self.close)

        hbox = QtGui.QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(btnOk)
        hbox.addWidget(btnCancel)
        
        grid = QGridLayout()
        grid.setSpacing(10)
        grid.addWidget(title, 1, 0)
        grid.addWidget(titleEdit, 1, 1)
        grid.addWidget(author, 2, 0)
        grid.addWidget(authorEdit, 2, 1)
        grid.addWidget(review, 3, 0)
        grid.addWidget(reviewEdit, 3, 1, 5, 1)
        grid.addLayout(hbox, 8, 1)
        self.setLayout(grid)
        
        self.setGeometry(300, 300, 350, 300)
        self.setWindowTitle('Review')
        self.setWindowIcon(QtGui.QIcon('kpdf.png'))
        self.show()

def main():
    app = QApplication(sys.argv)
    w = Editor()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()

