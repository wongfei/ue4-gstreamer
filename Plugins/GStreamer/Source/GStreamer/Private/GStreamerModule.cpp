#include "GStreamerModule.h"
#include "GstCoreImpl.h"
#include "SharedUnreal.h"
#include "Runtime/Core/Public/Misc/Paths.h"

#if PLATFORM_WINDOWS
	#include "Windows/AllowWindowsPlatformTypes.h"
	#include <windows.h>
	#include "Windows/HideWindowsPlatformTypes.h"
#endif

DEFINE_LOG_CATEGORY(LogGStreamer);

class FGStreamerModule : public IGStreamerModule
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

void FGStreamerModule::StartupModule()
{
	UE_LOG(LogGStreamer, Display, TEXT("GStreamerModule::StartupModule"));

	INIT_PROFILER;

	FString BinPath, PluginPath;

#if PLATFORM_WINDOWS
	FString RootPath = GetGstRoot();
	if (!RootPath.IsEmpty())
	{
		UE_LOG(LogGStreamer, Display, TEXT("GSTREAMER_ROOT: \"%s\""), *RootPath);
		BinPath = FPaths::Combine(RootPath, TEXT("bin"));
		if (FPaths::DirectoryExists(BinPath))
		{
			SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
			//SetDllDirectoryW(*BinPath);
			AddDllDirectory(*BinPath);
		}
		else
		{
			UE_LOG(LogGStreamer, Error, TEXT("Directory not found: \"%s\""), *BinPath);
			BinPath = "";
		}
		PluginPath = FPaths::Combine(RootPath, TEXT("lib"), TEXT("gstreamer-1.0"));
		if (!FPaths::DirectoryExists(PluginPath))
		{
			UE_LOG(LogGStreamer, Error, TEXT("Directory not found: \"%s\""), *PluginPath);
			PluginPath = "";
		}
	}
	else
	{
		UE_LOG(LogGStreamer, Error, TEXT("GSTREAMER_ROOT not found"));
	}
#endif

	if (FGstCoreImpl::Init(TCHAR_TO_ANSI(*BinPath), TCHAR_TO_ANSI(*PluginPath)))
	{
		UE_LOG(LogGStreamer, Display, TEXT("GstCore: Init SUCCESS"));
	}
	else
	{
		UE_LOG(LogGStreamer, Error, TEXT("GstCore: Init FAILED"));
	}
}

void FGStreamerModule::ShutdownModule()
{
	GST_LOG_DBG(TEXT("ShutdownModule"));

	FGstCoreImpl::Deinit();

	SHUT_PROFILER;
}

IMPLEMENT_MODULE(FGStreamerModule, GStreamer)
