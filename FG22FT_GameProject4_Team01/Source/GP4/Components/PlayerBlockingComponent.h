#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBlockingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockingDelegate, bool, Blocked);

UCLASS(ClassGroup=(Attacks), meta=(BlueprintSpawnableComponent))
class GP4_API UPlayerBlockingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPlayerBlockingComponent();


	UPROPERTY(BlueprintAssignable)
	FOnBlockingDelegate OnBlocking;
	
	
	UFUNCTION(BlueprintCallable)
	bool IsBlocking() const { return bBlocking; }
	
	UFUNCTION(BlueprintCallable)
	void SetBlocking(bool Blocking);

	UFUNCTION(BlueprintCallable)
	bool BlocksDamage(const FVector& AttackDirection);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0", UIMax="180"))
	float BlockingAngle = 90;
	
	
	virtual void BeginPlay() override;

private:

	bool bBlocking = false;

	UPROPERTY()
	AActor* Owner = nullptr;
	
};
