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
	const int32 BufSize = 2048;
	TCHAR RootPath[BufSize] = {0};

	FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT_X86_64"), RootPath, BufSize);
	if (!RootPath[0])
	{
		FPlatformMisc::GetEnvironmentVariable(TEXT("GSTREAMER_ROOT"), RootPath, BufSize);
	}
	if (RootPath[0])
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
