#pragma once

#include "ModuleManager.h"

class GSTREAMER_API IGStreamerModule : public IModuleInterface
{
public:

	static inline IGStreamerModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IGStreamerModule>("GStreamer");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GStreamer");
	}
};
