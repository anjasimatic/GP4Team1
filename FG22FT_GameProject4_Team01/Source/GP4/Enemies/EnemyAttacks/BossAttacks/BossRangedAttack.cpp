#include "BossRangedAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "GP4/Enemies/EnemyAttacks/EnemyRangedAttack.h"

void UBossRangedAttack::Attack()
{
	ExplosionLocs.Empty();
	
	for(int i = 0; i <= AmountOfExplosions; i++)
	{
		FVector Loc;

		//First explosion always gets placed at the player's location
		if(i == 0)
		{
			Loc = Target->GetActorLocation() - FVector(0.f,0.f,50.f);
		}else
		{
			//Get location to explode at, retry if the new explosion is too close to previous explosions
			float SmallestDist;
			do
			{
				//Get random location within range
				float DirX = FMath::RandRange(Target->GetActorLocation().X - ExplosionSpawnRangeAroundPlayer, Target->GetActorLocation().X + ExplosionSpawnRangeAroundPlayer);
				float DirY = FMath::RandRange(Target->GetActorLocation().Y - ExplosionSpawnRangeAroundPlayer, Target->GetActorLocation().Y + ExplosionSpawnRangeAroundPlayer);
				Loc = FVector(DirX, DirY, 0.f);

				//Get distance to closest previous explosion
				SmallestDist = 100000.f;
				for(int j = 0; j < ExplosionLocs.Num(); j++)
				{
					float Dist = FVector::Dist(ExplosionLocs[j], Loc);
					if(Dist < SmallestDist)
					{
						SmallestDist = Dist;	
					}
				}
			}
			while (SmallestDist < ExplosionSeparationAmount);
		}

		//Found a good location, add to array
		ExplosionLocs.Add(Loc);

		//spawn decal at explosion loc
		TObjectPtr<ADecalActor> Decal = GetWorld()->SpawnActor<ADecalActor>(Loc, FRotator::ZeroRotator);
		Decal->SetDecalMaterial(ExplosionDecal);
		Decal->GetDecal()->DecalSize = FVector(100.f, ExplosionRange, ExplosionRange);
		Decal->SetLifeSpan(TimeBeforeExplosion);
	}
	
	//Damage target if they're still within the range of the explosions
	GetWorld()->GetTimerManager().SetTimer(RangedAttackHandle, this, &UBossRangedAttack::DoRangedAttackTimer, TimeBeforeExplosion);
}

void UBossRangedAttack::DoRangedAttackTimer()
{
	for(int i = 0; i < ExplosionLocs.Num(); i++)
	{
		//Spawn explosion visual
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionNiagaraSystem, ExplosionLocs[i]);

		//Damage target if within range
		float Dist = FVector::Distance(Target->GetActorLocation(), ExplosionLocs[i]);
		if(Dist <= ExplosionRange)
		{
			DamageTarget();
		}
	}
}