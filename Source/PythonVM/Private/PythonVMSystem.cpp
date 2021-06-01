// Fill out your copyright notice in the Description page of Project Settings.


#include "PythonVMSystem.h"
#include <string>
#include "PythonVm.h"



PyObject* mPyModule_log_catch = nullptr;
PyObject* mPyFunction_getLog = nullptr;
PyObject* mPyFunction_clearLog = nullptr;
PyObject* mPyArgs = nullptr;

void UPythonVMSystem::Initialize(FSubsystemCollectionBase& Collection) {
	InitializePython();
	Super::Initialize(Collection);
}

void UPythonVMSystem::Deinitialize() {
	FinalizePython();
	Super::Deinitialize();
}

void UPythonVMSystem::InitializePython() {
	if (!Py_IsInitialized()) {
		Py_Initialize();
#if WITH_EDITOR
	}{
#endif
		FString AbsPluginDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "ProjectPluginDir:" + FPaths::ConvertRelativePathToFull( FPaths::ProjectPluginsDir()));
		FString ExecScript = "import sys";
		ExecScript.Append(LINE_TERMINATOR);
		ExecScript.Append("sys.path.append('");
		ExecScript.Append(AbsPluginDir / "PythonVM/Source/Scripts");
		ExecScript.Append("')");
		PyRun_SimpleString(TCHAR_TO_UTF8(*ExecScript));

		mPyArgs = Py_BuildValue("");

		CallFunction("log_catch::redirectLog");
		CallStringFunctionWithStringParam("enviroment_path::initEnviroment", AbsPluginDir);
		CallFunction("enviroment_path::initOther");
	}
}

void UPythonVMSystem::FinalizePython() {
	if (Py_IsInitialized()) {
		SAFE_DELETE_PYOBJECT(mPyModule_log_catch);
		SAFE_DELETE_PYOBJECT(mPyFunction_getLog);
		SAFE_DELETE_PYOBJECT(mPyFunction_clearLog);
		SAFE_DELETE_PYOBJECT(mPyArgs);
		TArray<FPyModule> ValueAry;
		for (auto& Ite : Modules) {
			for (auto& Ite2 : Ite.Value.Functions) {
				SAFE_DELETE_PYOBJECT(Ite2.Value);
			}
			SAFE_DELETE_PYOBJECT(Ite.Value.Module);
		}
		Modules.Empty();
#if !WITH_EDITOR
		Py_Finalize();
#endif
	}
}

void UPythonVMSystem::RecreatePythonEnvironment() {
	FinalizePython();
	InitializePython();
}

void UPythonVMSystem::RunPythonString(const FString& Str) {
	if (Str.IsEmpty()) return;
	//PyRun_SimpleString(std::string(TCHAR_TO_UTF8(*Str)).c_str());
	PyRun_SimpleString(TCHAR_TO_UTF8(*Str));
	PrintPythonLog();
}

void UPythonVMSystem::RunPythonFile(const FString& ModuleName) {
	//FString CallString = ModuleName + "::__main__";
	//CallFunction(CallString);
}

bool UPythonVMSystem::ImportPythonModule(FString ModuleName) {
	bool Ret = ImportPythonModuleImpl(ModuleName);
	PrintPythonLog();
	return Ret;
}

void UPythonVMSystem::CallEmptyFunction(FString FunctionName) {
	CallFunction(FunctionName);
}

FString UPythonVMSystem::CallStringFunctionWithStringParam(FString FunctionName, FString Param) {
	FString Ret;
	PyObject* Args = Py_BuildValue("(s)", TCHAR_TO_UTF8(*Param));
	PyObject* Result = CallFunction(FunctionName, Args);
	SAFE_DELETE_PYOBJECT(Args);
	if (Result) {
		Ret = PyObjectToString(Result);
		SAFE_DELETE_PYOBJECT(Result);
	}
	return Ret;
}

