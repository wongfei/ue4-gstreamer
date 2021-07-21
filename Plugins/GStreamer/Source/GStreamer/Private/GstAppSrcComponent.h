#pragma once

#include "GstElementComponent.h"
#include "GstAppSrcImpl.h"
#include "GstVideoFormat.h"
#include "GstAppSrcComponent.generated.h"

class FGstAppSrcBuffer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSTREAMER_API UGstAppSrcComponent : public UGstElementComponent
{
	GENERATED_BODY()

public:
	UGstAppSrcComponent();

	// UActorComponent
	void UninitializeComponent() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// UGstElementComponent
	void CbPipelineStart(class IGstPipeline* Pipeline) override;
	void CbPipelineStop() override;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString AppSrcName;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	bool AppSrcEnabled;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere)
	TArray<FComponentReference> AppSrcCaptures;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1", ClampMax="10", UIMin="1", UIMax="10"))
	int MaxQueueLength = 5;

	UFUNCTION(Category="GstAppSrc", BlueprintCallable)
	void SetCaptureInterval(float Interval);

protected:

	void ResetState();

	void PushBufferAsync(class FTextureRenderTargetResource* TextureResource);

	friend class FGstAppSrcBuffer;
	FGstAppSrcBuffer* GetBuffer();
	void ReleaseBuffer(FGstAppSrcBuffer* Buffer);
	void DestroyBuffers();

protected:

	IGstAppSrc* AppSrc = nullptr;
	TArray<FGstAppSrcBuffer*> BufferPool;
	TArray<FGstAppSrcBuffer*> BufferQueue;
	FCriticalSection PoolMx;
	size_t FrameId = 0;
};
