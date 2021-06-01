// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Python.h>
#include "Subsystems/GameInstanceSubsystem.h"
#include "PythonVMSystem.generated.h"

#define SAFE_DELETE_PYOBJECT(Obj) \
if ((Obj) != nullptr) {\
Py_DECREF((Obj));\
(Obj) = nullptr; }

/**
 * 
 */

struct FPyModule {
	PyObject* Module;
	TMap<FString, PyObject*> Functions; // Key:FunctionName  Value:FunctionInstance
};

UCLASS()
class PYTHONVM_API UPythonVMSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize();

	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	void RecreatePythonEnvironment();
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	void RunPythonString(const FString& Str);
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	void RunPythonFile(const FString& ModuleName);
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	bool ImportPythonModule(FString ModuleName);
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	void CallEmptyFunction(FString FunctionName);
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	FString CallStringFunctionWithStringParam(FString FunctionName, FString Param);
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
	FString CallStringFunction(FString FunctionName);

	PyObject* GetOrLoadModule(FString ModuleName, bool PrintLog = true);
	PyObject* GetOrLoadFunction(FString FunctionName, bool PrintLog = true);
	PyObject* CallFunction(FString FunctionName, PyObject* Args = nullptr, PyObject* Kwargs = nullptr);
	const PyObject* GetEmptyArgs();
	FString PyObjectToString(PyObject* Object);
private:
	bool ImportPythonModuleImpl(FString ModuleName);
	void InitializePython();
	void FinalizePython();
	void PrintPythonLog();
	inline bool SplitFunctionName(FString In, FString& OutModuleName, FString& OutFunctionName);

	TMap<FString, FPyModule> Modules; // Key: ModuleName  Value:ModuleStruct
};