FString UPythonVMSystem::CallStringFunction(FString FunctionName) {
	FString Ret;
	PyObject* Result = CallFunction(FunctionName);
	if (Result) {
		Ret = PyObjectToString(Result);
		SAFE_DELETE_PYOBJECT(Result);
	}
	return Ret;
}


PyObject* UPythonVMSystem::GetOrLoadModule(FString ModuleName) {
	if (ImportPythonModuleImpl(ModuleName)) {
		return Modules.FindRef(ModuleName).Module;
	}
	return nullptr;
}

PyObject* UPythonVMSystem::GetOrLoadFunction(FString FunctionName) {
	FString Left, Right;
	if (SplitFunctionName(FunctionName, Left, Right)) {
		if (ImportPythonModuleImpl(Left)) {
			FPyModule* ModulePtr = Modules.Find(Left);
			if (ModulePtr) {
				PyObject** FuncPtr = ModulePtr->Functions.Find(Right);
				if (FuncPtr) {
					return *FuncPtr;
				}
				else {
					PyObject* Func = PyObject_GetAttrString(ModulePtr->Module, TCHAR_TO_UTF8(*Right));
					if (Func) {
						ModulePtr->Functions.Add(Right, Func);
						return Func;
					}
				}
			}
		}
	}
	return nullptr;
}

PyObject* UPythonVMSystem::CallFunction(FString FunctionName, PyObject* Args, PyObject* Kwargs) {
	PyObject* FuncPtr = GetOrLoadFunction(FunctionName);
	if (FuncPtr) {
		if (Args == nullptr) Args = mPyArgs;
		PyObject* Result = PyObject_Call(FuncPtr, Args, Kwargs);
		PrintPythonLog();
		return Result;
	}
	return nullptr;
}

const PyObject* UPythonVMSystem::GetEmptyArgs() {
	return mPyArgs;
}

FString UPythonVMSystem::PyObjectToString(PyObject* Object) {
	FString Ret;
	if (Object) {
		Py_ssize_t Size = 0;
		const char* CharRet = PyUnicode_AsUTF8AndSize(Object, &Size);
		if (Size > 0 && CharRet != nullptr) {
			Ret = CharRet;
		}
	}
	return Ret;
}

void UPythonVMSystem::PrintPythonLog() {
	PyObject* GetLogPtr = GetOrLoadFunction("log_catch::getLog");
	PyObject* ClearLogPtr = GetOrLoadFunction("log_catch::clearLog");
	if (GetLogPtr && ClearLogPtr) {
		PyObject* Result = PyObject_Call(GetLogPtr, mPyArgs, nullptr);
		if (Result) {
			FString LogStr = PyObjectToString(Result);
			if (!LogStr.IsEmpty()) {
				UE_LOG(LogPythonVM, Display, TEXT("PythonLevel:\n%s"), *LogStr);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, LogStr);
				PyObject_Call(ClearLogPtr, mPyArgs, nullptr);
			}
			SAFE_DELETE_PYOBJECT(Result);
		}
	}
}

bool UPythonVMSystem::SplitFunctionName(FString In, FString& OutModuleName, FString& OutFunctionName) {
	if (!In.Contains("::")) {
		UE_LOG(LogPythonVM, Error, TEXT("Wrong FunctionName. eg: ModuleName::FunctionName"));
		return false;
	}
	return In.Split("::", &OutModuleName, &OutFunctionName);
}

bool UPythonVMSystem::ImportPythonModuleImpl(FString ModuleName) {
	if (!Modules.Contains(ModuleName)) {
		PyObject* mPyModule = PyImport_ImportModule(TCHAR_TO_UTF8(*ModuleName));
		if (mPyModule) {
			FPyModule NewModule;
			NewModule.Module = mPyModule;
			UE_LOG(LogPythonVM, Display, TEXT("Import python module: %s"),*ModuleName);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Import python module:" + ModuleName);
			Modules.Add(ModuleName, NewModule);
			return true;
		}
		UE_LOG(LogPythonVM, Error, TEXT("Import python module failed: %s"), *ModuleName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Import python module failed:" + ModuleName);
		return false;
	}
	return true;
}
