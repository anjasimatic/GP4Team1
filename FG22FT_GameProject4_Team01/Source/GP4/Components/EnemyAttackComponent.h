#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackComponent.generated.h"

class UEnemyAttackBase;

USTRUCT()
struct FEnemyAttackSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UEnemyAttackBase>> Attacks;
	UPROPERTY()
	TArray<TObjectPtr<UEnemyAttackBase>> AttackInstances;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_API UEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyAttackComponent();

	bool bIsBusy = false;

	UPROPERTY(EditAnywhere)
	TArray<FEnemyAttackSet> AttackSets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackSetIndex = 0;

	UFUNCTION(BlueprintCallable)
	void Attack();
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentCastTime();
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentMeleeTime();
	float MeleeAttackTime = -1.f;
	
	UFUNCTION(BlueprintCallable)
	void StartMeleeAttackTimer(float InTime);
	void FinishMeleeAttackTimer();
	FTimerHandle MeleeAttackTimer;

protected:
	virtual void BeginPlay() override;
};
