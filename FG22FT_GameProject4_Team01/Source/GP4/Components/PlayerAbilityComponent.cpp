#include "PlayerAbilityComponent.h"

// INITIALISATION

	UPlayerAbilityComponent::UPlayerAbilityComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

// ABILITY ACTIONS

	