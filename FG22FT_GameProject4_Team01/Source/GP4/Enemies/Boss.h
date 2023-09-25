#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Boss.generated.h"

class AEnemyAIController;

USTRUCT()
struct FBossPhase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (UIMIn = "0", UIMax = "1"))
	float BelowThisHpPercentage = 0.5f;
	UPROPERTY(EditAnywhere)
	int SwitchToThisAttackSet = 0;
};

UCLASS()
class GP4_API ABoss : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABoss();

	UPROPERTY(EditAnywhere)
	TArray<FBossPhase> BossPhases;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> TeleportLocations;
	UPROPERTY(EditAnywhere)
	float TimeBetweenTeleports = 3.f;
	UPROPERTY(EditAnywhere)
	float TimeDeviation = 1.f;
	UFUNCTION(BlueprintCallable)
	void Teleport();

	UFUNCTION(BlueprintCallable)
	void PauseBoss(float Duration);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReEvaluateAttackSet(int Hp);

	TObjectPtr<AEnemyAIController> MyController;

	bool bCanTeleport = true;
	FVector GetRandomTeleportLocation();
	int CurrentTeleportLocation = -1;
	FTimerHandle TeleportHandle;
	void AllowTeleporting();
};
