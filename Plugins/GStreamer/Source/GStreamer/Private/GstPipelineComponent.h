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

	UFUNCTION(Category="GstPipeline", BlueprintCallable)
	static void SetVerbosity(int Verbosity);

protected:

	void ResetState();

	IGstPipeline* Pipeline = nullptr;
};
