
import sys


from PyQt4 import QtCore as qtc
from PyQt4 import QtGui as qt

from mesh import *
from common import GLOBAL_CM,GLOBAL_MH

class AnalisisWidget(qt.QWidget):
    """AnalisisWidget doc"""
    def __init__(self, parent=None):
        qt.QWidget.__init__(self, parent)
        #build ui
        
        self._layout = qt.QVBoxLayout(self)
        
        ##Algorithm
        self._algorithmHBox = qt.QHBoxLayout()
        self._algorithmLabel = qt.QLabel("Algorithm:")
        self._algorithmCombo = qt.QComboBox()
        self._algorithmCombo.addItems([
                             "Stress Life(Manson-Coffin)","Stress Life(Morrow)","Stress Life(BWI Weld)",\
                             "Strain Life(Manson-Coffin)","Strain Life(Morrow)","Strain Life(S-W-T)",\
                             "Safety Factor(Goodman)","Safety Factor(Gerber)"])
        
        self._algorithmHBox.addWidget(self._algorithmLabel)
        self._algorithmHBox.addWidget(self._algorithmCombo)
        self._layout.addLayout(self._algorithmHBox,1)
        
        ##Analisis Definition
        self._grpAnalysis = qt.QGroupBox("Analisis Definition")
        self._grpAnalysisVBox = qt.QVBoxLayout()
        
        self._pathLabel = qt.QLabel("Path of material:             ")
        self._pathEdit = qt.QLineEdit()
        self._pathEdit.setReadOnly(True)
        self._pathBtn= qt.QPushButton("...")
        self._pathHBox = qt.QHBoxLayout()
        
        #@
        self._meanStressLabel = qt.QLabel("Mean Stress Correction        ")
        self._meanStressCombo = qt.QComboBox()
        self._meanStressCombo.addItems(["None","Goodman","Gerber","Sodeberg"])
        self._meanStressHBox = qt.QHBoxLayout()
        #@
        
        self._inFluenceFactorsLabel =qt.QLabel("influence factors             ")
        self._inFluenceFactorsEdit = qt.QLineEdit("1.0,1.0,1.0,1.0,1.0")
        self._inFluenceFactorsEdit.setReadOnly(True)
        self._inFluenceFactorsBtn = qt.QPushButton("...")
        self._inFluenceFactorsHBox = qt.QHBoxLayout()
        
        #@
        self._weldLabel = qt.QLabel("Weld Class               ")
        self._weldCombo = qt.QComboBox()
        self._weldCombo.addItems(["B","C","D","E","F","G"])
        self._weldHBox = qt.QHBoxLayout()
        self._weldHBox.addWidget(self._weldLabel)
        self._weldHBox.addWidget(self._weldCombo)
        
        self._sdNumLabel = qt.QLabel("Num of SD             ")
        self._sdNumCombo = qt.QComboBox()
        self._sdNumCombo.addItems(["2","3","4","5"])
        self._sdNumHBox = qt.QHBoxLayout()
        self._sdNumHBox.addWidget(self._sdNumLabel)
        self._sdNumHBox.addWidget(self._sdNumCombo)
        #@
        
        self._pathHBox.addWidget(self._pathLabel)
        self._pathHBox.addWidget(self._pathEdit)
        self._pathHBox.addWidget(self._pathBtn)
        self._meanStressHBox.addWidget(self._meanStressLabel)
        self._meanStressHBox.addWidget(self._meanStressCombo)
        self._inFluenceFactorsHBox.addWidget(self._inFluenceFactorsLabel)
        self._inFluenceFactorsHBox.addWidget(self._inFluenceFactorsEdit)
        self._inFluenceFactorsHBox.addWidget(self._inFluenceFactorsBtn)
        self._grpAnalysisVBox.addLayout(self._pathHBox)
        self._grpAnalysisVBox.addLayout(self._meanStressHBox)
        self._grpAnalysisVBox.addLayout(self._weldHBox)
        self._grpAnalysisVBox.addLayout(self._sdNumHBox)
        self._grpAnalysisVBox.addLayout(self._inFluenceFactorsHBox)
        self._grpAnalysis.setLayout(self._grpAnalysisVBox)
        self._layout.addWidget(self._grpAnalysis,3)
        
        ##RainFlow Parameters
        self._grpRainFlow = qt.QGroupBox("RainFlow Parameters")
        self._grpRainFlowVBox = qt.QVBoxLayout()
        
        self._peakValleyHBox = qt.QHBoxLayout()
        self._peakValleyLabel = qt.QLabel("Peak-Valley Reduction Factor (Between 0 to 1)")
        self._peakValleyCombo = qt.QComboBox()
        self._peakValleyCombo.addItems(["0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9","1.0"])#todo
        self._peakValleyHBox.addWidget(self._peakValleyLabel)
        self._peakValleyHBox.addWidget(self._peakValleyCombo)
        
        self._countHBox = qt.QHBoxLayout()
        self._countLabel = qt.QLabel("Number of RAnges for counting(>0)")
        self._countCombo = qt.QComboBox()
        self._countCombo.addItems(["10","20"])#todo
        self._countHBox.addWidget(self._countLabel)
        self._countHBox.addWidget(self._countCombo)
        
        self._grpRainFlowVBox.addLayout(self._peakValleyHBox)
        self._grpRainFlowVBox.addLayout(self._countHBox)
        self._grpRainFlow.setLayout(self._grpRainFlowVBox)
        self._layout.addWidget(self._grpRainFlow,2)
        
        ##Entity Selection
        self._grpEntitySelection = qt.QGroupBox("Entity Selection")
        self._grpEntitySelectionVBox = qt.QVBoxLayout()
        
        self._entityTypeLabel = qt.QLabel("Entity Type                   ")
        self._entityTypeCombo = qt.QComboBox()
        self._entityTypeCombo.addItems(["Node","Element"])
        self._entityTypeHBox = qt.QHBoxLayout()
        self._entityTypeHBox.addWidget(self._entityTypeLabel)
        self._entityTypeHBox.addWidget(self._entityTypeCombo)
        
        self._entityNumLabel = qt.QLabel("Selected Entities             ")
        self._entityNumEdit = qt.QLineEdit()
        self._entityNumEdit.setReadOnly(True)

        self._entityNumBtn = qt.QPushButton("update")
        self._entityNumHBox = qt.QHBoxLayout()
        self._entityNumHBox.addWidget(self._entityNumLabel)
        self._entityNumHBox.addWidget(self._entityNumEdit)
        self._entityNumHBox.addWidget(self._entityNumBtn)
        
        self._grpEntitySelectionVBox.addLayout(self._entityTypeHBox)
        self._grpEntitySelectionVBox.addLayout(self._entityNumHBox)
        self._grpEntitySelection.setLayout(self._grpEntitySelectionVBox)
        self._layout.addWidget(self._grpEntitySelection,2)
        
        #componet
        self._InfluenceFactorDialog = InfluenceFactorDialog(self)
        self._fileDialog = qt.QFileDialog(self,"select material file")

        
        #signals
        #1 algorthm clicked,change ui
        self.connect(self._algorithmCombo, qtc.SIGNAL('currentIndexChanged(QString)'), self.algorithmChanged)
        #2 path selector dialog
        qtc.QObject.connect(self._inFluenceFactorsBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('showInfuenceFactor()'))
        #3 influence ui
        qtc.QObject.connect(self._pathBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('showFileDialog()'))
        #4 entity type
        #x=qtc.QObject.connect(self._entityTypeCombo, qtc.SIGNAL('currentIndexChanged(str)'), self, qtc.SLOT('entityTypeChanged(str)'))
        self.connect(self._entityTypeCombo, qtc.SIGNAL('currentIndexChanged(QString)'), self.entityTypeChanged) 
        #5 entity Selection clicked
        self._mesh = GLOBAL_MH
        qtc.QObject.connect(self._entityNumBtn, qtc.SIGNAL('clicked()'), self._mesh, qtc.SLOT('getSecletion()'))

    @qtc.pyqtSlot()
    def showInfuenceFactor(self):
        resStr=""
        if( qt.QDialog.Accepted == self._InfluenceFactorDialog.exec_()): 
           resStr = "%s,%s,%s,%s,%s"%(self._InfluenceFactorDialog._NotchEdit.text(),
                                     self._InfluenceFactorDialog._SurfaceEdit.text(),
                                     self._InfluenceFactorDialog._SizeEdit.text(),
                                     self._InfluenceFactorDialog._LoadEdit.text(),
                                     self._InfluenceFactorDialog._OtherhEdit.text())
           #print resStr
           self._inFluenceFactorsEdit.setText(resStr)

    @qtc.pyqtSlot()
    def showFileDialog(self):
        fileName = self._fileDialog.getOpenFileName(None,"select material file","c:/")
        #print fileName
        if(fileName != ''):
            self._pathEdit.setText(fileName)

    @qtc.pyqtSlot(str)
    def entityTypeChanged(self,arg):
        self._mesh._meshType = self._entityTypeCombo.currentText()
        

    #when algorithm Changed ,change ui
    @qtc.pyqtSlot(str)
    def algorithmChanged(self,arg):
        if self._algorithmCombo.currentIndex() in [0,1,6,7]:
            self.showStressUi()
        elif self._algorithmCombo.currentIndex() in [2]:
            self.showBwiUi()
        elif self._algorithmCombo.currentIndex() in [3,4,5]:
            self.showStrainUi()

    def showStressUi(self):
        self._pathLabel.show()
        self._pathEdit.show()
        self._pathBtn.show()
        
        self._meanStressLabel.show()
        self._meanStressCombo.show()
        
        self._weldLabel.hide()
        self._weldCombo.hide()
        self._sdNumLabel.hide()
        self._sdNumCombo.hide()
    def showBwiUi(self):
        self._pathLabel.hide()
        self._pathEdit.hide()
        self._pathBtn.hide()
        
        self._meanStressLabel.hide()
        self._meanStressCombo.hide()
        
        self._weldLabel.show()
        self._weldCombo.show()
        self._sdNumLabel.show()
        self._sdNumCombo.show()
    def showStrainUi(self):
        self._pathLabel.show()
        self._pathEdit.show()
        self._pathBtn.show()
        
        self._meanStressLabel.hide()
        self._meanStressCombo.hide()
        
        self._weldLabel.hide()
        self._weldCombo.hide()
        self._sdNumLabel.hide()
        self._sdNumCombo.hide()

    def writeToFile(self,str):
        fw = InputFileWriter()
        #todo:


