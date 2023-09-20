#include "EnemyAttackBase.h"
#include "GameFramework/Actor.h"
#include "GP4/Components/EmotionComponent.h"
#include "GP4/Components/HealthComponent.h"
#include "GP4/Enemies/Grunt.h"

void UEnemyAttackBase::AttackSetup(AActor* InAttacker, AActor* InTarget)
{
	Attacker = InAttacker;
	Target = InTarget;
	TargetEmotionComponent = Cast<UEmotionComponent>(Target->GetComponentByClass(UEmotionComponent::StaticClass()));
	TargetHealthComponent = Cast<UHealthComponent>(Target->GetComponentByClass(UHealthComponent::StaticClass()));
	AttackerGrunt = Cast<AGrunt>(InAttacker);
}

void UEnemyAttackBase::Attack()
{
	OnAttack(Attacker, Target);
}

void UEnemyAttackBase::DamageTarget()
{
	float DamageToDeal = EmotionDamage;
	float DamageToHealth = HealthDamage;
	
	//Deal more damage if enemy is opposite emotion state of target
	if(AttackerGrunt)
	{
		if((TargetEmotionComponent->GetEmotionState() && AttackerGrunt->MyEmotion == EEmotionType::Sadness) || (!TargetEmotionComponent->GetEmotionState() && AttackerGrunt->MyEmotion == EEmotionType::Anger)) //If target is in anger state and enemy is of type sadness, or target is in sadness state and enemy is of type hatred
		{
			DamageToDeal *= EmotionDamageMultiplier;
			DamageToHealth *= EmotionDamageMultiplier;
		}
	}

	//Increase targets emotion state
	if(TargetEmotionComponent->GetEmotionState())
	{
		TargetEmotionComponent->AddHatred(DamageToDeal);

	} else {
		TargetEmotionComponent->AddSorrow(DamageToDeal);
	}

	//Damage targets health
	TargetHealthComponent->DirectionalDamage(ConstrainedDirFromAttackerToTarget(), DamageToHealth);
}

FVector UEnemyAttackBase::ConstrainedDirFromAttackerToTarget()
{
	float X = Target->GetActorLocation().X - Attacker->GetActorLocation().X;
	float Y = Target->GetActorLocation().Y - Attacker->GetActorLocation().Y;
	return FVector(X, Y, 0.f).GetSafeNormal();
}
