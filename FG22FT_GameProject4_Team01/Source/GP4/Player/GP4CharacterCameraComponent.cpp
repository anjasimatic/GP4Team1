#include "GP4CharacterCameraComponent.h"

#include "CameraArmComponent.h"
#include "MathUtil.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4/Enemies/EnemyBase.h"
#include "GP4/Utility/CustomMath.h"
#include "Kismet/KismetSystemLibrary.h"

UGP4CharacterCameraComponent::UGP4CharacterCameraComponent()
{
	CameraArmComponent = CreateDefaultSubobject<UCameraArmComponent>(TEXT("CameraArmComponent"));
	CameraArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraArmComponent, UCameraArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bDisplayDebug = false;
	LockOnSearchExtents = FVector(3000.f, 2000.f, 1000.f);

	StateTransitionSpeed = 3.f;
	RestingRotationOffset = FRotator(-15.f, 0, 0);
	RestingCameraDistance = 350.0f;
	AutoCameraInfluence = 1.f;
	AutoCameraSleepDuration = 2.f;
	AutoCameraInterpSpeed = FVector2D(1.f, 0.8f);
	AutoCameraSpeedMultiplier = 1.f;	

	CombatRotationOffset = FRotator(-5.f, 0, 0);
	CombatCameraDistance = 250.f;
	CombatCameraRotInfluence = 1.f;
	CombatCameraLocInterpSpeed = 8.f;
	CombatCameraRotInterpSpeed = FVector2D(1.25f, 1.f);
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UGP4CharacterCameraComponent::SetupAttachment(USceneComponent* InParent, FName InSocketName) const
{
	CameraArmComponent->SetupAttachment(InParent, InSocketName);
	CameraComponent->SetupAttachment(CameraArmComponent, USpringArmComponent::SocketName);
}

void UGP4CharacterCameraComponent::BeginPlay()
{
	AutoCameraSpeedMultiplier = 1.f;
	SetCurrentCameraMode(EPlayerCameraMode::Default);
	Super::BeginPlay();
}

void UGP4CharacterCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AutoCameraSleepTimer += DeltaTime;

	switch (CurrentCameraMode)
	{
		case EPlayerCameraMode::Default:
			 AutoCameraLoop(DeltaTime);
		break;
		case EPlayerCameraMode::Combat:
			CombatCameraLoop(DeltaTime);
		break;
	}
}

void UGP4CharacterCameraComponent::AutoCameraLoop(float DeltaTime)
{
	if(!PawnOwner || !PawnOwner->IsPawnControlled()) return;

	if(bStartNewCameraState)
	{
		const FVector TargetLocation = PawnOwner->GetActorLocation();
		TransitionLocation = FCustomMath::VInterpTo(TransitionLocation, TargetLocation, DeltaTime, StateTransitionSpeed);
		CameraArmComponent->SetImmediateLocation(TransitionLocation);

		if(FVector::Dist(TransitionLocation, TargetLocation) < 5.f)
		{
			bStartNewCameraState = false;
			CameraArmComponent->SetDesiredLocation(TargetLocation);
		}
	}

	const float CurrentArmLength = FMath::FInterpTo(CameraArmComponent->TargetArmLength, RestingCameraDistance, DeltaTime, 2.f);
	CameraArmComponent->TargetArmLength = CurrentArmLength;

	if(AutoCameraSleepTimer < AutoCameraSleepDuration) return;
	const float Scale = AutoCameraSpeedMultiplier*AutoCameraInfluence;
	if(Scale <= UE_SMALL_NUMBER) return;

	FRotator CurrentRotation = PawnOwner->GetControlRotation();
	FRotator TargetRotation = GetOwner()->GetActorRotation() + RestingRotationOffset;
	// added offset added to prevent interp locking at 180 degree distance
	if(TargetRotation.GetManhattanDistance(CurrentRotation) >= 90.f) TargetRotation += FRotator(0,.01f, 0);
	TargetRotation.Normalize();

	CurrentRotation.Pitch = FCustomMath::FInterpAngleTo(CurrentRotation.Pitch, TargetRotation.Pitch, DeltaTime, AutoCameraInterpSpeed.Y*Scale);
	CurrentRotation.Yaw = FCustomMath::FInterpAngleTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, AutoCameraInterpSpeed.X*Scale);	
			
	PawnOwner->GetController()->SetControlRotation(CurrentRotation);
}

