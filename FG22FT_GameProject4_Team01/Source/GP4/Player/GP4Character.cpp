#include "GP4Character.h"

#include "GP4CharacterCameraComponent.h"
#include "GP4CharacterMovementComponent.h"
#include "GP4/Components/EmotionComponent.h"
#include "GP4/Components/HealthComponent.h"
#include "GP4/Components/PlayerAttackComponent.h"
#include "GP4/Components/PlayerAbilityComponent.h"
#include "GP4/Components/PlayerBlockingComponent.h"

AGP4Character::AGP4Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGP4CharacterMovementComponent>(CharacterMovementComponentName))
{
	GP4CharacterMovement = GetCharacterMovement<UGP4CharacterMovementComponent>();
	GP4CharacterCamera = CreateDefaultSubobject<UGP4CharacterCameraComponent>(TEXT("CameraComponent"));
	GP4CharacterCamera->SetupAttachment(RootComponent);

	EmotionComponent = CreateDefaultSubobject<UEmotionComponent>(TEXT("EmotionComponent"));
	HealthComponent  = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AttackComponent  = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));
	AbilityComponent = CreateDefaultSubobject<UPlayerAbilityComponent>(TEXT("AbilityComponent"));
	BlockingComponent = CreateDefaultSubobject<UPlayerBlockingComponent>(TEXT("BlockingComponent"));
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGP4Character::BeginPlay()
{
	bLockActions = false;
	bInCombatMode = false;
	bIsAttacking = false;
	Super::BeginPlay();
}

void AGP4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGP4Character::MoveInput(const FVector2D Input, const bool bForce /*=false*/)
{
	MovementInput = Input;
	
	if(bLockActions && !bForce) return;
	if(!Controller) return;
	
	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector WorldInput = (ForwardDirection*Input.Y + RightDirection*Input.X).GetSafeNormal();

	OnMoveInput(Input, WorldInput, bForce);
}

void AGP4Character::LookInput(const FVector2D Input, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;
	
	OnLookInput(Input, bForce);
}

void AGP4Character::StateSwitchInput(const bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;
	
	OnStateSwitchInput(bState, bForce);
}

void AGP4Character::DodgeInput(const bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;

	OnDodgeInput(bState, bForce);
}

void AGP4Character::CombatModeInput(const bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;

	OnCombatModeInput(bState, bForce);
}

void AGP4Character::TargetSwitchInput(bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;

	OnTargetSwitchInput(bState, bForce);
}

void AGP4Character::ActionInput(const bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;

	if(bState)
	{
		AttackComponent->Attack();	
	}

	OnActionInput(bState, bForce);
}

void AGP4Character::BlockInput(const bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;

	OnBlockInput(bState, bForce);
}

void AGP4Character::AbilityInput(const bool bState, const bool bForce /*=false*/)
{
	if(bLockActions && !bForce) return;
	if(!Controller) return;
	if(bIsAttacking) return;

	OnAbilityInput(bState, bForce);
}

void AGP4Character::OnMoveInput_Implementation(const FVector2D RawInput, const FVector WorldInput, bool bForce /*=false*/)
{
	if(bIsAttacking) return;
	AddMovementInput(WorldInput, RawInput.Length());
}

void AGP4Character::OnLookInput_Implementation(const FVector2D Input, bool bForce /*=false*/)
{
	if(!GP4CharacterCamera) return;

	GP4CharacterCamera->LookInput(Input, bForce);
}

void AGP4Character::OnStateSwitchInput_Implementation(const bool bState, bool bForce /*=false*/)
{
	if(bState) Jump();
	else StopJumping();
}

void AGP4Character::OnCombatModeInput_Implementation(const bool bState, bool bForce /*=false*/)
{
	if(!GP4CharacterCamera) return;

	if(bState)
	{
		if(GP4CharacterCamera->GetCurrentCameraMode() == EPlayerCameraMode::Combat) GP4CharacterCamera->SetCurrentCameraMode(EPlayerCameraMode::Default);
		else GP4CharacterCamera->SelectEnemyTarget();
	}
}

void AGP4Character::OnTargetSwitchInput_Implementation(const bool bState, bool bForce /*=false*/)
{
	if(!GP4CharacterCamera) return;

	if(GP4CharacterCamera->GetCurrentCameraMode() != EPlayerCameraMode::Combat) return;
	if(bState) GP4CharacterCamera->SelectEnemyTarget();
}

void AGP4Character::OnDodgeInput_Implementation(const bool bState, bool bForce /*=false*/)
{
}

void AGP4Character::OnActionInput_Implementation(const bool bState, bool bForce /*=false*/)
{
}

void AGP4Character::OnBlockInput_Implementation(const bool bState, bool bForce /*=false*/)
{
}

void AGP4Character::OnAbilityInput_Implementation(const bool bState, bool bForce /*=false*/)
{
}

void AGP4Character::SetLockActions(const bool bState)
{
	bLockActions = bState;
	OnLockActionsChange(bState);
}

void AGP4Character::SetCombatMode(const bool bState)
{
	bInCombatMode = bState;
	OnCombatModeChange(bState);
}

void AGP4Character::SetIsAttacking(bool bState)
{
	bIsAttacking = bState;
}