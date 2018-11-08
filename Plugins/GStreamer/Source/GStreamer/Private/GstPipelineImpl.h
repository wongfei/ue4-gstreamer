#pragma once

#include "GstCoreImpl.h"

class IGstPipeline
{
	GST_INTERFACE_IMPL(IGstPipeline)

public:

	virtual bool Init(const char* Name, const char* Config) = 0;
	virtual void Shutdown() = 0;
	virtual bool Start() = 0;
	virtual void Stop() = 0;

	virtual const char* GetName() const = 0;
	virtual struct _GstElement* GetGPipeline() = 0;
	virtual struct _GstBus* GetGBus() = 0;
};
