#include "GstAppSrcComponent.h"
#include "GstPipelineImpl.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

class FGstAppSrcBuffer : public IGstAppSrcBuffer
{
public:
	FGstAppSrcBuffer(UGstAppSrcComponent* InOwner) : Owner(InOwner) { ColorBuffer.Reserve(1920 * 1080); }

	virtual void Release() { Owner->ReleaseBuffer(this); }
	virtual void* GetDataPtr() { return ColorBuffer.GetData(); }
	virtual size_t GetDataSize() { return ColorBuffer.Num() * sizeof(FColor); }

public:
	UGstAppSrcComponent* Owner;
	TArray<FColor> ColorBuffer;
	FRenderCommandFence Fence;
	size_t FrameId = 0;
};

UGstAppSrcComponent::UGstAppSrcComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.0f / 60.0f;
}

void UGstAppSrcComponent::UninitializeComponent()
{
	ResetState();
}

void UGstAppSrcComponent::ResetState()
{
	if (AppSrc)
		AppSrc->Disconnect();
	SafeDestroy(AppSrc);
	DestroyBuffers();
	FrameId = 0;
}

void UGstAppSrcComponent::CbPipelineStart(IGstPipeline* Pipeline)
{
	ResetState();

	if (AppSrcEnabled && !AppSrcName.IsEmpty())
	{
		AppSrc = IGstAppSrc::CreateInstance();
		AppSrc->Connect(Pipeline, TCHAR_TO_ANSI(*AppSrcName));
	}
}

void UGstAppSrcComponent::CbPipelineStop()
{
	ResetState();
}

void UGstAppSrcComponent::SetCaptureInterval(float Interval)
{
	PrimaryComponentTick.TickInterval = Interval;
}

void UGstAppSrcComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AppSrc)
	{
		AActor* Actor = GetOwner();
		for (FComponentReference& ComponentReference : AppSrcCaptures)
		{
			USceneCaptureComponent2D* CaptureComponent = Cast<USceneCaptureComponent2D>(ComponentReference.GetComponent(Actor));
			if (CaptureComponent)
			{
				UTextureRenderTarget2D* TextureTarget = CaptureComponent->TextureTarget;
				if (TextureTarget)
				{
					FTextureRenderTargetResource* TextureResource = TextureTarget->GameThread_GetRenderTargetResource();
					PushBufferAsync(TextureResource);
				}
			}
		}

		if (BufferQueue.Num() > 0)
		{
			auto* Buffer = BufferQueue[0];
			if (Buffer->Fence.IsFenceComplete())
			{
				BufferQueue.RemoveAt(0);
				AppSrc->PushBuffer(Buffer);
			}
		}
	}

	FrameId++;
}

void UGstAppSrcComponent::PushBufferAsync(FTextureRenderTargetResource* TextureResource)
{
	if (BufferQueue.Num() > MaxQueueLength)
	{
		GST_LOG_ERR(TEXT("PushBufferAsync overflow"));
		return;
	}

	FIntPoint Size = TextureResource->GetSizeXY();
	FIntRect InRect = FIntRect(0, 0, Size.X, Size.Y);
	FReadSurfaceDataFlags InFlags = FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX);

	FGstAppSrcBuffer* Buffer = GetBuffer();
	Buffer->ColorBuffer.SetNum(Size.X * Size.Y);
	Buffer->FrameId = FrameId;
	BufferQueue.Add(Buffer);

	struct FReadSurfaceContext
	{
		UGstAppSrcComponent* AppSrc;
		FGstAppSrcBuffer* Buffer;
		FRenderTarget* SrcRenderTarget;
		FIntRect Rect;
		FReadSurfaceDataFlags Flags;
	};

	FReadSurfaceContext Context =
	{
		this,
		Buffer,
		TextureResource,
		InRect,
		InFlags
	};

	ENQUEUE_RENDER_COMMAND(ReadSurfaceCommand)(
		[Context](FRHICommandListImmediate& RHICmdList)
	{
		RHICmdList.ReadSurfaceData(
			Context.SrcRenderTarget->GetRenderTargetTexture(),
			Context.Rect,
			Context.Buffer->ColorBuffer,
			Context.Flags
		);
	});

	Buffer->Fence.BeginFence();
}

FGstAppSrcBuffer* UGstAppSrcComponent::GetBuffer()
{
	if (BufferPool.Num() > 0)
	{
		FScopeLock Lock(&PoolMx);
		if (BufferPool.Num() > 0)
			return BufferPool.Pop();
	}
	return new FGstAppSrcBuffer(this);
}

void UGstAppSrcComponent::ReleaseBuffer(FGstAppSrcBuffer* Buffer)
{
	FScopeLock Lock(&PoolMx);
	BufferPool.Add(Buffer);
}

template<typename T> inline void DestroyBufferList(T& Container)
{
	for (auto* Buf : Container)
		delete Buf;
	Container.SetNum(0);
}

void UGstAppSrcComponent::DestroyBuffers()
{
	if (BufferQueue.Num() > 0)
	{
		FlushRenderingCommands();
		DestroyBufferList(BufferQueue);
	}
	DestroyBufferList(BufferPool);
}
