#include "PlayerAttackComponent.h"
#include "EmotionComponent.h"
#include "GP4/Player/PlayerAttacks/PlayerAttackBase.h"
#include "GP4/Player/PlayerAttacks/PlayerMeleeAttack.h"
#include "GP4/Player/PlayerAttacks/PlayerRangedAttack.h"

UPlayerAttackComponent::UPlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//Create instances of hatred attacks
	for (int i = 0; i < HatredAttackSets.Num(); i++)
	{
		for (TSubclassOf<UPlayerAttackBase> Template : HatredAttackSets[i].Attacks)
		{
			TObjectPtr<UPlayerAttackBase> NewAttackObj = NewObject<UPlayerAttackBase>(this, Template);
			HatredAttackSets[i].AttackInstances.Add(NewAttackObj);
			NewAttackObj->AttackSetup();
			NewAttackObj->Damage *= HatredAttackSets[i].DamageMultiplier;
			NewAttackObj->OnAttackFinished.AddDynamic(this, &UPlayerAttackComponent::AttackFinished);
			NewAttackObj->OnAttack.AddDynamic(this, &UPlayerAttackComponent::OnAttackEvent);
			if(TObjectPtr<UPlayerRangedAttack> RangedAttack = Cast<UPlayerRangedAttack>(NewAttackObj))
			{
				RangedAttack->OnRangedAttackHit.AddDynamic(this, &UPlayerAttackComponent::RollDiceForSkipCast);
			}
		}
	}

	//Create instances of sorrow attacks
	for (int i = 0; i < SorrowAttackSets.Num(); i++)
	{
		for (TSubclassOf<UPlayerAttackBase> Template : SorrowAttackSets[i].Attacks)
		{
			TObjectPtr<UPlayerAttackBase> NewAttackObj = NewObject<UPlayerAttackBase>(this, Template);
			SorrowAttackSets[i].AttackInstances.Add(NewAttackObj);
			NewAttackObj->AttackSetup();
			NewAttackObj->Damage *= SorrowAttackSets[i].DamageMultiplier;
			NewAttackObj->OnAttackFinished.AddDynamic(this, &UPlayerAttackComponent::AttackFinished);
			NewAttackObj->OnAttack.AddDynamic(this, &UPlayerAttackComponent::OnAttackEvent);
			if(TObjectPtr<UPlayerRangedAttack> RangedAttack = Cast<UPlayerRangedAttack>(NewAttackObj))
			{
				RangedAttack->OnRangedAttackHit.AddDynamic(this, &UPlayerAttackComponent::RollDiceForSkipCast);
			}
		}
	}

	//Bind delegates
	EmotionComponent = Cast<UEmotionComponent>(GetOwner()->GetComponentByClass(UEmotionComponent::StaticClass()));
	EmotionComponent->OnEmotionsChanged.AddDynamic(this, &UPlayerAttackComponent::ReEvaluateAttackSet);
	EmotionComponent->OnStateChanged.AddDynamic(this, &UPlayerAttackComponent::ReEvaluateAttackSetState);
}

void UPlayerAttackComponent::Attack()
{
	if(HatredAttackSets.Num() == 0 || SorrowAttackSets.Num() == 0) return; //Has no attacks

	//If is busy attacking
	if(bIsBusy)  
	{
		//Do input queueing
		if(GetCurrentCastTime() > 0.f) //busy doing a ranged attack
		{
			if((1.f - GetCurrentCastTime()) <= InputQueueAllowedTimeBeforeAttackEnd)
			{
				GetWorld()->GetTimerManager().SetTimer(QueueInputHandle, this, &UPlayerAttackComponent::QueueInput, GetWorld()->GetDeltaSeconds(), true);
			}
		}else //busy doing a melee attack
		{
			if((1.f - GetCurrentMeleeTime()) <= InputQueueAllowedTimeBeforeAttackEnd)
			{
				GetWorld()->GetTimerManager().SetTimer(QueueInputHandle, this, &UPlayerAttackComponent::QueueInput, GetWorld()->GetDeltaSeconds(), true);
			}
		}
		
		return;
	}
	
	bIsBusy = true;

	//If attack to be performed is a ranged attack and can skip, apply skip
	TObjectPtr<UPlayerRangedAttack> RangedAttack = Cast<UPlayerRangedAttack>(CurrentAttackSet.AttackInstances[CurrentAttackIndex]);
	if(RangedAttack && bSkipCast)
	{
		bSkipCast = false;
		RangedAttack->bSkipCast = true;
	}
	
	//Perform attack
	CurrentAttackSet.AttackInstances[CurrentAttackIndex]->Attack();

	//Restart combo expiration timer
	GetWorld()->GetTimerManager().SetTimer(ExpireComboHandle, this, &UPlayerAttackComponent::ExpireCombo, TimeBeforeComboExpires);
}

