#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from PySide.QtCore import QObject, Slot, QUrl, QTimer
from PySide.QtGui import *
from PySide.QtDeclarative import QDeclarativeView

# Our main window
class MainWindow(QDeclarativeView):

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setWindowTitle("Title on Main Window")
        # Renders 'view.qml'
        self.setSource(QUrl.fromLocalFile('view.qml'))
        # QML resizes to main window
        self.setResizeMode(QDeclarativeView.SizeRootObjectToView)

        con = Console()
        rotatevalue = RotateValue()

        context = self.rootContext()
        context.setContextProperty('con', con)
        context.setContextProperty('rotatevalue', rotatevalue)

        root = self.rootObject()
        root.textRotationChanged.connect(self.rotationStatus)
        button = root.findChild(QObject, 'btnMouseArea')
        button.clicked.connect(lambda: con.outputStr('click button'))

        self.timer = QTimer()
        self.timer.start(2000)
        self.timer.timeout.connect(root.updateRotater)

    def rotationStatus(self, r):
        print 'rotation ' + str(r)

class Console(QObject):
    def __init__(self):
        super(Console, self).__init__()

    @Slot(str)
    def outputStr(self, s):
        print s

class RotateValue(QObject):
    def __init__(self):
        super(RotateValue, self).__init__()
        self.r = 0

    @Slot(result=int)
    def val(self):
        self.r = self.r + 10
        return self.r


if __name__ == '__main__':
    # Create the Qt Application
    app = QApplication(sys.argv)
    # Create and show the main window
    window = MainWindow()
    window.show()
    # Run the main Qt loop
    sys.exit(app.exec_())

