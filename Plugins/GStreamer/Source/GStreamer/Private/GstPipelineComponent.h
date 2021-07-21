#pragma once

#include "GstElementComponent.h"
#include "GstPipelineImpl.h"
#include "GstPipelineComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSTREAMER_API UGstPipelineComponent : public UGstElementComponent
{
	GENERATED_BODY()

public:	

	UGstPipelineComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void UninitializeComponent() override;

	UPROPERTY(Category="GstPipeline", EditAnywhere, BlueprintReadWrite)
	FString PipelineConfig;

	UPROPERTY(Category="GstPipeline", EditAnywhere, BlueprintReadWrite)
	bool PipelineAutostart;

	UFUNCTION(Category="GstPipeline", BlueprintCallable)
	bool StartPipeline();

	UFUNCTION(Category="GstPipeline", BlueprintCallable)
	void StopPipeline();

protected:

	void ResetState();

	IGstPipeline* Pipeline = nullptr;
};
