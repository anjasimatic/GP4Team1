#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PackConciousness.generated.h"

class AEnemyAIController;

UCLASS()
class GP4_API APackConciousness : public AActor
{
	GENERATED_BODY()

public:
	APackConciousness();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> AllGrunts;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AEnemyAIController>> AggroControllers;
	UPROPERTY()
	TObjectPtr<APawn> GruntWhoseTurnItIs;

	UFUNCTION()
	void OnEnemyAggro(APawn* Enemy);
	UFUNCTION()
	void OnEnemyDeath();

	UFUNCTION()
	void ReEvaluateWhoseTurnItIs();
};
