#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBase.h"
#include "EnemyRangedAttack.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable)
class GP4_API UEnemyRangedAttack : public UEnemyAttackBase
{
	GENERATED_BODY()
	
public:
	virtual void Attack() override;

	UPROPERTY(EditAnywhere)
	float TimeBeforeExplosion = 1.f;
	UPROPERTY(EditAnywhere)
	float ExplosionRange = 300.f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> ExplosionDecal;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ExplosionNiagaraSystem;

	float GetCurrentCastTime();

private:
	FTimerHandle RangedAttackHandle;
	void DoRangedAttackTimer();

	FVector ExplosionLoc;
};
