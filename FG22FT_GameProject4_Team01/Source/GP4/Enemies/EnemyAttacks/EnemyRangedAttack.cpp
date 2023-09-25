#include "EnemyRangedAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"

void UEnemyRangedAttack::Attack()
{
	ExplosionLoc = Target->GetActorLocation();

	//spawn decal at target loc
	TObjectPtr<ADecalActor> Decal = GetWorld()->SpawnActor<ADecalActor>(ExplosionLoc, FRotator::ZeroRotator);
	Decal->SetDecalMaterial(ExplosionDecal);
	Decal->GetDecal()->DecalSize = FVector(100.f, ExplosionRange, ExplosionRange);
	Decal->SetLifeSpan(TimeBeforeExplosion);

	//Damage target if they're still within the range of the attack
	GetWorld()->GetTimerManager().SetTimer(RangedAttackHandle, this, &UEnemyRangedAttack::DoRangedAttackTimer, TimeBeforeExplosion);
}

void UEnemyRangedAttack::DoRangedAttackTimer()
{
	//Spawn explosion visual
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionNiagaraSystem, ExplosionLoc);

	//Damage target if within range
	float Dist = FVector::Distance(Target->GetActorLocation(), ExplosionLoc);
	if(Dist <= ExplosionRange)
	{
		DamageTarget();
	}
}

float UEnemyRangedAttack::GetCurrentCastTime()
{
	//Get elapsed time
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(RangedAttackHandle);
	//Get elapsed time in range from 0 to 1
	if(ElapsedTime > 0.f && TimeBeforeExplosion > 0.f)
	{
		ElapsedTime /= TimeBeforeExplosion;
	}
	return ElapsedTime;
}