void UGP4CharacterCameraComponent::CombatCameraLoop(float DeltaTime)
{
	if(!PawnOwner || !PawnOwner->IsPawnControlled()) return;

	if(!IsValid(CurrentCombatTarget))
	{
		SelectEnemyTarget();
		if(!IsValid(CurrentCombatTarget))
		{
			SetCurrentCameraMode(EPlayerCameraMode::Default);
			return;
		}			
	}

	const FVector PawnLocation = PawnOwner->GetActorLocation();
	const FVector CombatTargetLocation = CurrentCombatTarget->GetActorLocation();

	float CollisionZoomAmount = CameraArmComponent->GetCameraDistance() / CameraArmComponent->TargetArmLength;

	FVector CurrentLocation = CameraArmComponent->GetComponentLocation();
	const FVector TargetLocation = FMath::Lerp(PawnLocation, CombatTargetLocation, FMath::Clamp(CollisionZoomAmount *.5f, 0, 1.f));

	CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, CombatCameraLocInterpSpeed);
	
	if(bStartNewCameraState)
	{
		TransitionLocation = FCustomMath::VInterpTo(TransitionLocation, TargetLocation, DeltaTime, StateTransitionSpeed);
		CameraArmComponent->SetImmediateLocation(TransitionLocation);

		CurrentLocation = TransitionLocation;

		if(FVector::Dist(TransitionLocation, TargetLocation) < 5.f)
		{
			bStartNewCameraState = false;
			CameraArmComponent->SetDesiredLocation(TargetLocation);
		}
	}
	
	const float LocDistance = (PawnLocation - CurrentLocation).Length();
	const float CurrentArmLength = FMath::FInterpTo(CameraArmComponent->TargetArmLength, CombatCameraDistance + LocDistance, DeltaTime, 5.f);
	CameraArmComponent->TargetArmLength = CurrentArmLength;

	CameraArmComponent->SetDesiredLocation(CurrentLocation);

	if(CharacterOwner)
	{
		const FRotator CharacterRotRate = CharacterOwner->GetCharacterMovement()->RotationRate;
		FRotator CurrentCharacterRot = CharacterOwner->GetActorRotation();
		const float TargetCharacterYaw = (CombatTargetLocation - PawnLocation).Rotation().Yaw;

		CurrentCharacterRot.Yaw = FCustomMath::FInterpAngleConstantTo(CurrentCharacterRot.Yaw, TargetCharacterYaw, DeltaTime, CharacterRotRate.Yaw);

		CharacterOwner->SetActorRotation(CurrentCharacterRot);
	}

	//if(AutoCameraSleepTimer < AutoCameraSleepDuration) return;

	const float RotScale = CombatCameraRotInfluence;
	if(RotScale <= UE_SMALL_NUMBER) return;

	FRotator CurrentRotation = PawnOwner->GetControlRotation();
	FRotator TargetRotation = (CombatTargetLocation - CameraArmComponent->GetWorldCameraLocation(TargetLocation)).Rotation();
	TargetRotation += CombatRotationOffset;
	if(TargetRotation.GetManhattanDistance(CurrentRotation) >= 90.f) TargetRotation += FRotator(0,.01f, 0);
	TargetRotation.Normalize();

	CurrentRotation.Pitch = FCustomMath::FInterpAngleTo(CurrentRotation.Pitch, TargetRotation.Pitch, DeltaTime, CombatCameraRotInterpSpeed.Y*RotScale);
	CurrentRotation.Yaw = FCustomMath::FInterpAngleTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, CombatCameraRotInterpSpeed.X*RotScale);	

	PawnOwner->GetController()->SetControlRotation(CurrentRotation);
}

void UGP4CharacterCameraComponent::LookInput(const FVector2D Input, bool bForce /*=false*/)
{
	if(!PawnOwner || !PawnOwner->IsPawnControlled()) return;

	OnLookInput(Input, bForce);
}

void UGP4CharacterCameraComponent::OnLookInput_Implementation(const FVector2D Input, bool bForce /*=false*/)
{
	AutoCameraSleepTimer = 0;
	PawnOwner->AddControllerPitchInput(-Input.Y);
	PawnOwner->AddControllerYawInput(Input.X);
}

