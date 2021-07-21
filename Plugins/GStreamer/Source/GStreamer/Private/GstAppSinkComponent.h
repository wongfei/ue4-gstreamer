#pragma once

#include "GstElementComponent.h"
#include "GstAppSinkImpl.h"
#include "GstTexture.h"
#include "GstVideoFormat.h"
#include "GstAppSinkComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FGstTextureCreatedSignature, UGstAppSinkComponent*, AppSink, UTexture2D*, NewTexture, EGstVideoFormat, Format, int, Width, int, Height);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSTREAMER_API UGstAppSinkComponent : public UGstElementComponent, public IGstAppSinkCallback, public IGstTextureCallback
{
	GENERATED_BODY()

public:	

	UGstAppSinkComponent();

	// UActorComponent
	void UninitializeComponent() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// UGstElementComponent
	void CbPipelineStart(class IGstPipeline* Pipeline) override;
	void CbPipelineStop() override;

	// IGstAppSinkCallback
	void CbGstSampleReceived(class IGstSample* Sample) override;

	// IGstTextureCallback
	void CbGstTextureCreated() override;

	UPROPERTY(Category="GstAppSink", EditAnywhere, BlueprintReadWrite)
	FString AppSinkName;

	UPROPERTY(Category="GstAppSink", EditAnywhere, BlueprintReadWrite)
	bool AppSinkEnabled;

	UFUNCTION(Category="GstAppSink", BlueprintCallable)
	UTexture2D* GetTexture() { return Texture->GetTextureObject(); }

	UPROPERTY(Category="GstAppSink", BlueprintAssignable)
	FGstTextureCreatedSignature OnGstTextureCreated;

protected:

	void ResetState();

	IGstAppSink* AppSink = nullptr;
	FGstTexture* Texture = nullptr;
};
