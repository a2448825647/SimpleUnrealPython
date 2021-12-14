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
class UPyObject {
public:
	UPyObject() :Object(nullptr) {};
	UPyObject(PyObject* InObj):Object(InObj) {}
	~UPyObject() { SAFE_DELETE_PYOBJECT(Object); }
	PyObject* Get() { return Object; }
	void Set(PyObject* InObj) {
		SAFE_DELETE_PYOBJECT(Object);
		Object = InObj;
	}
	bool IsValid() { return Object != nullptr; };
private:
	UPyObject(const UPyObject&) = delete;
	UPyObject& operator= (UPyObject&) = delete;
private:
	PyObject* Object;
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
		void InitializePython();

	UFUNCTION(BlueprintCallable, Category = "PythonVM")
		void FinalizePython();

	UFUNCTION(BlueprintCallable, Category = "PythonVM")
		bool IsInitialized();

	UFUNCTION(BlueprintCallable, Category = "PythonVM")
		void RunPythonString(const FString& Str);

	UFUNCTION(BlueprintCallable, Category = "PythonVM")
		FString SimpleCallFunction(FString ModuleName, FString FunctionName, FString Param = "");

	UFUNCTION(BlueprintCallable, Category = "PythonVM")
		void SimpleCallFunctionAsync(FString ModuleName, FString FunctionName, FString Param = "");

	FString PyObjectToString(PyObject* Object);
private:
	bool ImportPythonModule(FString ModuleName);

	UPyObject* GetOrImportPyModule(FString ModuleName);

private:
	TMap<FString, UPyObject*> Modules; // Key: ModuleName  Value:ModuleStruct
};


