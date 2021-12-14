import sys
import os
import log_catch

def initEnviroment(pluginPath):
    print('initEnviroment')
    sys.path.append(pluginPath + 'Source/ThirdParty/PythonPackages')
    # sys.path.append(pluginPath + 'Source/Scripts/yolov3')
    sys.path.append(pluginPath + 'Source/ThirdParty/Scripts')
    sys.path.append(pluginPath + 'Source/ThirdParty/Scripts/yolov3')
    #log_catch.redirectLog()
    print(pluginPath + 'Source/ThirdParty/PythonPackages')
    
def initOther():
    print('initOther')
    printEnviromentPath()

def printEnviromentPath():
    env_dist = os.environ
    for key in env_dist:
        print(key + ' : ' + env_dist[key])