#include "AbilityData.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// ABILITY ACTIONS

	void UAbilityData::Update(const float EmotionValue){
		if (EmotionValue >= ActivationValue && !bActivated){
			bActivated = true;
			OnActivated.Broadcast();
			
		} else if (EmotionValue < ActivationValue && bActivated){
			bActivated = false;
			OnDeactivated.Broadcast();
		}
	}

	UNiagaraComponent* UAbilityData::SpawnEffect(const AActor* Target){
		if (Effect.IsNull()){
			ErrorMissingEffect();
			return nullptr;
		}
		const auto Position = Target->GetActorLocation() + PositionOffset;
		const auto Rotation = Target->GetActorRotation() + RotationOffset;
		const auto World = GetWorld();
		
		const auto FXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World, Effect.Get(), Position, Rotation, ScaleOverride,
			bAutoDestroy, bAutoActivate
		);
		if (!bAttachToTarget)
			return FXComp;

		FXComp->AttachToComponent(
			Target->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
		);
		return FXComp;
	}

// ERRORS

	void UAbilityData::ErrorMissingEffect(){
		const auto text = "Error! Missing NiagaraSystem Effect in AbilityData";
		
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, text);
		UE_LOG(LogTemp, Error, TEXT("%hc"), *text);
	}
