#include "EnemyAttackComponent.h"
#include "GP4/Enemies/EnemyAttacks/EnemyAttackBase.h"
#include "GP4/Enemies/EnemyAttacks/EnemyRangedAttack.h"
#include "Kismet/GameplayStatics.h"

UEnemyAttackComponent::UEnemyAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//Create instances of all attacks
	for (int i = 0; i < AttackSets.Num(); i++)
	{
		for (TSubclassOf<UEnemyAttackBase> Template : AttackSets[i].Attacks)
		{
			TObjectPtr<UEnemyAttackBase> NewObj = NewObject<UEnemyAttackBase>(this, Template);
			AttackSets[i].AttackInstances.Add(NewObj);
			NewObj->AttackSetup(GetOwner(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		}
	}
}

void UEnemyAttackComponent::Attack()
{
	if(bIsBusy) return; //Is busy attacking
	if(AttackSets[AttackSetIndex].Attacks.Num() == 0) return; //Has no attacks to pick from
	
	bIsBusy = true;

	//Get and perform a random attack
	int RandomAttackInd = FMath::RandRange(0, AttackSets[AttackSetIndex].AttackInstances.Num() - 1);
	AttackSets[AttackSetIndex].AttackInstances[RandomAttackInd]->Attack();

	bIsBusy = false;
}

float UEnemyAttackComponent::GetCurrentCastTime()
{
	//Find current ranged attack if it exists
	TObjectPtr<UEnemyRangedAttack> RangedAttack;
	RangedAttack = Cast<UEnemyRangedAttack>(AttackSets[AttackSetIndex].AttackInstances[0]);

	if(RangedAttack)
	{
		return RangedAttack->GetCurrentCastTime();
	}

	return -1.f;
}

float UEnemyAttackComponent::GetCurrentMeleeTime()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(MeleeAttackTimer))
	{
		float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MeleeAttackTimer);
		return ElapsedTime / MeleeAttackTime;
	}

	return -1.f;
}

void UEnemyAttackComponent::StartMeleeAttackTimer(float InTime)
{
	MeleeAttackTime = InTime;
	GetWorld()->GetTimerManager().SetTimer(MeleeAttackTimer, this, &UEnemyAttackComponent::FinishMeleeAttackTimer, MeleeAttackTime);
}

void UEnemyAttackComponent::FinishMeleeAttackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(MeleeAttackTimer);
}
