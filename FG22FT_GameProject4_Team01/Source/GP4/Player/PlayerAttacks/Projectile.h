#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitEnemyDelegate);

UCLASS()
class GP4_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 1000.f;

	UPROPERTY(VisibleAnywhere)
	float Damage = 0.f;
	UPROPERTY(VisibleAnywhere)
	float DamageMultiplier = 1.f;

	bool bMyEmotionState = false;

	FOnHitEnemyDelegate OnHitEnemy;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	virtual void Tick(float DeltaTime) override;
};
