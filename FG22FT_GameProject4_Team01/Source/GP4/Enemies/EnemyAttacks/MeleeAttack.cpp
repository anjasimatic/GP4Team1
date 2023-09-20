#include "MeleeAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"

void UMeleeAttack::Attack()
{
	//Show vfx
	UNiagaraFunctionLibrary::SpawnSystemAttached(SwingNiagaraSystem, Attacker->GetDefaultAttachComponent(), FName("SwingSocket"), FVector(SwingDistanceInFrontOfEnemy, 0.f, 0.f), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	//Damage target if within range and FOV
	float Dist = FVector::Distance(Attacker->GetActorLocation(), Target->GetActorLocation());
	float Ang = FMath::RadiansToDegrees(acosf(FVector::DotProduct(Attacker->GetActorForwardVector(), ConstrainedDirFromAttackerToTarget())));
	if(Dist <= Range && Ang <= Angle)
	{
		//Attack
		DamageTarget();

		//Debug hitbox
		if(bShowDebug)
		{
			DrawDebugLine(GetWorld(), Attacker->GetActorLocation(), Attacker->GetActorLocation() + ConstrainedDirFromAttackerToTarget() * Range, FColor::Red, false, 1, 0, 10.f);
		}
	}
}
