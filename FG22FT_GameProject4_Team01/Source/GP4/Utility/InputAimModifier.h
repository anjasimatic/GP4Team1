#pragma once

#include "CoreMinimal.h"
#include "GP4UserSettings.h"
#include "InputModifiers.h"
#include "InputAimModifier.generated.h"

UCLASS()
class GP4_API UInputAimModifier : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override
	{
		const UGP4UserSettings* UserSettings = UGP4UserSettings::GetGP4Settings();

		if(!UserSettings) return CurrentValue;

		FVector NewValue = CurrentValue.Get<FVector>();

		NewValue *= UserSettings->GetAimSensitivity();
		NewValue.Y *= UserSettings->GetInvertCameraY() ? -1.f : 1.f;
	
		return NewValue;
	};
};
