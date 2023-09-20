#include "HealthComponent.h"
#include "PlayerBlockingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GP4/DataAssets/HealthData.h"

// INITIALISATION

	UHealthComponent::UHealthComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

	void UHealthComponent::BeginPlay(){
		Super::BeginPlay();

		InitCurrentHealth();
		InitBlockingComponent();
	}

	void UHealthComponent::InitCurrentHealth(){
		if (HealthSettings)
			CurrentHealth = HealthSettings->GetMaxHealth();

		else ErrorMissingHealthSettings();
	}

	void UHealthComponent::InitBlockingComponent(){
		const auto Owner = GetOwner();

		const auto Component = Owner->GetComponentByClass(
			UPlayerBlockingComponent::StaticClass()
		);
		if (!Component) return;
		BlockingComponent = Cast<UPlayerBlockingComponent>(Component);
	}

// GETTERS

	float UHealthComponent::GetHealthPercent() const {
		return UKismetMathLibrary::NormalizeToRange(
			CurrentHealth, 0, HealthSettings->GetMaxHealth()
		);
	}

// SETTERS

	void UHealthComponent::SetHealth(int32 Amount){
		CurrentHealth = FMath::Clamp(
			Amount, 0, HealthSettings->GetMaxHealth()
		);
		OnChanged.Broadcast(CurrentHealth);
	}

	void UHealthComponent::SetInvincibility(const bool Enabled){
		bInvincible = Enabled;
		OnInvincible.Broadcast(bInvincible);
	}

// HEALTH ACTIONS

	void UHealthComponent::Damage(const int32 Amount){
		if (bDead || bInvincible) return;
		
		if (!HealthSettings){
			ErrorMissingHealthSettings();
			return;
		}
		CurrentHealth = FMath::Clamp(
			CurrentHealth - Amount,
			0,
			CurrentHealth
		);
		if (Amount > 0){
			OnDamage.Broadcast(Amount);
			OnChanged.Broadcast(CurrentHealth);
		}
		if (CurrentHealth == 0){
			bDead = true;

			OnDeath.Broadcast();
		}
	}

	void UHealthComponent::DirectionalDamage(const FVector& AttackDirection, const int32 Amount){
		if (BlockingComponent && BlockingComponent->BlocksDamage(AttackDirection)) return;
		
		Damage(Amount);
	}

	void UHealthComponent::Heal(const int32 Amount){
		if (bDead) return;
		
		if (!HealthSettings){
			ErrorMissingHealthSettings();
			return;
		} 
		CurrentHealth = FMath::Clamp(
			CurrentHealth + Amount,
			CurrentHealth,
			HealthSettings->GetMaxHealth()
		);
		if (Amount > 0){
			OnHeal.Broadcast(Amount);
			OnChanged.Broadcast(CurrentHealth);
		}
	}

	void UHealthComponent::Kill(){
		if (bDead) return;
		
		CurrentHealth = 0;
		bDead = true;

		OnDeath.Broadcast();
		OnChanged.Broadcast(CurrentHealth);
	}

	void UHealthComponent::Revive(){
		if (!bDead) return;
		
		if (!HealthSettings){
			ErrorMissingHealthSettings();
			return;
		}
		CurrentHealth = HealthSettings->GetMaxHealth();
		bDead = false;

		OnRevive.Broadcast();
		OnChanged.Broadcast(CurrentHealth);
	}

// ERRORS

	void UHealthComponent::ErrorMissingHealthSettings(){
		const auto text = "Error! Missing HealthSettings in HealthComponent";
		
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, text);
		UE_LOG(LogTemp, Error, TEXT("%hc"), *text);
	}
