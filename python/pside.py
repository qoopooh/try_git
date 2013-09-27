import sys
from PySide import QtGui

app = QtGui.QApplication(sys.argv)
win = QtGui.QWidget()
win.resize(320,240)
win.setWindowTitle('Hi PySide')
win.show()

sys.exit(app.exec_())


