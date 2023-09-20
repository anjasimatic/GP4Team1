#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmotionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmotionsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedDelegate, bool, InHatredState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHatredChangedDelegate, float, Hatred);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSorrowChangedDelegate, float, Sorrow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEmotionsChangedDelegate, float, Range);

UCLASS(ClassGroup=(Emotions), meta=(BlueprintSpawnableComponent))
class GP4_API UEmotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UEmotionComponent();


	UPROPERTY(BlueprintAssignable, meta=(ToolTip="Returns a value between 0 and 1"))
	FHatredChangedDelegate OnHatredChanged;

	UPROPERTY(BlueprintAssignable, meta=(ToolTip="Returns a value between 0 and 1"))
	FSorrowChangedDelegate OnSorrowChanged;
	
	UPROPERTY(BlueprintAssignable, meta=(ToolTip="Returns a value between Sorrow -1 and Hatred +1"))
	FEmotionsChangedDelegate OnEmotionsChanged;

	UPROPERTY(BlueprintAssignable, meta=(ToolTip="Returns true if in Hatred state else in Sorrow state"))
	FOnStateChangedDelegate OnStateChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnEmotionsDelegate OnBreakdown;

	UPROPERTY(BlueprintAssignable)
	FOnEmotionsDelegate OnResetState;

	
	UFUNCTION(BlueprintCallable, meta=(ToolTip="Returns true if in Hatred state else in Sorrow state"))
	bool GetEmotionState() const { return bHatredState; }
	
	UFUNCTION(BlueprintCallable, meta=(ToolTip="Returns a value between Sorrow -1 and Hatred +1"))
	float GetEmotionRange() const { return Hatred - Sorrow; }
	
	UFUNCTION(BlueprintCallable, meta=(ToolTip="Returns a value between 0 and 1"))
	float GetSadness() const { return Sorrow; }

	UFUNCTION(BlueprintCallable, meta=(ToolTip="Returns a value between 0 and 1"))
	float GetAnger() const { return Hatred; }


	UFUNCTION(BlueprintCallable)
	void InitEmotions();
	
	UFUNCTION(BlueprintCallable)
	void SetHatred(const float hatred);

	UFUNCTION(BlueprintCallable)
	void SetSorrow(const float sorrow);
	

	UFUNCTION(BlueprintCallable, meta=(ToolTip="True for Hatred, False for Sorrow"))
	void SwitchState(const bool State);
	
	UFUNCTION(BlueprintCallable)
	void AddHatred(const float Amount);

	UFUNCTION(BlueprintCallable)
	void AddSorrow(const float Amount);
	
	UFUNCTION(BlueprintCallable)
	void ResetState();

protected:
	
	virtual void BeginPlay() override;

private:

	bool bHatredState = false;
	bool bBrokenDown  = false;
	
	float Hatred = 0.5f;
	float Sorrow = 0.5f;
	const float MIN_EMOTION = 0;
	const float MAX_EMOTION = 1;


	void BroadcastDelegates() const;
	
};