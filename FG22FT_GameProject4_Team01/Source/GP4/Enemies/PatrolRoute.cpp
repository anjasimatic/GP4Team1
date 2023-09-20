#include "PatrolRoute.h"
#include "Misc/CString.h"

APatrolRoute::APatrolRoute()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();

	//Get all patrol point scene components
	TArray<UActorComponent*> Components = GetComponents().Array();
	for (UActorComponent* Component : Components)
	{
		MyPatrolPoints.Add(Cast<USceneComponent>(Component));
	}

	//Sort after name
	Algo::Sort(MyPatrolPoints, [](const TObjectPtr<USceneComponent> Comp1, const TObjectPtr<USceneComponent> Comp2) {
	   int Name1 = FCString::Atoi(*Comp1.GetName());
	   int Name2 = FCString::Atoi(*Comp2.GetName());
	   return Name1 < Name2;
   });
}

#pragma region DEBUG
bool APatrolRoute::ShouldTickIfViewportsOnly() const
{
	return true;
}

void APatrolRoute::Tick(float DeltaSeconds)
{
	//DEBUG
	if(!IsSelected()) return;
	
	DebugLocs.Empty();
	TArray<UActorComponent*> Components = GetComponents().Array();
	for (UActorComponent* Component : Components)
	{
		if(Component == RootComponent) continue;
		DebugLocs.Add(Cast<USceneComponent>(Component)->GetComponentLocation());
	}
	
	for (int i = 0; i < DebugLocs.Num(); i++)
	{
		DrawDebugSphere(GetWorld(), DebugLocs[i], 30.f, 6, FColor::MakeRedToGreenColorFromScalar((float)i / (float)(DebugLocs.Num() - 1)), false, -1, 0, 10.f);
	}
}
#pragma endregion