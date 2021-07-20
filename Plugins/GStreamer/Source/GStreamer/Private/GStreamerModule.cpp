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
