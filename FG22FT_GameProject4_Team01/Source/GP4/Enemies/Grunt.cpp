#include "Grunt.h"

AGrunt::AGrunt()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGrunt::BeginPlay()
{
	Super::BeginPlay();

	//Set my color depending on my emotion type
}