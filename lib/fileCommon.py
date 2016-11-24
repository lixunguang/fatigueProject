
import xml.etree.cElementTree as et
import random

from common import GLOBAL_CM,GLOBAL_MH

class WriteObj(object):
    def __init__(self,tag,text):
        self.__element = et.Element(tag)
        self.__element.text = text
        
    def getElement(self):
        return self.__element
        
        
    def addAttrib(self,key,val):
        #merge dict
        self.__element._attr[key] =  val

    
class AlgorithmSection(WriteObj):
    def __init__(self,text):
        WriteObj.__init__(self,'Algorithm',text)
    
class AnalisisDefSection(WriteObj):
    def __init__(self, idx, text, dic):
        WriteObj.__init__(self,'AnalisisParas',text)
        
        for i in dic.keys():
            elem = et.Element(i)
            if isinstance(dic[i],dict):
                for j in dic[i].keys():
                    e = et.Element("Param")
                    e.attrib[j] = dic[i][j] 
                    elem.append(e)
            else:
                elem.attrib[i] =  dic[i]
            self.getElement().append(elem)


class RainFlowSection(WriteObj):
    def __init__(self, text, dic):
        WriteObj.__init__(self,'RainflowParas',text)
        
        #build element tree from dict
        for i in dic.keys():
            elem = et.Element("Param")
            elem.attrib[i] =  dic[i]
            self.getElement().append(elem)

class CaseSection(WriteObj):
    '''
    indexs: node or element list
    caseNames: casename list
    '''
    def __init__(self, text, dic,caseNames, indexs):
        WriteObj.__init__(self,'Case',text)
        
        for i in dic.keys():
            self.getElement().attrib[i] =  dic[i]

        for eachCase in caseNames:
            elem = et.Element("Item")
            elem.attrib['caseName'] =  eachCase
            for id in indexs:
               e = et.Element("d")
               e.text = "%d,%d,%d,%d,%d,%d,%d"%(id,random.random(),random.random(),random.random(),random.random(),random.random(),random.random())
               elem.append(e)
               
            self.getElement().append(elem)
            

class LoadSection(WriteObj):
    def __init__(self, text, dic,table):
        WriteObj.__init__(self,'Load',text)
        #print table
        for i in dic.keys():
            self.getElement().attrib[i] =  dic[i]
        
        ####handle table
        type = dic['LoadingType']
        for i in table:
            l = i.split('##')
            e = et.Element("Item")
            e.attrib['caseName'] = l[1]
            e.text=','.join(l[2:])
            self.getElement().append(e)
        
class InputFileWriter(object):
    def __init__(self,tab):
        #print "InputFileWriter init"
        self._analisisWidget = tab.widget(0)
        self._loadWidget = tab.widget(1)
        
        self._tree = et.ElementTree()#element tree
        self._treeRoot = et.Element('input')# tree Root
        self._tree._setroot(self._treeRoot)
        
        self._conf = et.Element('Configure')
        self._treeRoot.append(self._conf)

    def WriteToFile(self):
        self.updateFromUI()
        self._tree.write("c:/inputfile.xml")
        

    def updateFromUI(self):
        #stp1: clear the root
        self.ClearSubElement()
        
        #stp2: update ui to obj
        self._algorithm = str(self._analisisWidget._algorithmCombo.currentText())
        self._path = str(self._analisisWidget._pathEdit.text())
        self._meanStress = str(self._analisisWidget._meanStressCombo.currentText())
        self._inFluenceFactors = str(self._analisisWidget._inFluenceFactorsEdit.text())
        self._weld = str(self._analisisWidget._weldCombo.currentText() )
        self._sdNum = str(self._analisisWidget._sdNumCombo.currentText() )
        self._peakValley = str(self._analisisWidget._peakValleyCombo.currentText() )
        self._count = str(self._analisisWidget._countCombo.currentText() )
        self._entityType = str(self._analisisWidget._entityTypeCombo.currentText()) 
        self._entityNum = str(self._analisisWidget._entityNumEdit.text())
        
        self.loadType = str(self._loadWidget._loadTypeCombo.currentText() )
        self.loadEvent = str(self._loadWidget._loadEventEdit.text())
        
        #stp3: build tree
        #
        self.addConfigueElement(AlgorithmSection(self._algorithm).getElement())
        #
        dic={}
        idx = self._analisisWidget._algorithmCombo.currentIndex()
        if idx in [0,1,6,7]:
            #self.showStressUi()
            dic['MaterialPath'] = self._path
            dic['MeanStress'] = self._meanStress
        elif idx in [2]:
            #self.showBwiUi()
            dic['WeldClass'] = self._weld
            dic['NumSD'] = self._sdNum
        elif idx in [3,4,5]:
            #self.showStrainUi()
            dic['MaterialPath'] = self._path
            
        sl = self._inFluenceFactors.split(',')
        ss = ['kt','ms','md','mt','mo']
        s = zip(ss,sl)

        dic['InfluenceFactors'] = dict(s)
        self.addConfigueElement(AnalisisDefSection(idx,'', dic).getElement())
        
        #
        dic.clear()
        dic['NumRange'] = self._count
        dic['PvGate'] = self._peakValley
        self.addConfigueElement(RainFlowSection('', dic).getElement())

        #self._case
        dic.clear()
        dic['ResultEntity'] = self._entityType
        dic['NumEntity'] = self._entityNum
        
        
        caseNames = GLOBAL_CM.getCaseName()
        indexs = GLOBAL_MH._selection
        self.addCaseElement(CaseSection('', dic, caseNames, indexs).getElement())
        
        #self._load
        dic.clear()
        dic['LoadingType'] = self.loadType
        dic['Unit'] = "Mpa"
        table = self._loadWidget._loadEventDialog._tabwidget._model.modelData()
        self.addLoadElement(LoadSection('',dic,table).getElement())
        
        
    def addConfigueElement(self,element):
        self._conf.append(element)
        
    def addCaseElement(self,element):
        self._treeRoot.append(element)
        
    def addLoadElement(self,element):
        self._treeRoot.append(element)
        
        
    def ClearSubElement(self):
        #memroy manager
        for i in self._conf:
            del i


            
#read exist input file for edit and check
class InputFileReader(object):
    def __init__(self,file):
        #print "InputFileReader init"
        self.tree = et.ElementTree(file)
        self._tab = tab
        
    def updateToUI(self):
        #update obj to ui
        pass
        