class InfluenceFactorDialog(qt.QDialog):
    """InfluenceFactorDialog doc"""
    def __init__(self, parent=None):
        qt.QDialog.__init__(self, parent)
        #build ui
        self.setGeometry(100, 100, 400, 250)
        self.setWindowTitle('Influence Factor')
        self._layout = qt.QVBoxLayout(self)
        
        self._NotchLabel = qt.QLabel("Notch Factor(kf,kt)")
        self._NotchEdit = qt.QLineEdit("1.0")        
        self._Notchlayout = qt.QHBoxLayout()
        self._Notchlayout.addWidget(self._NotchLabel)
        self._Notchlayout.addWidget(self._NotchEdit)

        self._SurfaceLabel = qt.QLabel("Surface Factor(ms)")
        self._SurfaceEdit = qt.QLineEdit("1.0")
        self._Surfacelayout = qt.QHBoxLayout()
        self._Surfacelayout.addWidget(self._SurfaceLabel)
        self._Surfacelayout.addWidget(self._SurfaceEdit)
        
        self._SizeLabel = qt.QLabel("Size Factor(md)")
        self._SizeEdit = qt.QLineEdit("1.0")
        self._Sizelayout = qt.QHBoxLayout()
        self._Sizelayout.addWidget(self._SizeLabel)
        self._Sizelayout.addWidget(self._SizeEdit)
        
        self._LoadLabel = qt.QLabel("Load Factor(mt)")
        self._LoadEdit = qt.QLineEdit("1.0")
        self._Loadlayout = qt.QHBoxLayout()
        self._Loadlayout.addWidget(self._LoadLabel)
        self._Loadlayout.addWidget(self._LoadEdit)
        
        self._OtherLabel = qt.QLabel("Other Factor(mo)")
        self._OtherhEdit = qt.QLineEdit("1.0")
        self._Otherlayout = qt.QHBoxLayout()
        self._Otherlayout.addWidget(self._OtherLabel)
        self._Otherlayout.addWidget(self._OtherhEdit)
        
        self._layout.addLayout(self._Notchlayout)
        self._layout.addLayout(self._Surfacelayout)
        self._layout.addLayout(self._Sizelayout)
        self._layout.addLayout(self._Loadlayout)
        self._layout.addLayout(self._Otherlayout)
        
        self._buttonBox = qt.QDialogButtonBox(qt.QDialogButtonBox.Ok| qt.QDialogButtonBox.Cancel)
        self._layout.addWidget(self._buttonBox)
        self.setLayout(self._layout)
        
        #signals
        qtc.QObject.connect(self._buttonBox, qtc.SIGNAL("accepted()"), self, qtc.SLOT("accept()") )
        qtc.QObject.connect(self._buttonBox, qtc.SIGNAL("rejected()"), self, qtc.SLOT("reject()") )
        


