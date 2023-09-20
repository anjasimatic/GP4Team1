#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "AbilityData.generated.h"

class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityDelegate);

UCLASS()
class GP4_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnActivated;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnDeactivated;

	
	bool GetRequiresLockOn()   const { return bRequiresLockOn; }
	float GetActivationValue() const { return ActivationValue; }
	float GetAbilityCooldown() const { return AbilityCooldown; }

	bool GetCauseKnockback() const { return bCauseKnockback; }
	float GetAttackSpeed()   const { return AttackSpeed; }
	float GetAttackDamage()  const { return AttackDamage; }
	float GetAttackRange()   const { return AttackRange; }
	
	float GetSpeedInfluence()    const { return SpeedInfluence; }
	float GetInfluenceDuration() const { return InfluenceDuration; }
	float GetInfluenceDecayDuration() const { return InfluenceDecayDuration; }
	
	bool GetAutoActivate()   const { return bAutoActivate; }
	bool GetAutoDestroy()    const { return bAutoDestroy; }
	bool GetAttachToTarget() const { return bAttachToTarget; }
	FVector GetPositionOffset()  const { return PositionOffset; }
	FRotator GetRotationOffset() const { return RotationOffset; }
	FVector GetScaleOverride()   const { return ScaleOverride; }
	UNiagaraSystem* GetEffect()  const { return Effect.Get(); }
	
	UMaterialInstance* GetUIMaterial() const { return UIMaterial.Get(); }
		

	UFUNCTION(BlueprintCallable)
	void Update(const float EmotionValue);

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* SpawnEffect(const AActor* Target); 
	
protected:

	UPROPERTY(Category="Requirements", EditDefaultsOnly, BlueprintReadOnly)
	bool bRequiresLockOn = false;
	
	UPROPERTY(Category="Requirements", EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin="0.0", UIMax="1.0"))
	float ActivationValue = 0.5f;
	
	UPROPERTY(Category="Requirements", EditDefaultsOnly, BlueprintReadOnly)
	float AbilityCooldown = 1.0f;
	

	UPROPERTY(Category="Attacks", EditDefaultsOnly, BlueprintReadOnly)
	bool bCauseKnockback = false;
	
	UPROPERTY(Category="Attacks", EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeed = 1.0f;

	UPROPERTY(Category="Attacks", EditDefaultsOnly, BlueprintReadOnly)
	float AttackDamage = 1.0f;

	UPROPERTY(Category="Attacks", EditDefaultsOnly, BlueprintReadOnly)
	float AttackRange = 1.0f;

	
	UPROPERTY(Category="Influence", EditDefaultsOnly, BlueprintReadOnly)
	float SpeedInfluence = 1.0f;

	UPROPERTY(Category="Influence", EditDefaultsOnly, BlueprintReadOnly)
	float InfluenceDuration = 0.0f;

	UPROPERTY(Category="Influence", EditDefaultsOnly, BlueprintReadOnly)
	float InfluenceDecayDuration = 0.0f;
	
	
	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	bool bAutoActivate = false;

	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	bool bAutoDestroy = false;
	
	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	bool bAttachToTarget = false;
	
	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	FVector PositionOffset = FVector::Zero();

	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	FVector ScaleOverride = FVector::One();
	
	UPROPERTY(Category="Effects", EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> Effect = nullptr;
	
	
	UPROPERTY(Category="Interface", EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInstance> UIMaterial = nullptr;

private:

	bool bActivated = false;
	
	static void ErrorMissingEffect();
	
};
