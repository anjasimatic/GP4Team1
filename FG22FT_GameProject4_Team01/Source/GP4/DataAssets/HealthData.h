#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HealthData.generated.h"

UCLASS()
class GP4_API UHealthData : public UDataAsset
{
	GENERATED_BODY()

public:

	int32 GetMaxHealth() const { return MaxHealth; }

protected:

	UPROPERTY(Category="Health", EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxHealth = 100;
	
};
