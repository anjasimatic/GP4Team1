#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProgressSubsystem.generated.h"

UCLASS()
class GP4_API UProgressSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	int LifeStageProgress;

public:
	UFUNCTION(BlueprintCallable)
	void SetLifeStageProgress(int NewStage);
	UFUNCTION(BlueprintCallable)
	int GetLifeStageProgress() const;
	
};