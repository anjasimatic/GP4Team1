#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP4CharacterCameraComponent.generated.h"

class UCameraArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class EPlayerCameraMode : uint8
{
	Default = 0,
	Combat = 1
};

UCLASS( ClassGroup=(Camera), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent), HideCategories=("Mobility", "Physics", "Collision", "Cooking", "Rendering", "LOD"))
class GP4_API UGP4CharacterCameraComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraArmComponent> CameraArmComponent;

protected:
	UPROPERTY()
	TObjectPtr<APawn> PawnOwner;
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CameraComponent|Debug")
	EPlayerCameraMode CurrentCameraMode;

	UPROPERTY(EditAnywhere, Category="CameraComponent|Debug")
	bool bDisplayDebug;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CurrentCombatTarget;

public:
	UGP4CharacterCameraComponent();	
	void SetupAttachment(USceneComponent* InParent, FName InSocketName = NAME_None) const;
	
	UFUNCTION(BlueprintCallable, Category="Components", meta=(Keywords = "Pawn Owning Parent"))
	APawn* GetPawnOwner() const;

	UFUNCTION(BlueprintCallable, Category="Components", meta=(Keywords = "Character Owning Parent"))
	ACharacter* GetCharacterOwner() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentCameraMode(EPlayerCameraMode NewState);
	
	UFUNCTION(BlueprintCallable)
	EPlayerCameraMode GetCurrentCameraMode() const;

	UFUNCTION(BlueprintCallable)
	void SetCombatTarget(AActor* InTarget);

	UFUNCTION(BlueprintCallable)
	AActor* GetCombatTarget() const;

public:
	/** Speed to transition between states */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings")
	float StateTransitionSpeed;
	
	/** Offset to the player forwards used as target rotation for AutoCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings")
	FRotator RestingRotationOffset;

	/** Default arm distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|AutoCamera", meta=(ClampMin=0, UIMin=0))
	float RestingCameraDistance;
	
	/** Multiplier for the AutoCameraInterpSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|AutoCamera", meta=(ClampMin=0, ClampMax = 1, UIMin=0, UIMax=1))
	float AutoCameraInfluence;

	/** Time until AutoCamera activates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|AutoCamera", meta=(ClampMin=0, UIMin=0))
	float AutoCameraSleepDuration;

	/** Yaw and Pitch InterpSpeed for AutoCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|AutoCamera")
	FVector2D AutoCameraInterpSpeed;

	/** Speed multiplier used by AutoCamera */
	UPROPERTY(BlueprintReadWrite)
	float AutoCameraSpeedMultiplier;

	/** Offset to the player forwards used as target rotation for CombatCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|CombatCamera")
	FRotator CombatRotationOffset;

	/** Combat arm distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|CombatCamera", meta=(ClampMin=0, UIMin=0))
	float CombatCameraDistance;

	/** Multiplier for the CombatCameraInterpSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|CombatCamera", meta=(ClampMin=0, ClampMax = 1, UIMin=0, UIMax=1))
	float CombatCameraRotInfluence;

	/** Location InterpSpeed for CombatCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|CombatCamera")
	float CombatCameraLocInterpSpeed;

	/** Yaw and Pitch InterpSpeed for CombatCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings|CombatCamera")
	FVector2D CombatCameraRotInterpSpeed;

protected:
	/** Extents used for search box overlap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraComponent|Settings")
	FVector LockOnSearchExtents;

protected:
	UPROPERTY(Transient)
	float AutoCameraSleepTimer;
	UPROPERTY(Transient)
	bool bStartNewCameraState;
	UPROPERTY(Transient)
	FVector TransitionLocation;
	
public:	
	/** Tells the camera to look */
	UFUNCTION(BlueprintCallable)
	void LookInput(const FVector2D Input, bool bForce = false);

	UFUNCTION(BlueprintCallable)
	void SelectEnemyTarget();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnLookInput(const FVector2D Input, bool bForce = false);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectTarget();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeCameraMode(const EPlayerCameraMode NewMode);


	void AutoCameraLoop(float DeltaTime);

	void CombatCameraLoop(float DeltaTime);
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;
};

FORCEINLINE APawn* UGP4CharacterCameraComponent::GetPawnOwner() const
{
	return PawnOwner;
}

FORCEINLINE ACharacter* UGP4CharacterCameraComponent::GetCharacterOwner() const
{
	return CharacterOwner;
}


FORCEINLINE EPlayerCameraMode UGP4CharacterCameraComponent::GetCurrentCameraMode() const
{
	return CurrentCameraMode;
}

FORCEINLINE AActor* UGP4CharacterCameraComponent::GetCombatTarget() const
{
	return CurrentCombatTarget;
}