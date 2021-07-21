#include "GStreamerModule.h"
#include "GstCoreImpl.h"
#include "SharedUnreal.h"
#include "Runtime/Core/Public/Misc/Paths.h"

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
	GST_LOG_DBG(TEXT("StartupModule"));

	INIT_PROFILER;

	FString RootPath = GetGstRoot();
	if (!RootPath.IsEmpty())
	{
		UE_LOG(LogGStreamer, Display, TEXT("GSTREAMER_ROOT: \"%s\""), *RootPath);
	}
	else
	{
		UE_LOG(LogGStreamer, Error, TEXT("GSTREAMER_ROOT not found"));
	}

	FString BinPath = FPaths::Combine(RootPath, TEXT("bin"));
	FString PluginPath = FPaths::Combine(RootPath, TEXT("lib"), TEXT("gstreamer-1.0"));
	if (!FPaths::DirectoryExists(BinPath))
	{
		UE_LOG(LogGStreamer, Error, TEXT("Directory not found: \"%s\""), *BinPath);
	}

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
