// Copyright Epic Games, Inc. All Rights Reserved.

#include "MetaSoundExt.h"
#include "MetasoundFrontendRegistries.h"
#include "Modules/ModuleManager.h"
#include "MetasoundDataTypeRegistrationMacro.h"

#define LOCTEXT_NAMESPACE "FMetaSoundExtModule"

void FMetaSoundExtModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FMetasoundFrontendRegistryContainer::Get()->RegisterPendingNodes();
}

void FMetaSoundExtModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMetaSoundExtModule, MetaSoundExt)