#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAggroDelegate, APawn*, Enemy);

class UHealthComponent;
class AGrunt;

UCLASS()
class GP4_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UFUNCTION(BlueprintCallable)
	void BecomeAggro();

	UPROPERTY()
	TObjectPtr<UHealthComponent> MyHealthComp;
	UFUNCTION()
	void OnDamaged(int DamageTaken);

	FOnAggroDelegate OnAggro;

	FTimerHandle StaggerStunHandle; 
	void Stagger(float Duration);
	void Stun(float Duration);

protected:
	virtual void BeginPlay() override;

	virtual FRotator GetControlRotation() const override;

	void FinishStagger();
	void FinishStun();
};
