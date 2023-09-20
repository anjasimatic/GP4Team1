#pragma once

#include "CoreMinimal.h"
#include "PlayerAttackBase.h"
#include "PlayerRangedAttack.generated.h"

class AProjectile;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRangedAttackHit);

UCLASS()
class GP4_API UPlayerRangedAttack : public UPlayerAttackBase
{
	GENERATED_BODY()

public:
	virtual void AttackSetup() override;
	virtual void Attack() override;
	virtual void CancelAttack() override;

	UPROPERTY(EditAnywhere)
	float CastTime = 1.f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> CastingNiagaraSystem;
	UPROPERTY(EditAnywhere)
	float DistanceInFrontOfPlayerToCastAt = 100.f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> Projectile;

	float GetCurrentCastTime();

	FOnRangedAttackHit OnRangedAttackHit;

	bool bSkipCast = false;

private:
	FTimerHandle CastHandle;
	void FinishCast();

	UPROPERTY()
	TObjectPtr<AProjectile> SpawnedProj;

	UFUNCTION()
	void EnemyDamaged();
};
