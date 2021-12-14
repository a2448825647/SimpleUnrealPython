// Fill out your copyright notice in the Description page of Project Settings.


#include "PythonVMSystem.h"
#include <string>
#include "PythonVm.h"
#include "Projects/Public/Interfaces/IPluginManager.h"
#include "PyModule_uelog.h"



void UPythonVMSystem::Initialize(FSubsystemCollectionBase& Collection) {
	//InitializePython();
	Super::Initialize(Collection);
}

void UPythonVMSystem::Deinitialize() {
	//FinalizePython();
	Super::Deinitialize();
}

void UPythonVMSystem::InitializePython() {
	if (!Py_IsInitialized()) {
		PyImport_AppendInittab("uelog", PyInit_uelog);
		Py_Initialize();
		FString AbsPluginDir = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("PythonVM"))->GetBaseDir() + "/");
		FString ExecScript = "import sys";
		ExecScript.Append(LINE_TERMINATOR);
		ExecScript.Append("sys.path.append('");
		ExecScript.Append(AbsPluginDir / "Source/ThirdParty/Scripts");
		//ExecScript.Append(AbsPluginDir / "Binaries/Scripts");
		ExecScript.Append("')");
		PyRun_SimpleString(TCHAR_TO_UTF8(*ExecScript));
		PyRun_SimpleString("import log_catch");
		PyRun_SimpleString("log_catch.redirectLog()");
		SimpleCallFunction("enviroment_path", "initEnviroment", AbsPluginDir);
	}
}

void UPythonVMSystem::FinalizePython() {
#if !WITH_EDITOR
	if (Py_IsInitialized()) {
		Py_Finalize();
	}
#endif
}

bool UPythonVMSystem::IsInitialized() {
	return (bool)Py_IsInitialized();
}

void UPythonVMSystem::RunPythonString(const FString& Str) {
	if (Str.IsEmpty()) return;
	if (Py_IsInitialized()) {
		PyRun_SimpleString(TCHAR_TO_UTF8(*Str));
	}
}

FString UPythonVMSystem::SimpleCallFunction(FString ModuleName, FString FunctionName, FString Param) {
	UPyObject* Module = GetOrImportPyModule(ModuleName);
	if (Module) {
		UPyObject Args;
		Param.IsEmpty() ? Args.Set(Py_BuildValue("")) : Args.Set(Py_BuildValue("(s)", TCHAR_TO_UTF8(*Param)));
		UPyObject FunctionObject(PyObject_GetAttrString(Module->Get(), TCHAR_TO_UTF8(*(FunctionName))));
		if (FunctionObject.IsValid()) {
			UPyObject ResultObject(PyObject_Call(FunctionObject.Get(), Args.Get(), nullptr));
			if (ResultObject.IsValid()) {
				return FString(PyObjectToString(ResultObject.Get()));
			}
		}
	}
	return FString();
}

void UPythonVMSystem::SimpleCallFunctionAsync(FString ModuleName, FString FunctionName, FString Param) {
	UPyObject* Module = GetOrImportPyModule(ModuleName);
	if (Module) {
		PyObject* PyModulePtr = Module->Get();
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [PyModulePtr, FunctionName, Param]() {
			if (PyModulePtr) {
				UPyObject Args;
				Param.IsEmpty() ? Args.Set(Py_BuildValue("")) : Args.Set(Py_BuildValue("(s)", TCHAR_TO_UTF8(*Param)));
				UPyObject FunctionObject = PyObject_GetAttrString(PyModulePtr, TCHAR_TO_UTF8(*(FunctionName)));
				if (FunctionObject.IsValid()) {
					PyObject_Call(FunctionObject.Get(), Args.Get(), nullptr);
				}
			}
			});
	}
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

UPyObject* UPythonVMSystem::GetOrImportPyModule(FString ModuleName)
{
	UPyObject** FindModule = Modules.Find(ModuleName);
	if (FindModule) {
		return *FindModule;
	}
	else {
		if (ImportPythonModule(ModuleName)) {
			FindModule = Modules.Find(ModuleName);
			if (FindModule) {
				return *FindModule;
			}
		}
	}
	return nullptr;
}

bool UPythonVMSystem::ImportPythonModule(FString ModuleName) {
	if (!Modules.Contains(ModuleName)) {
		PyObject* NewPyModule = PyImport_ImportModule(TCHAR_TO_UTF8(*ModuleName));
		if (NewPyModule) {
			UE_LOG(LogPythonVM, Display, TEXT("Import python module: %s"), *ModuleName);
			Modules.Add(ModuleName, new UPyObject(NewPyModule));
			return true;
		}
		UE_LOG(LogPythonVM, Error, TEXT("Import python module failed: %s"), *ModuleName);
		return false;
	}
	return true;
}

