// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PythonVMSystem.generated.h"

/**
 * 
 */
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
		void RunPythonString(const FString& Str);
	UFUNCTION(BlueprintCallable, Category = "PythonVM")
		void RunPythonFile(const FString& FilePath);
private:
	void InitializePython();
	void FinalizePython();
	void PrintPythonLog();


};
