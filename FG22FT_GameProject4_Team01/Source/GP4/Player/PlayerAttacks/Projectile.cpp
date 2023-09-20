#include "Projectile.h"
#include "GP4/Components/HealthComponent.h"
#include "GP4/Enemies/EnemyBase.h"
#include "GP4/Enemies/Grunt.h"
#include "GP4/Player/GP4Character.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move projectile
	FVector NewLoc = GetActorLocation() + GetActorForwardVector() * ProjectileSpeed * DeltaTime;
	SetActorLocation(NewLoc);
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Hit something
	Super::NotifyActorBeginOverlap(OtherActor);

	if(OtherActor->IsA(AProjectile::StaticClass())) return; // Don't hit other projectiles
	if(OtherActor->IsA(AGP4Character::StaticClass())) return; // Don't hit player

	//Hit actor is an enemy, damage them
	if(OtherActor->IsA(AEnemyBase::StaticClass()))
	{
		//Figure out how much damage to deal
		float DamageToDeal = Damage;
		if(TObjectPtr<AGrunt> Grunt = Cast<AGrunt>(OtherActor)) //Hit enemy is a grunt
		{
			//If player and enemy are of opposite states, deal more damage
			if(bMyEmotionState && Grunt->MyEmotion == EEmotionType::Sadness || !bMyEmotionState && Grunt->MyEmotion == EEmotionType::Anger)
			{
				DamageToDeal *= DamageMultiplier;
			}
		}
		
		//Damage enemy
		Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass()))->Damage(DamageToDeal);
		
		OnHitEnemy.Broadcast();
	}

	Destroy();
}