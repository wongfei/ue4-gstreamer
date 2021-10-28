#include "GstPipelineImpl.h"
#include <string>
#include <thread>

extern "C" {
	#include <gst/gst.h>
}

class FGstPipelineImpl : public IGstPipeline
{
public:

	FGstPipelineImpl() {}
	~FGstPipelineImpl() { Shutdown(); }
	virtual void Destroy();

	virtual bool Init(const char* Name, const char* Config);
	virtual void Shutdown();
	virtual bool Start();
	virtual void Stop();

	virtual const char* GetName() const { return m_Name.c_str(); }
	virtual struct _GstElement* GetGPipeline() { return m_Pipeline; }
	virtual struct _GstBus* GetGBus() { return m_Bus; }

	void WorkerLoop();
	gboolean OnBusMessage(GstMessage* Message);

private:

	std::string m_Name;
	GstElement* m_Pipeline = nullptr;
	GstBus* m_Bus = nullptr;
	GMainLoop* m_Loop = nullptr;
	std::unique_ptr<std::thread> m_Worker;
};

IGstPipeline* IGstPipeline::CreateInstance()
{
	auto Obj = new FGstPipelineImpl();
	GST_LOG_DBG_A("GstPipeline: CreateInstance %p", Obj);
	return Obj;
}

void FGstPipelineImpl::Destroy()
{
	GST_LOG_DBG_A("GstPipeline: Destroy %p", this);
	delete this;
}

#define GST_RELEASE(func, ptr) do { if (ptr) { func(ptr); ptr = nullptr; } } while (0)
static gboolean BusMessageFunc(GstBus*, GstMessage* Message, FGstPipelineImpl* Context) { return Context->OnBusMessage(Message); }
static void ThreadWorkerFunc(FGstPipelineImpl* Context) { Context->WorkerLoop(); }

bool FGstPipelineImpl::Init(const char* Name, const char* Config)
{
	GST_LOG_INFO_A("GstPipeline: Init <%s>", Name);

	if (m_Pipeline)
	{
		GST_LOG_ERR_A("GstPipeline: Already initialized");
		return false;
	}

	for (;;)
	{
		m_Name = Name;

		GST_LOG_INFO_A("GstPipeline: gst_parse_launch \"%s\"", Config);
		GError* Error = nullptr;
		m_Pipeline = gst_parse_launch(Config, &Error);
		if (Error)
		{
			GST_LOG_ERR_A("gst_parse_launch failed -> %s", Error->message);
			g_error_free(Error);
			break;
		}

		m_Bus = gst_pipeline_get_bus(GST_PIPELINE(m_Pipeline));
		if (!m_Bus)
		{
			GST_LOG_ERR_A("gst_pipeline_get_bus failed");
			break;
		}

		gst_bus_add_watch(m_Bus, (GstBusFunc)BusMessageFunc, this);

		GST_LOG_DBG_A("GstPipeline: Init SUCCESS");
		return true;
	}

	GST_LOG_ERR_A("GstPipeline: Init FAILED");
	Shutdown();
	return false;
}

void FGstPipelineImpl::Shutdown()
{
	Stop();

	if (m_Pipeline)
	{
		GST_LOG_INFO_A("GstPipeline: Shutdown <%s>", m_Name.c_str());
		gst_element_set_state(m_Pipeline, GST_STATE_NULL);
	}

	GST_RELEASE(gst_object_unref, m_Bus);
	GST_RELEASE(gst_object_unref, m_Pipeline);
}

bool FGstPipelineImpl::Start()
{
	GST_LOG_INFO_A("GstPipeline: Start <%s>", m_Name.c_str());

	if (m_Loop)
	{
		GST_LOG_ERR_A("GstPipeline: Already started");
		return false;
	}

	for (;;)
	{
		m_Loop = g_main_loop_new(nullptr, FALSE);
		if (!m_Loop)
		{
			GST_LOG_ERR_A("g_main_loop_new failed");
			break;
		}

		m_Worker.reset(new std::thread(ThreadWorkerFunc, this));

		GST_LOG_DBG_A("GstPipeline: Start SUCCESS");
		return true;
	}

	GST_LOG_ERR_A("GstPipeline: Start FAILED");
	Stop();
	return false;
}

void FGstPipelineImpl::Stop()
{
	if (m_Loop)
	{
		GST_LOG_INFO_A("GstPipeline: Stop <%s>", m_Name.c_str());

		g_main_loop_quit(m_Loop);

		if (m_Worker && m_Worker->joinable())
		{
			m_Worker->join();
		}
		m_Worker.reset(nullptr);

		GST_RELEASE(g_main_loop_unref, m_Loop);
	}
}

void FGstPipelineImpl::WorkerLoop()
{
	GST_LOG_DBG_A("GstPipelineWorker: Start <%s>", m_Name.c_str());

	gst_element_set_state(m_Pipeline, GST_STATE_PLAYING);
	g_main_loop_run(m_Loop);
	gst_element_set_state(m_Pipeline, GST_STATE_NULL); 

	GST_LOG_DBG_A("GstPipelineWorker: Stop <%s>", m_Name.c_str());
}

gboolean FGstPipelineImpl::OnBusMessage(GstMessage* Message)
{
	const int Type = GST_MESSAGE_TYPE(Message);
	switch (Type)
	{
		// ignore
		case GST_MESSAGE_TAG:
		case GST_MESSAGE_BUFFERING:
			break;

		case GST_MESSAGE_EOS:
			gst_element_seek(m_Pipeline, 
				1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
				GST_SEEK_TYPE_SET,  0, // (in nanoseconds)
				GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
			break;

		case GST_MESSAGE_ERROR:
			GST_LOG_ERR_A("GstPipeline: BUS ERROR <%s>", m_Name.c_str());
			g_main_loop_quit(m_Loop);
			break;

		case GST_MESSAGE_WARNING:
			GST_LOG_ERR_A("GstPipeline: BUS WARNING <%s>", m_Name.c_str());
			break;

		case GST_MESSAGE_STATE_CHANGED:
		{
			GstState old_state, new_state;
			gst_message_parse_state_changed (Message, &old_state, &new_state, NULL);
			GST_LOG_DBG_A("GstPipeline: STATE CHANGED <%s> %s -> %s", m_Name.c_str(), gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
			break;
		}

		default:
		{
			GST_LOG_DBG_A("GstPipeline: OnBusMessage <%s> %s (%d)", m_Name.c_str(), gst_message_type_get_name((GstMessageType)Type), Type);
			break;
		}
	}

	return TRUE;
}
