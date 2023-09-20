#include "EnemyBase.h"
#include "GP4/Components/EnemyAttackComponent.h"
#include "GP4/Components/HealthComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//Create components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AttackComponent = CreateDefaultSubobject<UEnemyAttackComponent>(TEXT("AttackComponent"));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	//Bind delegates
	HealthComponent->OnDeath.AddDynamic(this, &AEnemyBase::Die);
}

void AEnemyBase::Die()
{
	this->Destroy();
}
