#include "GStreamerLoaderModule.h"
#include "Runtime/Core/Public/Misc/Paths.h"

#if PLATFORM_WINDOWS
	#include "AllowWindowsPlatformTypes.h"
	#include <windows.h>
	#include "HideWindowsPlatformTypes.h"
#endif

class FGStreamerLoaderModule : public IGStreamerLoaderModule
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FGStreamerLoaderModule::StartupModule()
{
	#if PLATFORM_WINDOWS

	auto RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_1_0_ROOT_MSVC_X86_64"));
	if (RootPath.IsEmpty())
		RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT_X86_64"));
	if (RootPath.IsEmpty())
		RootPath = FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT"));

	if (!RootPath.IsEmpty())
	{
		FString BinPath = FPaths::Combine(RootPath, TEXT("bin"));
		SetDllDirectoryW(*BinPath);
	}
	#endif
}

void FGStreamerLoaderModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FGStreamerLoaderModule, GStreamerLoader)
