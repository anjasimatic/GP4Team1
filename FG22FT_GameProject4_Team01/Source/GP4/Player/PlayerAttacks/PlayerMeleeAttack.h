#pragma once

#include "CoreMinimal.h"
#include "PlayerAttackBase.h"
#include "PlayerMeleeAttack.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable)
class GP4_API UPlayerMeleeAttack : public UPlayerAttackBase
{
	GENERATED_BODY()

public:
	virtual void AttackSetup() override;
	virtual void Attack() override;
	
	UPROPERTY(EditAnywhere)
	float SwingRange = 1.f;
	UPROPERTY(EditAnywhere)
	float SwingTime = 1.f;
	UPROPERTY(EditAnywhere, meta = (UIMin = "0", UIMax = "90"))
	float SwingRollAngle = 0.f;
	UPROPERTY(EditAnywhere, meta = (UIMin = "1", UIMax = "360"))
	float SwingTotalAngle = 180.f;
	UPROPERTY(EditAnywhere)
	bool bFlipAttack = false;
	UPROPERTY(EditAnywhere)
	float SwingStartTimeOffset = 0.f;
	UPROPERTY(EditAnywhere)
	float SwingDistanceInFrontOfPlayer = 100.f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> SwingNiagaraSystem;
	UPROPERTY(EditAnywhere)
	float PlayerForwardStepPower = 100.f;
	UPROPERTY(EditAnywhere)
	bool Staggers = false;
	UPROPERTY(EditAnywhere)
	float StaggerDuration = 1.f;
	UPROPERTY(EditAnywhere)
	float ShapeThickness = 35.f;

	float GetCurrentMeleeTime() {return CurrentStep / StepAmount;};
	
private:
	FTimerHandle AttackTimerHandle;
	void AttackTimer();

	int StepAmount = 20;
	int CurrentStep = 0;
	
	FVector CurrentLoc = FVector::Zero();
	float StepAngle = 0;
	float StepRollAngle = 0;
	
	FRotator CurrentRot = FRotator::ZeroRotator;
	
	FCollisionShape TraceShape;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitEnemies;
};