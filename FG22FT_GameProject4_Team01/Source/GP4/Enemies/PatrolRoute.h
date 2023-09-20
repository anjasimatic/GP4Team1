#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

UCLASS()
class GP4_API APatrolRoute : public AActor
{
	GENERATED_BODY()

public:
	APatrolRoute();

	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> MyPatrolPoints;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<FVector> DebugLocs;
};
