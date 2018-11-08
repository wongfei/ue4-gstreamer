#pragma once

#include "GstCoreImpl.h"

class IGstSample
{
	GST_INTERFACE_IMPL(IGstSample)

public:

	virtual void Init(struct _GstSample* Sample, int Format, int Width, int Height) = 0;
	virtual void Release() = 0;
	virtual bool Map() = 0;
	virtual void Unmap() = 0;

	virtual int GetFormat() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual void* GetData() = 0;
	virtual size_t GetDataSize() const = 0;
};
