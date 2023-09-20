#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraArmComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_API UCameraArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UCameraArmComponent();

public:
	/** Speed to use when zooming in from a collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraArm|Settings", meta=(ClampMin="0"))
	float CollisionZoomInSpeed;

	/** Speed to use when zooming out from a collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraArm|Settings", meta=(ClampMin="0"))
	float CollisionZoomOutSpeed;
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> VisibilityTargets;

	UPROPERTY(Transient)
	float CurrentCameraDistance;

public:
	FVector GetWorldCameraLocation(const FVector& BaseLocation) const;
	float GetCameraDistance() const;
	void SetImmediateLocation(const FVector& NewLocation);
	void SetDesiredLocation(const FVector& NewLocation);

	TArray<AActor*> GetVisibilityTargets();
	void SetVisibilityTargets(const TArray<AActor*>& NewTargets);
	void ClearVisibilityTargets();

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;
	float BlendCameraDistance(const float CurrentDist, const float TargetDist, float DeltaTime) const;
};