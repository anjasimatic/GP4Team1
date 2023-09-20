#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP4/DataAssets/HealthData.h"
#include "HealthComponent.generated.h"

class UPlayerBlockingComponent;
class UHealthData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReviveDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDelegate, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealDelegate, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, int32, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvincibilityDelegate, bool, Invincible);

UCLASS(ClassGroup=(Health), meta=(BlueprintSpawnableComponent))
class GP4_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UHealthComponent();

	
	UPROPERTY(BlueprintAssignable)
	FOnDeathDelegate OnDeath;
	
	UPROPERTY(BlueprintAssignable)
	FOnReviveDelegate OnRevive;
	
	UPROPERTY(BlueprintAssignable)
	FOnDamageDelegate OnDamage;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealDelegate OnHeal;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnInvincibilityDelegate OnInvincible;
	

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, meta=(ToolTip="Returns current health betwen 0.0 and 1.0"))
	float GetHealthPercent() const;
	

	UFUNCTION(BlueprintCallable)
	void SetHealth(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void SetInvincibility(const bool Enabled);

	
	UFUNCTION(BlueprintCallable)
	void Damage(const int32 Amount);

	UFUNCTION(BlueprintCallable)
	void DirectionalDamage(const FVector& AttackDirection, const int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void Heal(const int32 Amount);

	UFUNCTION(BlueprintCallable)
	void Kill();
	
	UFUNCTION(BlueprintCallable)
	void Revive();
	
protected:

	UPROPERTY(Category="Health", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthData> HealthSettings = nullptr;

	
	virtual void BeginPlay() override;

private:

	bool bDead = false;
	bool bInvincible = false;
	int32 CurrentHealth = 0;

	UPROPERTY()
	UPlayerBlockingComponent* BlockingComponent = nullptr;
	
	
	static void ErrorMissingHealthSettings();
	
	void InitCurrentHealth();
	void InitBlockingComponent();
	
};
