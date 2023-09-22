#include "PlayerBlockingComponent.h"

// INITIALISATION

	UPlayerBlockingComponent::UPlayerBlockingComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

	void UPlayerBlockingComponent::BeginPlay(){
		Super::BeginPlay();
		
		Owner = GetOwner();
	}

// BLOCKING ACTIONS

	void UPlayerBlockingComponent::SetBlocking(bool Blocking){
		bBlocking = Blocking;
	}

	bool UPlayerBlockingComponent::BlocksDamage(const FVector& AttackDirection){
		if (!bBlocking){
			OnBlocking.Broadcast(false);
			return false;
		}
		const auto PlayerPos  = Owner->GetActorLocation();
		const auto PlayerFwrd = Owner->GetActorForwardVector();
		
		const auto Dot = PlayerFwrd.Dot(-AttackDirection);
		const bool bBlocked = FMath::Lerp(180, 0, Dot * 0.5f + 0.5f) < BlockingAngle * 0.5f;

		OnBlocking.Broadcast(bBlocked);
		return bBlocked;
	}
