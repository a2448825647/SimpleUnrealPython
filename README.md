# SimpleUnrealPythonRuntime
引擎版本：UE4.26
接入的Python版本为Python3.9

简易运行时Python调用的插件，打包测试成功

配置方法：
Source/Scripts/enviroment_path.py 内配置额外的python环境变量路径
目前接口：
RecreatePythonEnvironment
RunPythonString
RunPythonFile  //尚未解决CRTsecurity问题，无法使用
ImportPythonModule
CallEmptyFunction
CallStringFunctionWithStringParam
CallStringFunction
仅C++调用的接口：
GetOrLoadModule
GetOrLoadFunction
CallFunction
GetEmptyArgs
PyObjectToString

蓝图使用方法：
![image](https://github.com/a2448825647/UnrealPythonVM/blob/main/Resources/1.png)
