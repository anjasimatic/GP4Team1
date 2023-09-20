#include "Boss.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP4/Components/EnemyAttackComponent.h"
#include "GP4/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<AEnemyAIController>(GetController());
	MyController->GetBlackboardComponent()->SetValueAsInt(FName("BossPhase"), 0);
	HealthComponent->OnChanged.AddDynamic(this, &ABoss::ReEvaluateAttackSet);
}

void ABoss::ReEvaluateAttackSet(int Hp)
{
	//find attack set
	int AttackSetIndex = 0;
	for(int i = 0; i < BossPhases.Num(); i++)
	{
		if(HealthComponent->GetHealthPercent() <= BossPhases[i].BelowThisHpPercentage)
		{
			AttackSetIndex = BossPhases[i].SwitchToThisAttackSet;
		}
	}

	//set boss phase value in blackboard
	MyController->GetBlackboardComponent()->SetValueAsInt(FName("BossPhase"), AttackSetIndex);

	//change attack set used
	AttackComponent->AttackSetIndex = AttackSetIndex;
}

void ABoss::Teleport()
{
	if(!bCanTeleport) return;
	bCanTeleport = false;

	//teleport
	SetActorLocation(GetRandomTeleportLocation());

	//face player
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
	SetActorRotation(FRotator(0.f, Rot.Yaw, 0.f));
	
	//start cooldown timer
	float DeviatedTime = TimeBetweenTeleports + (FMath::RandRange(-1.f, 1.f) * TimeDeviation);
	GetWorld()->GetTimerManager().SetTimer(TeleportHandle, this, &ABoss::AllowTeleporting, DeviatedTime);
}

void ABoss::AllowTeleporting()
{
	bCanTeleport = true;
}

FVector ABoss::GetRandomTeleportLocation()
{
	if(TeleportLocations.Num() == 0) return GetActorLocation();
	
	int RandIndex;
	do
	{
		RandIndex = FMath::RandRange(0, TeleportLocations.Num() - 1);
	}
	while (CurrentTeleportLocation == RandIndex);

	CurrentTeleportLocation = RandIndex;

	return TeleportLocations[CurrentTeleportLocation]->GetActorLocation();
}
