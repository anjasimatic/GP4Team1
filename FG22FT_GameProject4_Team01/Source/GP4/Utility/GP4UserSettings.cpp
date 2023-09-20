#include "GP4UserSettings.h"

void UGP4UserSettings::SaveSettings()
{
	Super::SaveSettings();
}

UGP4UserSettings* UGP4UserSettings::GetGP4Settings()
{
	UGP4UserSettings* SharedSettings = Cast<UGP4UserSettings>(GetGameUserSettings());

	return SharedSettings;
}

float UGP4UserSettings::GetAimSensitivity() const
{
	return AimSensitivity;
}

void UGP4UserSettings::SetAimSensitivity(const float NewValue)
{
	ChangeValue(AimSensitivity, FMath::Max(NewValue, 0));
}

bool UGP4UserSettings::GetInvertCameraY() const
{
	return bInvertCameraY;
}

void UGP4UserSettings::SetInvertsCameraY(const bool NewValue)
{
	ChangeValue(bInvertCameraY, NewValue);
}

float UGP4UserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

void UGP4UserSettings::SetMasterVolume(const float NewValue)
{
	ChangeValue(MasterVolume, NewValue);
}

float UGP4UserSettings::GetAmbientVolume() const
{
	return AmbientVolume;
}

void UGP4UserSettings::SetAmbientVolume(const float NewValue)
{
	ChangeValue(AmbientVolume, NewValue);
}

float UGP4UserSettings::GetEffectsVolume() const
{
	return EffectsVolume;
}

void UGP4UserSettings::SetEffectsVolume(const float NewValue)
{
	ChangeValue(EffectsVolume, NewValue);
}