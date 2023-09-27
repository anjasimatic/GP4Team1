#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerAttackBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackFinishedDelegate, bool, bCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttack, bool, IsMelee);

class UEmotionComponent;
class AGP4Character;

UCLASS(Blueprintable)
class GP4_API UPlayerAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Damage = 10.f;
	UPROPERTY(EditAnywhere)
	float DamageMultiplierToOppositeEmotion = 1.f;
	UPROPERTY(EditAnywhere, meta = (UIMin = "0", UIMax = "1", ToolTip = "Amount to increase highest emotion amount by whenever the player successfully hits an enemy."))
	float EmotionIncreaseAmount = 0.1f;
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.f;
	UPROPERTY(EditAnywhere)
	float FreezePlayerTime = 0.f;

	virtual void AttackSetup();
	virtual void Attack();
	virtual void CancelAttack();

	UPROPERTY(BlueprintAssignable)
	FOnAttackFinishedDelegate OnAttackFinished;

	UPROPERTY(EditAnywhere)
	bool bShowDebug = false;

	FOnAttack OnAttack;

protected:
	UPROPERTY()
	TObjectPtr<AGP4Character> Player;
	UPROPERTY()
	TObjectPtr<UEmotionComponent> PlayerEmotionComponent;

	FTimerHandle CooldownHandle;
	void StartCooldown();
	void EndCooldown();
	FTimerHandle FreezePlayerHandle;
	void StartFreezePlayer();
	void EndFreezePlayer();

	bool bCanceled = false;

	UFUNCTION()
	void DamageSelfEmotion();
};
