#include "PlayerMeleeAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "GP4/Components/EmotionComponent.h"
#include "GP4/Components/HealthComponent.h"
#include "GP4/Enemies/EnemyAIController.h"
#include "GP4/Enemies/EnemyBase.h"
#include "GP4/Enemies/Grunt.h"
#include "GP4/Player/GP4Character.h"

void UPlayerMeleeAttack::AttackSetup()
{
	Super::AttackSetup();

	StepAmount *= SwingTime;
	StepAngle = SwingTotalAngle / StepAmount;
	StepRollAngle = SwingRollAngle / StepAmount;
	TraceShape = FCollisionShape::MakeBox(FVector(SwingRange, ShapeThickness, 10.f));
}

void UPlayerMeleeAttack::Attack()
{
	Super::Attack();
	
	//Reset values
	CurrentStep = 0;
	if(bFlipAttack)
	{
		CurrentRot = Player->GetActorForwardVector().Rotation() - FRotator(0.f, SwingTotalAngle / 2.f, 0.f) + FRotator(0.f, 0.f, SwingRollAngle);
	}else
	{
		CurrentRot = Player->GetActorForwardVector().Rotation() + FRotator(0.f, SwingTotalAngle / 2.f, 0.f) + FRotator(0.f, 0.f, SwingRollAngle);
	}
	HitEnemies.Empty();
	
	//Start freeze player timer
	StartFreezePlayer();
	
	//Spawn niagara system
	UNiagaraFunctionLibrary::SpawnSystemAttached(SwingNiagaraSystem, Player->GetDefaultAttachComponent(), FName("SwingSocket"), FVector(SwingDistanceInFrontOfPlayer, 0.f, 0.f), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	//Forward step
	Player->LaunchCharacter(Player->GetActorForwardVector() * PlayerForwardStepPower, true, false);

	OnAttack.Broadcast(true);
	
	//Start attack
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UPlayerMeleeAttack::AttackTimer, GetWorld()->GetDeltaSeconds(), true, SwingStartTimeOffset);
}

void UPlayerMeleeAttack::AttackTimer()
{
	//Increment rotation of hit collider
	if(bFlipAttack)
	{
		CurrentRot += FRotator(0.f, StepAngle, 0.f);
	}else
	{
		CurrentRot -= FRotator(0.f, StepAngle, 0.f);
	}
	CurrentRot += FRotator(StepRollAngle, 0.f, 0.f);

	//Increment position of hit collider
	CurrentLoc = Player->GetActorLocation() + CurrentRot.Vector() * SwingRange;

	//Debug hitbox
	if(bShowDebug)
	{
		DrawDebugBox(GetWorld(), CurrentLoc, FVector(SwingRange, ShapeThickness, 10.f), CurrentRot.Quaternion(), FColor::Red, false, 1, 0, 10.f);	
	}

	//Check if enemy is in hit collider
	FHitResult Hit;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(TEXT(""), false, Player); //Ignores self
	GetWorld()->SweepSingleByObjectType(Hit, CurrentLoc, CurrentLoc, CurrentRot.Quaternion(), ECollisionChannel::ECC_Pawn, TraceShape, TraceParams);

	//Damage enemy if hit, don't damage same enemy multiple times
	if(!HitEnemies.Contains(Hit.GetActor()) && Cast<AEnemyBase>(Hit.GetActor()))
	{
		//Figure out how much damage to deal
		float DamageToDeal = Damage;
		if(TObjectPtr<AGrunt> Grunt = Cast<AGrunt>(Hit.GetActor())) //Hit enemy is a grunt
		{
			//If player and enemy are of opposite states, deal more damage
			if(PlayerEmotionComponent->GetEmotionState() && Grunt->MyEmotion == EEmotionType::Sadness || !PlayerEmotionComponent->GetEmotionState() && Grunt->MyEmotion == EEmotionType::Anger) 
			{
				DamageToDeal *= DamageMultiplierToOppositeEmotion;
			}
		}
		
		//Damage enemy
		Cast<UHealthComponent>(Hit.GetActor()->GetComponentByClass(UHealthComponent::StaticClass()))->Damage(DamageToDeal); //Damage enemy

		//Stagger enemy
		if(Staggers)
		{
			TObjectPtr<APawn> Pawn = Cast<APawn>(Hit.GetActor());
			if(Pawn)
			{
				TObjectPtr<AEnemyAIController> Controller = Cast<AEnemyAIController>(Pawn->GetController());
				Controller->Stagger(StaggerDuration);
			}
		}

		//Increase player's highest emotion state since hit was successful
		DamageSelfEmotion(); 

		//Add enemy to array to not hit the same enemy multiple times during the same attack
		HitEnemies.Add(Hit.GetActor()); 
	}

	//Increment step
	CurrentStep++;
	//Reached final step or canceled, finish attack
	if(bCanceled || CurrentStep >= StepAmount)
	{
		StartCooldown();
		//Stop this attack timer
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
}