void UGP4CharacterCameraComponent::SelectEnemyTarget()
{
	if(!PawnOwner && !PawnOwner->IsPawnControlled()) return;

	// find out which way is forward
	const FRotator Rotation = PawnOwner->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector PawnLoc = PawnOwner->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(PawnOwner, 1);
	
	TArray<AActor*> Enemies;

	const FVector BoxExtents = LockOnSearchExtents;
	const float BoxOffset = BoxExtents.X*.5f;

	UKismetSystemLibrary::BoxOverlapActors(GetWorld(), PawnLoc + ForwardDirection *  BoxOffset, BoxExtents,
		TraceObjectTypes, AEnemyBase::StaticClass(), IgnoreActors, Enemies);

	if(bDisplayDebug)
		DrawDebugBox(GetWorld(), PawnLoc + ForwardDirection * BoxOffset, BoxExtents, FQuat::Identity, FColor::Red, false, 1, 0, 10.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PawnOwner);
	QueryParams.AddIgnoredActors(Enemies);
	
	if(!IsValid(CurrentCombatTarget))
	{
		float Closest = 10000.f;
		AActor* PotentialTarget = nullptr;
		for (AActor* Enemy : Enemies)
		{
			const float Dist = FVector::Distance(PawnLoc, Enemy->GetActorLocation());
			if(Dist < Closest)
			{
				if(GetWorld()->LineTraceTestByChannel(PawnLoc, Enemy->GetActorLocation(), ECC_Camera, QueryParams)) continue;
				Closest = Dist;
				PotentialTarget = Enemy;
			}
		}

		if(!PotentialTarget)
			return;

		SetCombatTarget(PotentialTarget);
	}
	else
	{
		const float CurrentDot = FVector::DotProduct((CurrentCombatTarget->GetActorLocation() - PawnLoc).GetSafeNormal(), RightDirection);
		
		if(bDisplayDebug)
			UE_LOG(LogTemp, Warning, TEXT("Current: %f"), CurrentDot);
		
		float Closest = 2.f;
		AActor* PotentialTarget = nullptr;

		float Smallest = 2.f;
		AActor* FirstTarget = nullptr;
		for (AActor* Enemy : Enemies)
		{
			const float Dot = FVector::DotProduct((Enemy->GetActorLocation() - PawnLoc).GetSafeNormal(), RightDirection);

			if(bDisplayDebug)
				UE_LOG(LogTemp, Warning, TEXT("Next: %f"), Dot);

			if(Enemy == CurrentCombatTarget) continue;
						
			if(CurrentDot < Dot && Dot < Closest)
			{
				if(GetWorld()->LineTraceTestByChannel(PawnLoc, Enemy->GetActorLocation(), ECC_Camera, QueryParams)) continue;
				Closest = Dot;
				PotentialTarget = Enemy;
			}
			else if(Dot < Smallest)
			{
				if(GetWorld()->LineTraceTestByChannel(PawnLoc, Enemy->GetActorLocation(), ECC_Camera, QueryParams)) continue;
				Smallest = Dot;
				FirstTarget = Enemy;
			}
		}

		if(!PotentialTarget && !FirstTarget)
			return;
		
		SetCombatTarget(PotentialTarget ? PotentialTarget : FirstTarget);
	}
	
	if(CurrentCameraMode != EPlayerCameraMode::Combat) SetCurrentCameraMode(EPlayerCameraMode::Combat);
	OnSelectTarget();
}

void UGP4CharacterCameraComponent::SetCurrentCameraMode(const EPlayerCameraMode NewState)
{
	CurrentCameraMode = NewState;

	if(NewState != EPlayerCameraMode::Combat)
		CurrentCombatTarget = nullptr;

	bStartNewCameraState = true;
	TransitionLocation = CameraArmComponent->GetComponentLocation();

	switch (NewState)
	{
		case EPlayerCameraMode::Default:
			CameraArmComponent->SetUsingAbsoluteLocation(false);
			CameraArmComponent->ClearVisibilityTargets();
			if(CharacterOwner)
			{
				CharacterOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
			}
		break;
		case EPlayerCameraMode::Combat:
			CameraArmComponent->SetUsingAbsoluteLocation(true);
			TArray<AActor*> CameraTargets;
			CameraTargets.Add(PawnOwner);
			CameraArmComponent->SetVisibilityTargets(CameraTargets);

			if(CharacterOwner)
			{
				CharacterOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
			}
		break;
	}

	OnChangeCameraMode(NewState);
}

void UGP4CharacterCameraComponent::SetCombatTarget(AActor* InTarget)
{	
	CurrentCombatTarget = InTarget;
}

void UGP4CharacterCameraComponent::OnRegister()
{
	Super::OnRegister();

	PawnOwner = Cast<APawn>(GetOwner());
	CharacterOwner = Cast<ACharacter>(GetOwner());
}