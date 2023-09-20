#include "EmotionComponent.h"

// INITIALISATION

	UEmotionComponent::UEmotionComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

	void UEmotionComponent::BeginPlay(){
		Super::BeginPlay();
	}

// SETTERS

	void UEmotionComponent::InitEmotions(){
		float half = FMath::Lerp(
			MIN_EMOTION,
			MAX_EMOTION,
			0.5f
		);
		Hatred = half;
		Sorrow = half;

		BroadcastDelegates();
	}

	void UEmotionComponent::SetHatred(const float Hate){
		Hatred = FMath::Clamp(
			Hate,
			MIN_EMOTION,
			MAX_EMOTION
		);
		Sorrow = FMath::Clamp(
			MAX_EMOTION - Hate,
			MIN_EMOTION,
			MAX_EMOTION
		);
		BroadcastDelegates();
	}

	void UEmotionComponent::SetSorrow(const float Sadness){
		Sorrow = FMath::Clamp(
			Sadness,
			MIN_EMOTION,
			MAX_EMOTION
		);
		Hatred = FMath::Clamp(
			MAX_EMOTION - Sadness,
			MIN_EMOTION,
			MAX_EMOTION
		);
		BroadcastDelegates();
	}

// EMOTION ACTIONS

	void UEmotionComponent::SwitchState(const bool State){
		if (bBrokenDown || State == bHatredState) return;

		bHatredState = State;
		OnStateChanged.Broadcast(bHatredState);
	}

	void UEmotionComponent::AddHatred(const float Amount){
		if (bBrokenDown) return;
		
		// if (!bHatredState &&
		// 	Hatred + Amount > 1.0f)
		// 	bBrokenDown = true;
		
		Hatred = FMath::Clamp(
			Hatred + Amount,
			MIN_EMOTION,
			MAX_EMOTION
		);
		Sorrow = FMath::Clamp(
			Sorrow - Amount,
			MIN_EMOTION,
			MAX_EMOTION
		);
		BroadcastDelegates();
	}

	void UEmotionComponent::AddSorrow(const float Amount){
		if (bBrokenDown) return;
		
		// if (bHatredState &&
		// 	Sorrow + Amount > 1.0f)
		// 	bBrokenDown = true;

		Sorrow = FMath::Clamp(
			Sorrow + Amount,
			MIN_EMOTION,
			MAX_EMOTION
		);
		Hatred = FMath::Clamp(
			Hatred - Amount,
			MIN_EMOTION,
			MAX_EMOTION
		);
		BroadcastDelegates();
	}

	void UEmotionComponent::ResetState(){
		bHatredState = false;
		bBrokenDown  = false;
		
		Hatred = 0.5f;
		Sorrow = 0.5f;

		OnResetState.Broadcast();
		BroadcastDelegates();
	}

// EVENTS

	void UEmotionComponent::BroadcastDelegates() const {
		OnHatredChanged.Broadcast(Hatred);
		OnSorrowChanged.Broadcast(Sorrow);
		
		OnEmotionsChanged.Broadcast(
			GetEmotionRange()
		);
		if (bBrokenDown) OnBreakdown.Broadcast();
	}
