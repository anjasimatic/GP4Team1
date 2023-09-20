#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GP4Character.generated.h"

class UGP4CharacterCameraComponent;
class UGP4CharacterMovementComponent;

class UEmotionComponent;
class UHealthComponent;
class UPlayerAttackComponent;
class UPlayerAbilityComponent;
class UPlayerBlockingComponent;

UCLASS(config=Game, BlueprintType)
class GP4_API AGP4Character : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UGP4CharacterCameraComponent> GP4CharacterCamera;

public:
	AGP4Character(const FObjectInitializer& ObjectInitializer);	

	FORCEINLINE UGP4CharacterMovementComponent* GetGP4MovementComponent() const {return GP4CharacterMovement; }

	TObjectPtr<UGP4CharacterMovementComponent> GP4CharacterMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEmotionComponent> EmotionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerAttackComponent> AttackComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerAbilityComponent> AbilityComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerBlockingComponent> BlockingComponent;

	/** Blocks all input action except camera controls */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="Block Actions Lock Inputs"))
	void SetLockActions(bool bState);

	/** Blocks all input action except camera controls */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="Block Actions Lock Inputs"))
	bool IsLockActions() const;

	UFUNCTION(BlueprintCallable)
	void SetCombatMode(bool bState);

	UFUNCTION(BlueprintCallable)
	bool InCombatMode() const;

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bState);
	
	/** Tells the character to move */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetMove Move Input"))
	void MoveInput(const FVector2D Input, bool bForce = false);

	/** Tells the character to look */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetLook Look Input"))
	void LookInput(const FVector2D Input, bool bForce = false);
	
	/** Tells the character to StateSwitch */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetStateSwitch StateSwitch Input"))
	void StateSwitchInput(bool bState = true, bool bForce = false);

	/** Tells the character to Dodge */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetDodge Dodge Input"))
	void DodgeInput(bool bState = true, bool bForce = false);

	/** Tells the character to TargetMode */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetCombatMode CombatMode Input"))
	void CombatModeInput(bool bState = true, bool bForce = false);

	/** Tells the character to TargetMode */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetTargetSwitch TargetSwitch Input"))
	void TargetSwitchInput(bool bState = true, bool bForce = false);

	/** Tells the character to action */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetAction Action Input"))
	void ActionInput(bool bState = true, bool bForce = false);

	/** Tells the character to Block */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetBlock Block Input"))
	void BlockInput(bool bState = true, bool bForce = false);

	/** Tells the character to action */
	UFUNCTION(BlueprintCallable, Category="Character|Input", meta=(Keywords="SetAbility Ability Input"))
	void AbilityInput(bool bState = true, bool bForce = false);

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnMove Move Input"))
	void OnMoveInput(const FVector2D RawInput, const FVector WorldInput, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnLook Look Input"))
	void OnLookInput(const FVector2D Input, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnStateSwitch StateSwitch Input"))
	void OnStateSwitchInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnDodge Dodge Input"))
	void OnDodgeInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnCombatMode CombatMode Input"))
	void OnCombatModeInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnTargetSwitch TargetSwitch Input"))
	void OnTargetSwitchInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnAction Action Input"))
	void OnActionInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnBlock Block Input"))
	void OnBlockInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintNativeEvent, Category="Character|Input", meta=(Keywords="OnAbility Ability Input"))
	void OnAbilityInput(const bool bState, bool bForce = false);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLockActionsChange(bool bState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCombatModeChange(bool bState);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Input|Debug")
	bool bLockActions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Info")
	bool bInCombatMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Info")
	bool bIsAttacking;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FVector2D GetMovementInput() {return MovementInput;};
	FVector2D MovementInput = FVector2D::Zero();
};

FORCEINLINE bool AGP4Character::IsLockActions() const
{
	return bLockActions;
}

FORCEINLINE bool AGP4Character::InCombatMode() const
{
	return bInCombatMode;
}