void UPlayerAttackComponent::QueueInput()
{
	if(!bIsBusy)
	{
		Attack();
		GetWorld()->GetTimerManager().ClearTimer(QueueInputHandle);
	}
}

void UPlayerAttackComponent::ReEvaluateAttackSet(float EmotionValue)
{
	//If state was switched, reset attack index
	if(bEmotionState != EmotionComponent->GetEmotionState())
	{
		CurrentAttackIndex = 0;
	}
	bEmotionState = EmotionComponent->GetEmotionState();
	
	//Find which attack set gets used depending on the emotion level and current state
	if(bEmotionState) //Hatred state
	{
		for (int i = 0; i < HatredAttackSets.Num(); i++)
		{
			switch (HatredAttackSets[i].EmotionComparisonType)
			{
			case EEmotionComparisonType::GreaterThan:
				if(HatredAttackSets[i].EmotionThreshold < EmotionComponent->GetEmotionRange())
				{
					CurrentAttackSet = HatredAttackSets[i];
					return;
				}
				break;
			case EEmotionComparisonType::LessThan:
				if(HatredAttackSets[i].EmotionThreshold > EmotionComponent->GetEmotionRange())
				{
					CurrentAttackSet = HatredAttackSets[i];
					return;
				}
				break;
			case EEmotionComparisonType::Equal:
				if(HatredAttackSets[i].EmotionThreshold == EmotionComponent->GetEmotionRange())
				{
					CurrentAttackSet = HatredAttackSets[i];
					return;
				}
				break;
			}
		}
	}else //Sorrow state
	{
		for (int i = 0; i < SorrowAttackSets.Num(); i++)
		{
			switch (SorrowAttackSets[i].EmotionComparisonType)
			{
			case EEmotionComparisonType::GreaterThan:
				if(SorrowAttackSets[i].EmotionThreshold < EmotionComponent->GetEmotionRange())
				{
					CurrentAttackSet = SorrowAttackSets[i];
					return;
				}
				break;
			case EEmotionComparisonType::LessThan:
				if(SorrowAttackSets[i].EmotionThreshold > EmotionComponent->GetEmotionRange())
				{
					CurrentAttackSet = SorrowAttackSets[i];
					return;
				}
				break;
			case EEmotionComparisonType::Equal:
				if(SorrowAttackSets[i].EmotionThreshold == EmotionComponent->GetEmotionRange())
				{
					CurrentAttackSet = SorrowAttackSets[i];
					return;
				}
				break;
			}
		}
	}

	//Default if none of the above conditions are met
	CurrentAttackSet = HatredAttackSets[0];
}

void UPlayerAttackComponent::ReEvaluateAttackSetState(bool State)
{
	ReEvaluateAttackSet();
}

void UPlayerAttackComponent::CancelAttack()
{
	//Cancel attack
	CurrentAttackSet.AttackInstances[CurrentAttackIndex]->CancelAttack();
	ExpireCombo();
	//Remove queued attack
	GetWorld()->GetTimerManager().ClearTimer(QueueInputHandle);
}

void UPlayerAttackComponent::ExpireCombo()
{
	CurrentAttackIndex = 0;
}

void UPlayerAttackComponent::AttackFinished(bool bCanceled)
{
	if(!bCanceled)
	{
		//Increment index
		CurrentAttackIndex++;
		//Check if index is above the length of the attack set
		if(CurrentAttackIndex >= CurrentAttackSet.AttackInstances.Num())
		{
			CurrentAttackIndex = 0;
		}
	}
	
	bIsBusy = false;
}

void UPlayerAttackComponent::RollDiceForSkipCast()
{
	if(CastSkipEmotionThreshold > EmotionComponent->GetEmotionRange()) return; //Not within emotion threshold to do skip
	
	float Roll = FMath::RandRange(0.f, 1.f);
	if(Roll <= CastSkipChance)
	{
		bSkipCast = true;
	}
}

float UPlayerAttackComponent::GetCurrentCastTime()
{
	//Find current ranged attack if it exists
	TObjectPtr<UPlayerRangedAttack> RangedAttack;
	RangedAttack = Cast<UPlayerRangedAttack>(CurrentAttackSet.AttackInstances[CurrentAttackIndex]);

	if(RangedAttack)
	{
		return RangedAttack->GetCurrentCastTime();
	}

	return -1.f;
}

float UPlayerAttackComponent::GetCurrentMeleeTime()
{
	//Find current melee attack if it exists
	TObjectPtr<UPlayerMeleeAttack> MeleeAttack;
	MeleeAttack = Cast<UPlayerMeleeAttack>(CurrentAttackSet.AttackInstances[CurrentAttackIndex]);

	if(MeleeAttack)
	{
		return MeleeAttack->GetCurrentMeleeTime();
	}

	return -1.f;
}

void UPlayerAttackComponent::OnAttackEvent(bool IsMelee)
{
	OnAttack.Broadcast(IsMelee);
}