class LoadWidget(qt.QWidget):
    """LoadWidget doc"""
    def __init__(self, parent=None):
        qt.QWidget.__init__(self, parent)

        self._layout = qt.QVBoxLayout(self)
        self._grpLoad = qt.QGroupBox("Define Loading(s)")
        self._grpLoadVBox = qt.QVBoxLayout()
        
        self._loadTypeHBox = qt.QHBoxLayout()
        self._loadTypeLabel = qt.QLabel("Loading Type:    ")
        self._loadTypeCombo = qt.QComboBox()
        self._loadTypeCombo.addItems(["Duty Cycle","Loading Hidstory","Superimposed"])#todo
        self._loadTypeHBox.addWidget(self._loadTypeLabel)
        self._loadTypeHBox.addWidget(self._loadTypeCombo)
        
        self._loadEventHBox = qt.QHBoxLayout()
        self._loadEventLabel = qt.QLabel("Loading Events:      ")
        self._loadEventEdit = qt.QLineEdit()
        self._loadEventBtn = qt.QPushButton("...")
        self._loadEventHBox.addWidget(self._loadEventLabel)
        self._loadEventHBox.addWidget(self._loadEventEdit)
        self._loadEventHBox.addWidget(self._loadEventBtn)
        
        self._grpLoadVBox.addLayout(self._loadTypeHBox)
        self._grpLoadVBox.addLayout(self._loadEventHBox)
        self._spacer = qt.QSpacerItem(100, 300, qt.QSizePolicy.Minimum)
        self._grpLoadVBox.addSpacerItem(self._spacer)
        
        
        self._grpLoad.setLayout(self._grpLoadVBox)
        self._layout.addWidget(self._grpLoad)
        
        self._loadEventDialog = None
        self._loadEventDialog_duty = LoadEventDialog("Duty Cycle")
        self._loadEventDialog_history = LoadEventDialog("Loading Hidstory")
        self._loadEventDialog_super = LoadEventDialog("Superimposed")
        #
        qtc.QObject.connect(self._loadEventBtn, qtc.SIGNAL('clicked()'), self, qtc.SLOT('showLoadEventWidget()'))
        qtc.QObject.connect(self._loadTypeCombo, qtc.SIGNAL('currentIndexChanged(QString)'), self.typeChanged)
        
    @qtc.pyqtSlot()
    def showLoadEventWidget(self):
        if self._loadTypeCombo.currentText() == "Duty Cycle":
            self._loadEventDialog = self._loadEventDialog_duty
        elif self._loadTypeCombo.currentText() == "Loading Hidstory":
            self._loadEventDialog = self._loadEventDialog_history
        elif self._loadTypeCombo.currentText() == "Superimposed":
            self._loadEventDialog = self._loadEventDialog_super

        if( qt.QDialog.Accepted == self._loadEventDialog.exec_()): 
            self._loadEventEdit.setText(str(len(self._loadEventDialog._tabwidget._model.modelData()))+' envents')

    @qtc.pyqtSlot(str)
    def typeChanged(self,arg):
        pass
        #print arg
    
    
