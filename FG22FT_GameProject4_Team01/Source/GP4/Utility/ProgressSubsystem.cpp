#include "ProgressSubsystem.h"

void UProgressSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LifeStageProgress = 0;
	Super::Initialize(Collection);
}

void UProgressSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UProgressSubsystem::SetLifeStageProgress(const int NewStage)
{
	LifeStageProgress = NewStage;
}

int UProgressSubsystem::GetLifeStageProgress() const
{
	return LifeStageProgress;
}