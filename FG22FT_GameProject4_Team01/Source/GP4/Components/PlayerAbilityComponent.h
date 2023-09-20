#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAbilityComponent.generated.h"

class UAbilityData;

UCLASS(ClassGroup=(Abilities), meta=(BlueprintSpawnableComponent))
class GP4_API UPlayerAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerAbilityComponent();

	UAbilityData* GetSorrowMeleeAbility()  const { return SorrowMeleeAbility;  }
	UAbilityData* GetSorrowRangedAbility() const { return SorrowRangedAbility; }
	UAbilityData* GetHatredMeleeAbility()  const { return HatredMeleeAbility;  }
	UAbilityData* GetHatredRangedAbility() const { return HatredRangedAbility; }
	
protected:

	UPROPERTY(Category="Abilities", EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilityData> SorrowMeleeAbility = nullptr;

	UPROPERTY(Category="Abilities", EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilityData> SorrowRangedAbility = nullptr;
	
	UPROPERTY(Category="Abilities", EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilityData> HatredMeleeAbility = nullptr;

	UPROPERTY(Category="Abilities", EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAbilityData> HatredRangedAbility = nullptr;
	
};