class LoadEventDialog(qt.QDialog):
    """LoadEventWidget doc"""
    def __init__(self, type,parent = None):
        qt.QDialog.__init__(self, parent)
        self.setGeometry(100, 100, 600, 400)
        self.setWindowTitle('Loading Events')
        
        self._buttonLayout = qt.QHBoxLayout()
        self.cancelBtn = qt.QPushButton('cancel')
        self.okBtn = qt.QPushButton('ok')
        self._buttonLayout.addWidget(self.cancelBtn)
        self._buttonLayout.addWidget(self.okBtn)
        
        self._layout = qt.QVBoxLayout(self)
        self._tabwidget = Table(type)
        self._layout.addWidget(self._tabwidget)
        self._layout.addLayout(self._buttonLayout)
        
        qtc.QObject.connect(self.okBtn, qtc.SIGNAL('clicked()'), self.accept)
        qtc.QObject.connect(self.cancelBtn, qtc.SIGNAL('clicked()'), self.reject)
    def setText(self,text):
        self._tabwidget.setText(text)

    def __del__(self):
        #print "LoadEventDialog __del__"
        pass


#tableview
class Table(qt.QWidget):
    """    """
    def __init__(self, type,parent = None):
        qtc.QObject.__init__(self,parent)

        self._layout = qt.QVBoxLayout(self)
        
        self._grp = qt.QGroupBox()
        self._grpLayout = qt.QVBoxLayout(self._grp)
        
        self._toolAddBtn = qt.QPushButton("add")
        self._toolRemoveBtn = qt.QPushButton("remove")
        self._toolUpBtn = qt.QPushButton("up")
        self._toolDownBtn = qt.QPushButton("down")
        self._toolLayout = qt.QHBoxLayout()

        self._toolLayout.addWidget(self._toolAddBtn)
        self._toolLayout.addWidget(self._toolRemoveBtn)
        self._toolLayout.addWidget(self._toolUpBtn)
        self._toolLayout.addWidget(self._toolDownBtn)

        self._spacer = qt.QSpacerItem(100, 5, qt.QSizePolicy.Minimum)
        self._toolLayout.addSpacerItem(self._spacer)
        
        self._table = qt.QTableView()
        self._model = Model(type)
        #print "self._model",self._model
        self._table.setModel(self._model)
        
        self._grpLayout.addLayout(self._toolLayout)
        self._grpLayout.addWidget(self._table)

        self._table.setItemDelegate(Delegate(self))
        self._layout.addWidget(self._grp)

        #tool button  clicked
        self.connect(self._toolAddBtn, qtc.SIGNAL('clicked()'), self.onAddClicked)
        self.connect(self._table.verticalHeader() , qtc.SIGNAL('sectionClicked(int)'), self.onRemove)
        self.connect(self._toolUpBtn, qtc.SIGNAL('clicked()'), self.onUpClicked)
        self.connect(self._toolDownBtn, qtc.SIGNAL('clicked()'), self.onDownClicked)
        
        
    def setText(self,text):
        self._grp.setTitle(text)
        
    def text(self,text):
        return self._grp.title()
        
    def onAddClicked(self):
        cnt = len(self._model.modelData())
        self._model.beginInsertRows(qtc.QModelIndex(), cnt, cnt)
        if(self._model._type == 'Duty Cycle'):
            self._model.modelData().append('Event%d## ## ## ## '%(self._model._eventID))
        elif(self._model._type == 'Loading Hidstory'):
            self._model.modelData().append('Event%d## ## '%(self._model._eventID))
        elif(self._model._type == 'Superimposed'):
            self._model.modelData().append('Event%d## ## '%(self._model._eventID))
        self._model._eventID += 1
        self._model.endInsertRows()

    def onRemove(self,idx):
        self._model.beginRemoveRows(qtc.QModelIndex(), idx, idx)
        del self._model.modelData()[idx]
        self._model.endRemoveRows()
        
    def onUpClicked(self):
        midx = self._table.currentIndex()
        r = midx.row()
        
        r1=0
        if r-1>=0:
           r1 = r-1
        else:
           r1 = 0
        if r>0 and r1!=r:
            self._model.beginMoveRows(qtc.QModelIndex(), r, r,qtc.QModelIndex(),r1)
            #change
            first = self._model.modelData()[r]
            second = self._model.modelData()[r1]
            self._model.modelData()[r] = second
            self._model.modelData()[r1] = first

            self._model.endMoveRows()
        
    def onDownClicked(self):
        print "down" 
        midx = self._table.currentIndex()
        r = midx.row()
        
        r1= r+1

        if r1 <= len(self._model.modelData())-1:
           pass
        else:
           r1 = r

        if r1 <= len(self._model.modelData())-1 and r1!=r:
            self._model.beginMoveRows(qtc.QModelIndex(), r1, r1,qtc.QModelIndex(),r)
        #change
            first = self._model.modelData()[r]
            second = self._model.modelData()[r1]
            self._model.modelData()[r] = second
            self._model.modelData()[r1] = first

            self._model.endMoveRows()


