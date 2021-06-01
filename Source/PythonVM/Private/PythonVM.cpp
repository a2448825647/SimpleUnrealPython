// Copyright Epic Games, Inc. All Rights Reserved.

#include "PythonVM.h"
#include "Core.h"
//#include "Modules/ModuleManager.h"
//#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FPythonVMModule"
DEFINE_LOG_CATEGORY(LogPythonVM);

void FPythonVMModule::StartupModule()
{
	
}

void FPythonVMModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPythonVMModule, PythonVM)
