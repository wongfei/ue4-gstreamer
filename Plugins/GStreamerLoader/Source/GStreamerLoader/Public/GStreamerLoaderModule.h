#pragma once

#include "ModuleManager.h"

class GSTREAMERLOADER_API IGStreamerLoaderModule : public IModuleInterface
{
public:

	static inline IGStreamerLoaderModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IGStreamerLoaderModule>("GStreamerLoader");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GStreamerLoader");
	}
};
