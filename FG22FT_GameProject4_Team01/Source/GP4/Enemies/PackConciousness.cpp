#include "PackConciousness.h"
#include "EnemyAIController.h"
#include "Grunt.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP4/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

APackConciousness::APackConciousness()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APackConciousness::BeginPlay()
{
	Super::BeginPlay();

	//Get all grunts
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrunt::StaticClass(), AllGrunts);

	//Remove any non anger grunts
	for(int i = 0; i < AllGrunts.Num(); i++)
	{
		if(Cast<AGrunt>(AllGrunts[i])->MyEmotion != EEmotionType::Anger) AllGrunts.RemoveAt(i);
	}

	//Bind delegate to on enemy aggro
	for (TObjectPtr<AActor> Grunt : AllGrunts)
	{
		TObjectPtr<AEnemyAIController> Controller = Cast<AEnemyAIController>(Cast<APawn>(Grunt)->GetController());
		Controller->OnAggro.AddUniqueDynamic(this, &APackConciousness::OnEnemyAggro);
	}
}

void APackConciousness::OnEnemyAggro(APawn* Enemy)
{
	AggroControllers.AddUnique(Cast<AEnemyAIController>(Enemy->GetController()));
	ReEvaluateWhoseTurnItIs();
}

void APackConciousness::ReEvaluateWhoseTurnItIs()
{
	if(AggroControllers.IsEmpty()) return;

	//Stop current grunt whose turn it is 
	for(int i = 0; i < AggroControllers.Num(); i++)
	{
		AggroControllers[i]->GetBlackboardComponent()->SetValueAsBool(TEXT("IsMyTurn"), false);
	}

	//Find out which enemy is best fit to attack next
	int BestIndex = 0;
	float ClosestDist = 100000.f;
	for(int i = 0; i < AggroControllers.Num(); i++)
	{
		TObjectPtr<APawn> Pawn = AggroControllers[i]->GetPawn();
		if(!Pawn) continue;
		
		float Dist = FVector::Distance(Pawn->GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
		if(Dist < ClosestDist)
		{
			ClosestDist = Dist;
			BestIndex = i;
		}
	}

	//Let chosen enemy attack
	AggroControllers[BestIndex]->GetBlackboardComponent()->SetValueAsBool(TEXT("IsMyTurn"), true);
	GruntWhoseTurnItIs = AggroControllers[BestIndex]->GetPawn();
	Cast<UHealthComponent>(GruntWhoseTurnItIs->GetComponentByClass(UHealthComponent::StaticClass()))->OnDeath.AddUniqueDynamic(this, &APackConciousness::OnEnemyDeath);
}

void APackConciousness::OnEnemyDeath()
{
	//Cleanup
	for(int i = 0; i < AggroControllers.Num(); i++)
	{
		if(!AggroControllers[i]->GetPawn()) AggroControllers.RemoveAt(i);
	}
	
	ReEvaluateWhoseTurnItIs();
}
