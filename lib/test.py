
import sys

import pdb

from PyQt4 import QtCore as qtc
from PyQt4 import QtGui as qt



#main interface
class Test(qt.QWidget):
    """Test doc"""
    def __init__(self, parent=None):
        qt.QWidget.__init__(self, parent)
        self.setGeometry(200, 200, 600, 500)
        self.setWindowTitle('Fatigue plugin')
        self.a = 101
        print id(self.a)
    def func(self):
        print id(self.a)

        


app = qt.QApplication([''])

t = Test()
t.func()
t.show()
app.exec_()