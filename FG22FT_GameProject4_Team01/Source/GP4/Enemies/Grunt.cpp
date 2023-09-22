#include "Grunt.h"

#include "EnemyAIController.h"

AGrunt::AGrunt()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGrunt::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<AEnemyAIController>(GetController());
}

void AGrunt::Stun(float Duration)
{
	MyController->Stun(Duration);
}
