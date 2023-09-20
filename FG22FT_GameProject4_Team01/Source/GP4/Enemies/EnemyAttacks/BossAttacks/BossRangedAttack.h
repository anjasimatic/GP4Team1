#pragma once

#include "CoreMinimal.h"
#include "GP4/Enemies/EnemyAttacks/EnemyAttackBase.h"
#include "BossRangedAttack.generated.h"

class UNiagaraSystem;

UCLASS()
class GP4_API UBossRangedAttack : public UEnemyAttackBase
{
	GENERATED_BODY()

public:
	virtual void Attack() override;

	UPROPERTY(EditAnywhere)
	int AmountOfExplosions = 4;
	UPROPERTY(EditAnywhere)
	float TimeBeforeExplosion = 1.f;
	UPROPERTY(EditAnywhere)
	float ExplosionRange = 100.f;
	UPROPERTY(EditAnywhere)
	float ExplosionSpawnRangeAroundPlayer = 1000.f;
	UPROPERTY(EditAnywhere)
	float ExplosionSeparationAmount = 200.f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> ExplosionDecal;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ExplosionNiagaraSystem;
	
private:
	FTimerHandle RangedAttackHandle;
	void DoRangedAttackTimer();

	TArray<FVector> ExplosionLocs;
};
