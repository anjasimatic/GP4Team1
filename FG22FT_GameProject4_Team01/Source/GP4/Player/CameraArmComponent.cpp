#include "CameraArmComponent.h"

#include "PhysicsEngine/PhysicsSettings.h"

UCameraArmComponent::UCameraArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CollisionZoomInSpeed = 1000.f;
	CollisionZoomOutSpeed = 10.f;
}

void UCameraArmComponent::BeginPlay()
{
	Super::BeginPlay();
	ClearVisibilityTargets();
	CurrentCameraDistance = TargetArmLength;
}

void UCameraArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCameraArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
	float DeltaTime)
{
	FRotator DesiredRot = GetTargetRotation();

	// If our viewtarget is simulating using physics, we may need to clamp deltatime
	if (bClampToMaxPhysicsDeltaTime)
	{
		// Use the same max timestep cap as the physics system to avoid camera jitter when the viewtarget simulates less time than the camera
		DeltaTime = FMath::Min(DeltaTime, UPhysicsSettings::Get()->MaxPhysicsDeltaTime);
	}

	// Apply 'lag' to rotation if desired
	if(bDoRotationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraRotationLagSpeed > 0.f)
		{
			const FRotator ArmRotStep = (DesiredRot - PreviousDesiredRot).GetNormalized() * (1.f / DeltaTime);
			FRotator LerpTarget = PreviousDesiredRot;
			float RemainingTime = DeltaTime;
			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmRotStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(LerpTarget), LerpAmount, CameraRotationLagSpeed));
				PreviousDesiredRot = DesiredRot;
			}
		}
		else
		{
			DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(DesiredRot), DeltaTime, CameraRotationLagSpeed));
		}
	}
	PreviousDesiredRot = DesiredRot;

	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	// We lag the target, not the actual camera position, so rotating the camera around does not have lag
	FVector DesiredLoc = ArmOrigin;
	if (bDoLocationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
		{
			const FVector ArmMovementStep = (DesiredLoc - PreviousDesiredLoc) * (1.f / DeltaTime);
			FVector LerpTarget = PreviousDesiredLoc;

			float RemainingTime = DeltaTime;
			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmMovementStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, LerpTarget, LerpAmount, CameraLagSpeed);
				PreviousDesiredLoc = DesiredLoc;
			}
		}
		else
		{
			DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);
		}

		// Clamp distance if requested
		bool bClampedDist = false;
		if (CameraLagMaxDistance > 0.f)
		{
			const FVector FromOrigin = DesiredLoc - ArmOrigin;
			if (FromOrigin.SizeSquared() > FMath::Square(CameraLagMaxDistance))
			{
				DesiredLoc = ArmOrigin + FromOrigin.GetClampedToMaxSize(CameraLagMaxDistance);
				bClampedDist = true;
			}
		}		

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDrawDebugLagMarkers)
		{
			DrawDebugSphere(GetWorld(), ArmOrigin, 5.f, 8, FColor::Green);
			DrawDebugSphere(GetWorld(), DesiredLoc, 5.f, 8, FColor::Yellow);

			const FVector ToOrigin = ArmOrigin - DesiredLoc;
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc, DesiredLoc + ToOrigin * 0.5f, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc + ToOrigin * 0.5f, ArmOrigin,  7.5f, bClampedDist ? FColor::Red : FColor::Green);
		}
#endif
	}

	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc;
	if (bDoTrace && (TargetArmLength != 0.0f))
	{
		bIsCameraFixed = true;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());		

		FHitResult Result;
		FCollisionShape ProbeShape = FCollisionShape::MakeSphere(ProbeSize);
		
		FVector ArmDirection = (DesiredLoc - ArmOrigin).GetSafeNormal();
		
		UnfixedCameraPosition = DesiredLoc;

		if(!VisibilityTargets.IsEmpty())
		{
			FHitResult TargetResult;
			QueryParams.AddIgnoredActors(VisibilityTargets);			
			
			FVector BestLocation = DesiredLoc;
			float BestDistance = UE_BIG_NUMBER;

			for (auto Target : VisibilityTargets)
			{
				GetWorld()->SweepSingleByChannel(TargetResult, Target->GetActorLocation(), DesiredLoc, FQuat::Identity, ProbeChannel, ProbeShape, QueryParams);

				float ProjectionLength = FVector::DotProduct(ArmDirection, (TargetResult.Location - ArmOrigin));
				if(ProjectionLength < BestDistance)
				{
					Result = TargetResult;
					BestDistance = ProjectionLength;
					BestLocation = ArmOrigin + (ArmDirection*ProjectionLength);
				}
			}

			ResultLoc = BlendLocations(DesiredLoc, BestLocation, Result.bBlockingHit, DeltaTime);
		}
		else
		{
			GetWorld()->SweepSingleByChannel(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, ProbeShape, QueryParams);
			ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);
		}

		float TargetDistance = FVector::DotProduct(ArmDirection, (ResultLoc - ArmOrigin));
		float CurrentDistance = BlendCameraDistance(CurrentCameraDistance, TargetDistance, DeltaTime);
				
		ResultLoc = ArmOrigin + (ArmDirection*CurrentDistance);

		CurrentCameraDistance = CurrentDistance;

		if (ResultLoc == DesiredLoc) 
		{	
			bIsCameraFixed = false;
		}
	}
	else
	{
		ResultLoc = DesiredLoc;
		bIsCameraFixed = false;
		UnfixedCameraPosition = ResultLoc;
	}

	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredRot, ResultLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}

float UCameraArmComponent::BlendCameraDistance(const float CurrentDist, const float TargetDist, float DeltaTime) const
{
	//use different interp speed depending on travel direction 
	const float TravelSign = (FMath::Abs(TargetDist) - FMath::Abs(CurrentDist));

	if(TravelSign < 0.f)
	{
		return FMath::FInterpTo(CurrentDist, TargetDist, DeltaTime, CollisionZoomInSpeed);
	}
	else return FMath::FInterpTo(CurrentDist, TargetDist, DeltaTime, CollisionZoomOutSpeed);
}

FVector UCameraArmComponent::GetWorldCameraLocation(const FVector& BaseLocation) const
{
	const FRotator DesiredRot = GetTargetRotation();	
	FVector DesiredLoc = BaseLocation + TargetOffset;
	
	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);

	return DesiredLoc;
}

float UCameraArmComponent::GetCameraDistance() const
{
	return CurrentCameraDistance;
}

void UCameraArmComponent::SetImmediateLocation(const FVector& NewLocation)
{
	SetWorldLocation(NewLocation);
	PreviousDesiredLoc = NewLocation;
}

void UCameraArmComponent::SetDesiredLocation(const FVector& NewLocation)
{
	const FVector CurrentLocation = GetComponentLocation();
	SetWorldLocation(NewLocation);
	PreviousDesiredLoc = CurrentLocation;
}

void UCameraArmComponent::SetVisibilityTargets(const TArray<AActor*>& NewTargets)
{
	VisibilityTargets = NewTargets;
}

TArray<AActor*> UCameraArmComponent::GetVisibilityTargets()
{
	return VisibilityTargets;
}

void UCameraArmComponent::ClearVisibilityTargets()
{
	VisibilityTargets.Empty();
}
