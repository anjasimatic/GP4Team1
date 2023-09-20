#include "GP4CharacterMovementComponent.h"

UGP4CharacterMovementComponent::UGP4CharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGP4CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UGP4CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