#model
#data=["aaa##bbb#z#23#","89##888##kkk##"]
class Model(qtc.QAbstractTableModel):
    """
    """

    def __init__(self,type):
        qtc.QAbstractTableModel.__init__(self)
        self._h = ['Event','Result Set','Scale','Scale History','Repeats','Radio']

        self._modelData_duty = []
        self._modelData_Hidstory = []
        self._modelData_Super = []
        self._headers= []
        self._type = type
        if(type == 'Duty Cycle'):
            self._headers = [self._h[0],self._h[1],self._h[2],self._h[4],self._h[5]]
        elif(type == 'Loading Hidstory'):
            self._headers = [self._h[0],self._h[1],self._h[2]]
        elif(type == 'Superimposed'):
            self._headers = [self._h[0],self._h[1],self._h[3]]
            
        self._eventID = 1

    def modelData(self):
        if(self._type == 'Duty Cycle'):
            return self._modelData_duty
        elif(self._type == 'Loading Hidstory'):
            return self._modelData_Hidstory
        elif(self._type == 'Superimposed'):
            return self._modelData_Super
            
    def rowCount(self, parent_midx):
        """Return the number of rows to Qt mechanism when the table
        changes"""
        return len(self.modelData())

    def columnCount(self, parent_midx):
        """Return the number of columns to Qt mechanism when the table
        changes"""
        return len(self._headers)
        
    def data(self, midx, role):
        """Provide data for each cell in the display role"""
        res = None
        if role in (qtc.Qt.DisplayRole, qtc.Qt.EditRole):
            text = self.modelData()[midx.row()]
            #print text,midx.row(),midx.column()
            l = text.split('##')
            #print l
            res = l[midx.column()]
        return res
    '''
    def setData(self, midx, value, role):
        if role == qtc.Qt.EditRole :
            print "set data "+str(value)
        return True
    '''
    def headerData(self, sidx, orientation, role):
        """Tell to Qt mechanism the header data for building table view"""
        res = None
        if role == qtc.Qt.DisplayRole and orientation == qtc.Qt.Horizontal:
             res = self._headers[sidx]
        elif role == qtc.Qt.DisplayRole and orientation == qtc.Qt.Vertical:
             res =  "remove"

        return res
        

    def flags(self, midx):
        """Tell to Qt mechanism that each cell is enabled and editable"""
        #readonly
        return qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled




