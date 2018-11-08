#include "GstElementComponent.h"

UGstElementComponent::UGstElementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGstElementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UGstElementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGstElementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UGstElementComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

void UGstElementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
