#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnemyAttackBase.generated.h"

class UEmotionComponent;
class UHealthComponent;
class AGrunt;

UCLASS(Blueprintable)
class GP4_API UEnemyAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (UIMin = "0", UIMax = "1"))
	float EmotionDamage = 0.5f;
	UPROPERTY(EditAnywhere, meta = (ToolTip = "If the target is in the opposite emotion state of the enemy, the enemy will deal more damage according to this multiplier."))
	float EmotionDamageMultiplier = 1.f;
	UPROPERTY(EditAnywhere)
	int32 HealthDamage = 10;
	
	virtual void AttackSetup(AActor* InAttacker, AActor* InTarget);
	virtual void Attack();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttack(AActor* OutAttacker, AActor* OutTarget);

	UPROPERTY(EditAnywhere)
	bool bShowDebug = false;

protected:
	void DamageTarget();

	UPROPERTY()
	TObjectPtr<AActor> Attacker;
	UPROPERTY()
	TObjectPtr<AGrunt> AttackerGrunt;
	UPROPERTY()
	TObjectPtr<AActor> Target;
	UPROPERTY()
	TObjectPtr<UEmotionComponent> TargetEmotionComponent;
	UPROPERTY()
	TObjectPtr<UHealthComponent> TargetHealthComponent;
	
	FVector ConstrainedDirFromAttackerToTarget();
};
