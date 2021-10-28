#include "GstAppSrcImpl.h"
#include "GstPipelineImpl.h"
#include "GstSampleImpl.h"

extern "C" {
	#include <gst/gst.h>
	#include <gst/app/gstappsrc.h>
}

#include <string>

class FGstAppSrcImpl : public IGstAppSrc
{
public:
	FGstAppSrcImpl() {}
	~FGstAppSrcImpl() { Disconnect(); }
	virtual void Destroy();

	virtual bool Connect(IGstPipeline* Pipeline, const char* ElementName);
	virtual void Disconnect();
	virtual void PushBuffer(IGstAppSrcBuffer* Buffer);

private:
	std::string m_Name;
	GstElement* m_AppSrc = nullptr;
};

IGstAppSrc* IGstAppSrc::CreateInstance()
{
	auto Obj = new FGstAppSrcImpl();
	GST_LOG_DBG_A("GstAppSrc: CreateInstance %p", Obj);
	return Obj;
}

void FGstAppSrcImpl::Destroy()
{
	GST_LOG_DBG_A("GstAppSrc: Destroy %p", this);
	delete this;
}

bool FGstAppSrcImpl::Connect(IGstPipeline* Pipeline, const char* ElementName)
{
	GST_LOG_DBG_A("GstAppSrc: Connect <%s>", ElementName);

	if (m_AppSrc)
	{
		GST_LOG_ERR_A("GstAppSrc: Already connected");
		return false;
	}

	for (;;)
	{
		m_Name = ElementName;

		m_AppSrc = gst_bin_get_by_name(GST_BIN(Pipeline->GetGPipeline()), ElementName);
		if (!m_AppSrc)
		{
			GST_LOG_ERR_A("gst_bin_get_by_name failed");
			break;
		}

		g_object_set(m_AppSrc, "emit-signals", TRUE, nullptr);

		GST_LOG_DBG_A("GstAppSrc: Connect SUCCESS");
		return true;
	}

	GST_LOG_ERR_A("GstAppSrc: Connect FAILED");
	Disconnect();
	return false;
}

void FGstAppSrcImpl::Disconnect()
{
	if (m_AppSrc)
	{
		GST_LOG_DBG_A("GstAppSrc: Disconnect <%s>", m_Name.c_str());
		gst_app_src_end_of_stream(GST_APP_SRC(m_AppSrc));
		g_object_set(m_AppSrc, "emit-signals", FALSE, nullptr);
		gst_object_unref(m_AppSrc);
		m_AppSrc = nullptr;
	}
}

static void DestroyNotifyHandler(gpointer Data)
{
	((IGstAppSrcBuffer*)Data)->Release();
}

void FGstAppSrcImpl::PushBuffer(IGstAppSrcBuffer* Buffer)
{
	void* Data = Buffer->GetDataPtr();
	size_t DataSize = Buffer->GetDataSize();
	GstBuffer* BufferObj = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY, Data, DataSize, 0, DataSize, Buffer, DestroyNotifyHandler);

	const GstFlowReturn Result = gst_app_src_push_buffer(GST_APP_SRC(m_AppSrc), BufferObj);
	if (Result != GST_FLOW_OK)
	{
		GST_LOG_ERR_A("gst_app_src_push_buffer failed: GstFlowReturn=%d (%s)", (int)Result, gst_flow_get_name(Result));
	}
}
