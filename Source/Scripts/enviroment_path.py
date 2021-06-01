import sys

def initEnviroment(pluginPath):
    sys.path.append(pluginPath + 'PythonVM/Source/ThirdParty/PythonPackages')
    sys.path.append(pluginPath + 'PythonVM/Source/Scripts/yolov3')
    
    
def initOther():
    pass