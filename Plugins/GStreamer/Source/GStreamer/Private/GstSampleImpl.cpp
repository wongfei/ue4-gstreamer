#include "GstSampleImpl.h"
#include <memory.h>

extern "C" {
	#include <gst/gst.h>
	#include <gst/video/video-format.h>
}

class FGstSampleImpl : public IGstSample
{
public:

	FGstSampleImpl() { memset(&m_Mapping, 0, sizeof(m_Mapping)); }
	~FGstSampleImpl() { Release(); }
	virtual void Destroy() { delete this; }

	virtual void Init(struct _GstSample* Sample, int Format, int Width, int Height);
	virtual void Release();
	virtual bool Map();
	virtual void Unmap();

	virtual int GetFormat() const { return m_Format; }
	virtual int GetWidth() const { return m_Width; }
	virtual int GetHeight() const { return m_Height; }
	virtual void* GetData() { return m_Mapping.data; }
	virtual size_t GetDataSize() const { return m_Mapping.size; }

private:

	GstSample* m_Sample = nullptr;
	GstBuffer* m_Buffer = nullptr;
	GstMapInfo m_Mapping;

	int m_Format = GST_VIDEO_FORMAT_UNKNOWN;
	int m_Width = 0;
	int m_Height = 0;
};

IGstSample* IGstSample::CreateInstance()
{
	return new FGstSampleImpl();
}

void FGstSampleImpl::Init(struct _GstSample* Sample, int Format, int Width, int Height)
{
	Release();

	m_Sample = Sample;
	m_Buffer = gst_sample_get_buffer(m_Sample);
	m_Width = Width;
	m_Height = Height;
	m_Format = Format;
}

void FGstSampleImpl::Release()
{
	Unmap();

	if (m_Sample)
	{
		gst_sample_unref(m_Sample);
		m_Sample = nullptr;
		m_Buffer = nullptr;
	}
}

bool FGstSampleImpl::Map()
{
	SCOPED_PROFILER;

	if (!m_Mapping.data && m_Buffer)
	{
		gst_buffer_map(m_Buffer, &m_Mapping, GST_MAP_READ);
	}

	return (m_Mapping.data ? true : false);
}

void FGstSampleImpl::Unmap()
{
	if (m_Mapping.data && m_Buffer)
	{
		SCOPED_PROFILER;

		gst_buffer_unmap(m_Buffer, &m_Mapping);
		memset(&m_Mapping, 0, sizeof(m_Mapping));
	}
}
