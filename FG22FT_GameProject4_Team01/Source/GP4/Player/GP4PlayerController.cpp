#include "GP4PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "GP4Character.h"

void AGP4PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AGP4PlayerController::SetPawn(APawn* InPawn)
{
	TargetGP4Character = (InPawn ? Cast<AGP4Character>(InPawn) : NULL);
	
	Super::SetPawn(InPawn);
}
