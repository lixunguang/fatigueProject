
from mesh import MeshHandler

class CaseManager(object):
    def __init__(self):
        self._caseName = ['case1','case2','case3']
    
    def getCaseName(self):
        return self._caseName
        
        
GLOBAL_CM = CaseManager()
GLOBAL_MH = MeshHandler()