// Fill out your copyright notice in the Description page of Project Settings.


#include "PythonVMSystem.h"
#include <Python.h>
#include <string>

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
		PyRun_SimpleString("import sys");
		FString EvenPath;
		EvenPath = "sys.path.append('" + FPaths::ProjectPluginsDir() / "PythonVM/Source/ThirdParty/PythonPackages" + "')";
		PyRun_SimpleString(TCHAR_TO_UTF8(*EvenPath));
		EvenPath = "sys.path.append('" + FPaths::ProjectPluginsDir() / "PythonVM/Source/Scripts" + "')";
		PyRun_SimpleString(TCHAR_TO_UTF8(*EvenPath));

		mPyArgs = Py_BuildValue("");
		mPyModule_log_catch = PyImport_ImportModule("log_catch");
		if (mPyModule_log_catch) {
			PyObject* Func = PyObject_GetAttrString(mPyModule_log_catch, "redirectLog");
			if (Func) {
				PyObject_Call(Func, mPyArgs, nullptr);
				Py_DECREF(Func);
			}
			mPyFunction_getLog = PyObject_GetAttrString(mPyModule_log_catch, "getLog");
			mPyFunction_clearLog = PyObject_GetAttrString(mPyModule_log_catch, "clearLog");
		}
		PrintPythonLog();
	}
}

void UPythonVMSystem::FinalizePython() {
	if (Py_IsInitialized()) {
		if (mPyModule_log_catch != nullptr) Py_DECREF(mPyModule_log_catch);
		if (mPyModule_log_catch != nullptr) Py_DECREF(mPyFunction_getLog);
		if (mPyModule_log_catch != nullptr) Py_DECREF(mPyFunction_clearLog);
		if (mPyModule_log_catch != nullptr) Py_DECREF(mPyArgs);
		mPyModule_log_catch = nullptr;
		mPyFunction_getLog = nullptr;
		mPyFunction_clearLog = nullptr;
		mPyArgs = nullptr;
		Py_Finalize();
	}
}

void UPythonVMSystem::RunPythonString(const FString& Str) {
	PyRun_SimpleString(std::string(TCHAR_TO_UTF8(*Str)).c_str());
	PrintPythonLog();
}

void UPythonVMSystem::RunPythonFile(const FString& FilePath) {
	const char* fileName = std::string(TCHAR_TO_UTF8(*FilePath)).c_str();
	FILE* f = fopen(fileName, "r");
	if (f) {
		PyRun_SimpleFile(f, fileName);
	}
	fclose(f);
	f = nullptr;
	PrintPythonLog();
}

void UPythonVMSystem::PrintPythonLog() {
	if (mPyFunction_getLog && mPyFunction_clearLog) {
		PyObject* Result = PyObject_Call(mPyFunction_getLog, mPyArgs, nullptr);
		if (Result) {
			Py_ssize_t Size = 0;
			const char* ret = PyUnicode_AsUTF8AndSize(Result, &Size);
			if (Size > 0 && ret != nullptr) {
				FString LogStr(ret);
				UE_LOG(LogTemp, Display, TEXT("PythonLevel:\n%s"), *LogStr);
			}
			Py_DECREF(Result);
			PyObject_Call(mPyFunction_clearLog, mPyArgs, nullptr);
		}
	}
}
