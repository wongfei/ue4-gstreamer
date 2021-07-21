#include "GStreamerLoaderModule.h"
#include "Runtime/Core/Public/Misc/Paths.h"

#if PLATFORM_WINDOWS
	#include "Windows/AllowWindowsPlatformTypes.h"
	#include <windows.h>
	#include "Windows/HideWindowsPlatformTypes.h"
#endif

class FGStreamerLoaderModule : public IGStreamerLoaderModule
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

static FString GetGstRoot()
{
	auto RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_1_0_ROOT_MSVC_X86_64"));
	if (RootPath.IsEmpty())
		RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT_X86_64"));
	if (RootPath.IsEmpty())
		RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT"));
	return RootPath;
}

void FGStreamerLoaderModule::StartupModule()
{
	UE_LOG(LogInit, Display, TEXT("GStreamerLoader::StartupModule"));

#if PLATFORM_WINDOWS
	FString RootPath = GetGstRoot();
	if (!RootPath.IsEmpty())
	{
		UE_LOG(LogInit, Display, TEXT("GSTREAMER_ROOT: \"%s\""), *RootPath);
		FString BinPath = FPaths::Combine(RootPath, TEXT("bin"));
		if (FPaths::DirectoryExists(BinPath))
		{
			SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
			//SetDllDirectoryW(*BinPath);
			AddDllDirectory(*BinPath);
		}
		else
		{
			UE_LOG(LogInit, Error, TEXT("Directory not found: \"%s\""), *BinPath);
		}
	}
	else
	{
		UE_LOG(LogInit, Error, TEXT("GSTREAMER_ROOT not found"));
	}
#endif
}

void FGStreamerLoaderModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FGStreamerLoaderModule, GStreamerLoader)
