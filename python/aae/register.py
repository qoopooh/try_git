#!/usr/bin/env python

import os, sys, urllib
from threading import Thread

from PySide.QtCore import QObject, Slot, Signal, Property
from PySide.QtGui import QApplication
from PySide.QtDeclarative import QDeclarativeView

from aaereader import Reader

class Downloader(QObject):
    def __init__(self, url, filename=None):
        QObject.__init__(self)
        self._url = url
        if filename is None:
            filename = os.path.basename(self._url)
        self._filename = filename
        self._progress = 0.
        self._running = False
        self._size = -1

    def _download(self):
        def reporthook(pos, block, total):
            if self.size != total: # one time setting
                self._size = total
                self.on_size.emit() # notify the qml
            self.progress = float(pos*block)/float(total)

        urllib.urlretrieve(self._url, self._filename, reporthook)
        self.running = False

    @Slot()
    def start_download(self):
        if not self.running:
            self.running = True
            thread = Thread(target=self._download)
            thread.start()

    def _get_progress(self):
        return self._progress

    def _set_progress(self, progress):
        self._progress = progress
        self.on_progress.emit()

    def _get_running(self):
        return self._running

    def _set_running(self, running):
        self._running = running
        self.on_running.emit()

    def _get_filename(self):
        return self._filename

    def _get_size(self):
        return self._size

    on_progress = Signal()
    on_running = Signal()
    on_filename = Signal()
    on_size = Signal()

    progress = Property(float, _get_progress, _set_progress, notify=on_progress)
    running = Property(bool, _get_running, _set_running, notify=on_running)
    filename = Property(str, _get_filename, notify=on_filename)
    size = Property(int, _get_size, notify=on_size) # notify for QML

class Stx10(QObject):

    def __init__(self, url, connection=None):
        QObject.__init__(self)
        self._url = url
        self._conn = connection
        self._progress = 0.
        self._running = False
        self._result = ()

    def _download(self):
        def reporthook(progress, result=None):
            self.progress = progress
            if progress >= 100:
                self._result = result
                self.on_result.emit() # notify the qml

        urllib.urlretrieve(self._url, self._filename, reporthook)
        self.running = False

    @Slot()
    def start_stx10(self):
        if not self.running:
            self.running = True
            thread = Thread(target=self._download)
            thread.start()

    def _get_progress(self):
        return self._progress

    def _set_progress(self, progress):
        self._progress = progress
        self.on_progress.emit()

    def _get_running(self):
        return self._running

    def _set_running(self, running):
        self._running = running
        self.on_running.emit()

    def _get_result(self):
        return self._result


    on_progress = Signal()
    on_running = Signal()
    on_result = Signal()

    progress = Property(float, _get_progress, _set_progress, notify=on_progress)
    running = Property(bool, _get_running, _set_running, notify=on_running)
    result = Property(int, _get_result, notify=on_result) # notify for QML


if __name__ == '__main__':
    d = Downloader("http://www.advancedidasia.com/pdf/Reader_PR-510.pdf")
    app = QApplication(sys.argv)
    view = QDeclarativeView()
    view.rootContext().setContextProperty('downloader', d)
    view.setSource(__file__.replace('.py', '.qml'))
    view.show()
    app.exec_()


