#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

class UEmotionComponent;
class UPlayerAttackBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackDelegate, bool, IsMelee);

UENUM()
enum EEmotionComparisonType
{
	GreaterThan,
	LessThan,
	Equal
};

USTRUCT()
struct FPlayerAttackSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UPlayerAttackBase>> Attacks;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEmotionComparisonType> EmotionComparisonType = EEmotionComparisonType::Equal;
	UPROPERTY(EditAnywhere, meta = (UIMin = -1.f, UIMax = 1.f, ToolTip = "-1 is max sorrow and +1 is max hatred"))
	float EmotionThreshold = 0.f;
	UPROPERTY(EditAnywhere)
	float DamageMultiplier = 1.f;

	UPROPERTY()
	TArray<TObjectPtr<UPlayerAttackBase>> AttackInstances;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_API UPlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerAttackComponent();
	
	bool bIsBusy = false;

	//Attack sets
	UPROPERTY(EditAnywhere)
	TArray<FPlayerAttackSet> HatredAttackSets;
	UPROPERTY(EditAnywhere)
	TArray<FPlayerAttackSet> SorrowAttackSets;
	int CurrentAttackIndex = 0;

	//Attack 
	void Attack();
	UFUNCTION()
	void ReEvaluateAttackSet(float EmotionValue = 0.f);
	UFUNCTION()
	void ReEvaluateAttackSetState(bool State);
	UFUNCTION(BlueprintCallable)
	void CancelAttack();

	UPROPERTY(BlueprintAssignable)
	FOnAttackDelegate OnAttack;

	//Combo
	UPROPERTY(EditAnywhere)
	float TimeBeforeComboExpires = 2.f;
	
	FTimerHandle ExpireComboHandle;
	void ExpireCombo();

	//Other
	UFUNCTION(BlueprintCallable)
	float GetCurrentCastTime();

	UFUNCTION(BlueprintCallable)
	float GetCurrentMeleeTime();

	UPROPERTY(EditAnywhere, meta = (ToolTip = "This is a percentage of the total time of an attack. I.e: 0.2 will mean the next attack can be queued during the final 20% of the length of the current attack."))
	float InputQueueAllowedTimeBeforeAttackEnd = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0", UIMax = "1", ToolTip = "Whenever a ranged attack hits, the next ranged attack has a chance to skip cast time."))
	float CastSkipChance = 0.2f;
	UPROPERTY(EditAnywhere, meta = (UIMin = -1.f, UIMax = 1.f, ToolTip = "Above this value, skip may happen. -1 is max sorrow and +1 is max hatred"))
	float CastSkipEmotionThreshold = 0.0f;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void AttackFinished(bool bCanceled);

	UFUNCTION()
	void OnAttackEvent(bool IsMelee);

	TObjectPtr<UEmotionComponent> EmotionComponent;

	FPlayerAttackSet CurrentAttackSet;

	bool bEmotionState = false;

	FTimerHandle QueueInputHandle;
	void QueueInput();

	//Cast skipping
	UFUNCTION()
	void RollDiceForSkipCast();
public:
	UPROPERTY(BlueprintReadWrite)
	bool bSkipCast = false;
};
