#include "PlayerRangedAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile.h"
#include "GP4/Components/EmotionComponent.h"
#include "GP4/Player/GP4Character.h"

void UPlayerRangedAttack::AttackSetup()
{
	Super::AttackSetup();
}

void UPlayerRangedAttack::Attack()
{
	Super::Attack();
	
	//casting vfx
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingNiagaraSystem, Player->GetDefaultAttachComponent(), FName("SwingSocket"), FVector(DistanceInFrontOfPlayerToCastAt, 0.f, 0.f), Player->GetActorRotation(), EAttachLocation::KeepRelativeOffset, true);
	
	//Casting, check if should skip cast
	if(bSkipCast)
	{
		bSkipCast = false;
		//Shoot projectile
		FinishCast();
	}else
	{
		//Start freeze player timer
		StartFreezePlayer();
		//Wait for casting before shooting
		GetWorld()->GetTimerManager().SetTimer(CastHandle, this, &UPlayerRangedAttack::FinishCast, CastTime);
	}
}

void UPlayerRangedAttack::CancelAttack()
{
	Super::CancelAttack();

	//cooldown
	StartCooldown();

	//stop cast
	GetWorld()->GetTimerManager().ClearTimer(CastHandle);
}

void UPlayerRangedAttack::FinishCast()
{
	//finished casting, spawn projectile
	SpawnedProj = GetWorld()->SpawnActor<AProjectile>(Projectile, Player->GetActorLocation(), Player->GetActorRotation());
	//null check since its possible for the projectile to spawn inside an actor and immediately destroy itself
	if(SpawnedProj)
	{
		SpawnedProj->Damage = Damage;
		SpawnedProj->DamageMultiplier = DamageMultiplierToOppositeEmotion;
		SpawnedProj->bMyEmotionState = PlayerEmotionComponent->GetEmotionState(); //Record emotion state when the projectile is shot
		SpawnedProj->OnHitEnemy.AddDynamic(this, &UPlayerRangedAttack::EnemyDamaged);
	}

	OnAttack.Broadcast(false);
	
	//Cooldown
	StartCooldown();
}

void UPlayerRangedAttack::EnemyDamaged()
{
	DamageSelfEmotion();
	OnRangedAttackHit.Broadcast();
}

float UPlayerRangedAttack::GetCurrentCastTime()
{
	//Get elapsed time
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CastHandle);
	//Get elapsed time in range from 0 to 1
	if(ElapsedTime > 0.f && CastTime > 0.f)
	{
		ElapsedTime /= CastTime;
	}
	return ElapsedTime;
}