class Delegate(qt.QStyledItemDelegate):
    """Delegate
    """
    def __init__(self,tab):
        qt.QStyledItemDelegate.__init__(self)
        self._tab = tab._table
        self._model = tab._model

    def createEditor(self, parent, options, midx):
        """Create a QLineEdit"""

        if midx.column() == 1:
            caseNames = GLOBAL_CM.getCaseName()
            #print 'createEditor',caseNames
            comb = qt.QComboBox(parent)
            comb.addItems(caseNames)
            return comb
        else:
            return qt.QLineEdit(parent)

    def setEditorData(self, control, midx):
        """Set the group names and select the current one"""
        #set val
        text = self._model.modelData()[midx.row()]
        l = text.split('##')
        if midx.column() == 1:
            caseNames = GLOBAL_CM.getCaseName()
            if l[midx.column()] != ' ':
                control.setCurrentIndex(caseNames.index(l[midx.column()]))
        else:
            control.setText(l[midx.column()])
        
        
    def setModelData(self, control, model, midx):
        """Set the selected group on selector and update wizard"""
        ori_text = self._model.modelData()[midx.row()]
        l = ori_text.split('##')
        if midx.column() == 1:
            l[midx.column()] = str(control.currentText())
        else:
            l[midx.column()] = str(control.text())
        s='##'
        s=s.join(l)

        self._model.modelData()[midx.row()] = s
        

