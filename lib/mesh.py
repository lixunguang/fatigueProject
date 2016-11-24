
from PyQt4 import QtCore as qtc
from PyQt4 import QtGui as qt
#from paraview.simple import *


#interactive with paraview mesh
class MeshHandler(qtc.QObject):
    def __init__(self):
        qtc.QObject.__init__(self)
        #paraview.print_debug_info("MeshHandler init")
        self._meshType = "Node"
        self._selection =[]
    @qtc.pyqtSlot()
    def getSecletion(self):
        if (self._meshType == 'Node'):
            #paraview.print_debug_info('Node')
            #sel = servermanager.ProxyManager().GetProxy("sources", "ExtractSelection1")
            #sel.UpdatePipeline()
            #cnt = 0
            #for i in sel.Selection.IDs:
                #if (cnt +1)%3 ==0:
                    #paraview.print_debug_info(sel.Selection.IDs[cnt])
                    #self._selection.append(sel.Selection.IDs[cnt])
                #cnt += 1
                
            pass
        elif(self._meshType == 'Element'):
            #paraview.print_debug_info('Element')
            #sel = servermanager.ProxyManager().GetProxy("sources", "ExtractSelection1")
            #sel.UpdatePipeline()
            #paraview.print_debug_info(sel.Selection.IDs)
            pass
