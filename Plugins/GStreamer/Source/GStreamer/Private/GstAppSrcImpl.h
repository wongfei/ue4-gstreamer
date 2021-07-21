#pragma once

#include "GstCoreImpl.h"

class IGstAppSrcBuffer
{
protected:
	virtual ~IGstAppSrcBuffer() {}

public:
	virtual void Release() = 0;
	virtual void* GetDataPtr() = 0;
	virtual size_t GetDataSize() = 0;
};

class IGstAppSrc
{
	GST_INTERFACE_IMPL(IGstAppSrc)

  public:

	virtual bool Connect(class IGstPipeline* Pipeline, const char* ElementName) = 0;
	virtual void Disconnect() = 0;
	virtual void PushBuffer(IGstAppSrcBuffer* Buffer) = 0;
};
