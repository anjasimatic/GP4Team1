#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4CharacterMovementComponent.generated.h"

UCLASS()
class GP4_API UGP4CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UGP4CharacterMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
