#include "GstTexture.h"
#include "RenderUtils.h"
#include "TextureResource.h"
#include "Async/Async.h"

#include "GstAppSinkImpl.h"
#include "GstSampleImpl.h"

FGstTexture::FGstTexture(FString Name, IGstAppSink* AppSink, IGstTextureCallback* Callback)
{
	m_Name = Name;
	m_AppSink = AppSink;
	m_Callback = Callback;
}

FGstTexture::~FGstTexture()
{
	Release();
}

void FGstTexture::Destroy()
{
	delete this;
}

void FGstTexture::SubmitSample(IGstSample* Sample)
{
	SCOPED_PROFILER;

	IGstSample* OldSample = SwapSample(Sample);
	ReleaseSample(OldSample);
	m_SampleCount++;
}

void FGstTexture::ReleaseSample(IGstSample* Sample)
{
	SCOPED_PROFILER;

	if (Sample)
	{
		if (m_AppSink) m_AppSink->ReleaseSample(Sample);
		else Sample->Destroy();
	}
}

IGstSample* FGstTexture::SwapSample(IGstSample* NewSample)
{
	SCOPED_PROFILER;

	FScopeLock Lock(&m_SampleMx);
	IGstSample* OldSample = m_Sample;
	m_Sample = NewSample;
	return OldSample;
}

void FGstTexture::TickGameThread()
{
	SCOPED_PROFILER;

	IGstSample* Sample = SwapSample(nullptr);
	if (Sample)
	{
		Resize(Sample);

		if (m_TextureObject)
		{
			// render commands should be submitted only from game thread
			auto Context = this;
			ENQUEUE_RENDER_COMMAND(SceneDrawCompletion)([Context, Sample](FRHICommandListImmediate& RHICmdList)
			{
				Context->RenderCmd_UpdateTexture(Sample);
			});

			m_EnqueCount++;
		}
		else
		{
			ReleaseSample(Sample);
		}
	}

	#if defined(GST_ENABLE_DEBUG_LOG)
	double CurTime = FPlatformTime::Seconds();
	if (m_StatTimer <= CurTime)
	{
		GST_LOG_DBG(TEXT("GstTexture: <%s> SampleCount=%d TickCount=%d EnqueCount=%d RenderCount=%d"), *m_Name, m_SampleCount, m_TickCount, m_EnqueCount, m_RenderCount);
		m_StatTimer = CurTime + 1.0;
		m_SampleCount = 0;
		m_TickCount = 0;
		m_EnqueCount = 0;
		m_RenderCount = 0;
	}
	#endif

	m_TickCount++;
}

void FGstTexture::RenderCmd_UpdateTexture(IGstSample* Sample)
{
	SCOPED_PROFILER;

	auto Tex = GetTextureObject();
	if (Tex && Tex->Resource)
	{
		RHIUpdateTexture2D(
			Tex->Resource->GetTexture2DRHI(),
			0,
			FUpdateTextureRegion2D(0, 0, 0, 0, Sample->GetWidth(), Sample->GetHeight()),
			m_Pitch,
			(const uint8*)Sample->GetData()
		);

		m_RenderCount++;
	}

	ReleaseSample(Sample);
}

void FGstTexture::Resize(IGstSample* Sample)
{
	const EGstVideoFormat SampleFormat = (EGstVideoFormat)Sample->GetFormat();

	if (m_GstFormat != SampleFormat
		|| m_Width != Sample->GetWidth()
		|| m_Height != Sample->GetHeight())
	{
		Release();

		GST_LOG_DBG(TEXT("GstTexture: Resize <%s> Width=%d Height=%d GstFormat=%d"), 
			*m_Name, Sample->GetWidth(), Sample->GetHeight(), Sample->GetFormat());

		switch (SampleFormat)
		{
			// SUPPORTED
			case EGstVideoFormat::GST_VIDEO_FORMAT_RGBA:
				m_UeFormat = EPixelFormat::PF_R8G8B8A8;
				m_Compression = TextureCompressionSettings::TC_VectorDisplacementmap;
				break;

			// NOT SUPPORTED
			default:
				return;
		}

		m_GstFormat = SampleFormat;
		m_Width = Sample->GetWidth();
		m_Height = Sample->GetHeight();
		m_Pitch = m_Width * GPixelFormats[m_UeFormat].BlockBytes;

		auto Context = this;
		#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 26)
		ENQUEUE_RENDER_COMMAND(SceneDrawCompletion)([Context](FRHICommandListImmediate& RHICmdList)
		{
			Context->RenderCmd_CreateTexture();
		});
		#else
		AsyncTask(ENamedThreads::GameThread, [Context]() {
			Context->RenderCmd_CreateTexture();
		});
		#endif
	}
}

void FGstTexture::RenderCmd_CreateTexture()
{
	GST_LOG_DBG(TEXT("DynamicTexture: CreateTransient <%s>"), *m_Name);

	auto Tex = UTexture2D::CreateTransient(m_Width, m_Height, m_UeFormat);
	if (!Tex)
	{
		GST_LOG_ERR(TEXT("CreateTransient failed"));
	}
	else
	{
		#if WITH_EDITORONLY_DATA
		Tex->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		#endif

		Tex->CompressionSettings = m_Compression;
		Tex->Filter = TextureFilter::TF_Nearest;
		Tex->SRGB = 0;

		Tex->AddToRoot();
		Tex->UpdateResource();

		m_TextureObject = Tex;

		if (m_Callback)
		{
			m_Callback->CbGstTextureCreated();
		}
	}
}

void FGstTexture::Release()
{
	if (m_TextureObject)
	{
		GST_LOG_DBG(TEXT("DynamicTexture: Release <%s>"), *m_Name);
		FlushRenderingCommands();
		m_TextureObject->RemoveFromRoot();
		m_TextureObject = nullptr;
	}

	IGstSample* Sample = SwapSample(nullptr);
	ReleaseSample(Sample);

	m_GstFormat = EGstVideoFormat::GST_VIDEO_FORMAT_UNKNOWN;
	m_UeFormat = EPixelFormat::PF_Unknown;
	m_Width = 0;
	m_Height = 0;
}
