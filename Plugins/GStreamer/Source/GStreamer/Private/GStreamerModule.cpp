#include "GStreamerModule.h"
#include "GstCoreImpl.h"
#include "SharedUnreal.h"
#include "Runtime/Core/Public/Misc/Paths.h"

class FGStreamerModule : public IGStreamerModule
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

DEFINE_LOG_CATEGORY(LogGStreamer);

static FString GetGstRoot()
{
	const int32 BufSize = 2048;
	TCHAR RootPath[BufSize] = {0};

	FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT_X86_64"), RootPath, BufSize);
	if (!RootPath[0])
	{
		FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT"), RootPath, BufSize);
	}

	return FString(RootPath);
}

void FGStreamerModule::StartupModule()
{
	GST_LOG_DBG(TEXT("StartupModule"));

	INIT_PROFILER;

	FString RootPath = GetGstRoot();
	FString BinPath = FPaths::Combine(RootPath, TEXT("bin"));
	FString PluginPath = FPaths::Combine(RootPath, TEXT("lib"), TEXT("gstreamer-1.0"));
	GST_LOG_DBG(TEXT("GStreamer: GSTREAMER_ROOT=\"%s\""), *RootPath);

	if (FGstCoreImpl::Init(TCHAR_TO_ANSI(*BinPath), TCHAR_TO_ANSI(*PluginPath)))
	{
		GST_LOG_DBG(TEXT("GStreamer: Init SUCCESS"));
	}
	else
	{
		GST_LOG_ERR(TEXT("GStreamer: Init FAILED"));
	}
}

void FGStreamerModule::ShutdownModule()
{
	GST_LOG_DBG(TEXT("ShutdownModule"));

	FGstCoreImpl::Deinit();

	SHUT_PROFILER;
}

IMPLEMENT_MODULE(FGStreamerModule, GStreamer)
