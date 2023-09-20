#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBase.h"
#include "MeleeAttack.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable)
class GP4_API UMeleeAttack : public UEnemyAttackBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Range = 100.f;
	UPROPERTY(EditAnywhere, meta = (UIMin = "0", UIMax = "360"))
	float Angle = 90.f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> SwingNiagaraSystem;
	UPROPERTY(EditAnywhere)
	float SwingDistanceInFrontOfEnemy = 100.f;

	virtual void Attack() override;
};
