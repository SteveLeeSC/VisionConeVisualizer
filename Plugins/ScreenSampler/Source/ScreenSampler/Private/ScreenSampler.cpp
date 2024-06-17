// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScreenSampler.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include <ShaderCore.h>

#define LOCTEXT_NAMESPACE "FScreenSamplerModule"

void FScreenSamplerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString ShaderPath = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("ScreenSampler"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/GlobalShaderModule"), ShaderPath);
}

void FScreenSamplerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FScreenSamplerModule, ScreenSampler)