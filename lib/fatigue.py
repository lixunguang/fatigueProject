
import sys
import pdb

from PyQt4 import QtCore as qtc
from PyQt4 import QtGui as qt
import os
import inspect

print sys.path[0] 
sys.path.append('E:/workspace/fatigueProject/lib')

from uiCommon import *
from fileCommon import InputFileWriter,InputFileReader

#main interface
class FatiguePluginWidget(qt.QWidget):
    """FatiguePluginWidget doc"""
    def __init__(self, parent=None):

        qt.QWidget.__init__(self, parent)
        self.setGeometry(200, 200, 600, 500)
        self.setWindowTitle('Fatigue plugin')
        
        self._layout = qt.QVBoxLayout(self)
        
        self._tab = qt.QTabWidget(self)
        self._analisisWidget = AnalisisWidget()
        self._loadWidget = LoadWidget()
        
        self._finishBtn = qt.QPushButton('write to file')
        
        self._tab.addTab(self._analisisWidget,"analisis settings")
        self._tab.addTab(self._loadWidget,"load settings")
        
        self._layout.addWidget(self._tab)
        self._layout.addWidget(self._finishBtn)
        
        qtc.QObject.connect(self._finishBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('onFinish()'))
        
    @qtc.pyqtSlot()
    def onFinish(self):
        ifw = InputFileWriter(self._tab)
        ifw.WriteToFile()


app = qt.QApplication([''])

mainWidget = FatiguePluginWidget()
#print inspect.stack()
#print  fatigue.__file__
mainWidget.show()
#app.exec_()