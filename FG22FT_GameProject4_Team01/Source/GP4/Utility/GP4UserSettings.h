#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "GP4UserSettings.generated.h"

//Inspired by implementation from Lyra example project
// https://github.com/EpicGames/UnrealEngine/blob/release/Samples/Games/Lyra/Source/LyraGame/Settings/LyraSettingsShared.h
UCLASS()
class GP4_API UGP4UserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UGP4UserSettings, FOnSettingChangedEvent, UGP4UserSettings* Settings);
	FOnSettingChangedEvent OnSettingChanged;

public:
	virtual void SaveSettings() override;
	UFUNCTION(BlueprintCallable)
	static UGP4UserSettings* GetGP4Settings();

public:
	UFUNCTION(BlueprintPure, Category="Settings", meta=(Keywords="Settings SaveGame Aim"))
	float GetAimSensitivity() const;
	UFUNCTION(BlueprintCallable, Category="Settings", meta=(Keywords="Settings SaveGame Aim"))
	void SetAimSensitivity(const float NewValue);

	UFUNCTION(BlueprintPure, Category="Settings", meta=(Keywords="Settings SaveGame Invert"))
	bool GetInvertCameraY() const;
	UFUNCTION(BlueprintCallable, Category="Settings", meta=(Keywords="Settings SaveGame Invert"))
	void SetInvertsCameraY(const bool NewValue);

	UFUNCTION(BlueprintPure, Category="Settings", meta=(Keywords="Settings SaveGame Volume"))
	float GetMasterVolume() const;
	UFUNCTION(BlueprintCallable, Category="Settings", meta=(Keywords="Settings SaveGame Volume"))
	void SetMasterVolume(const float NewValue);

	UFUNCTION(BlueprintPure, Category="Settings", meta=(Keywords="Settings SaveGame Volume"))
	float GetAmbientVolume() const;
	UFUNCTION(BlueprintCallable, Category="Settings", meta=(Keywords="Settings SaveGame Volume"))
	void SetAmbientVolume(const float NewValue);
	
	UFUNCTION(BlueprintPure, Category="Settings", meta=(Keywords="Settings SaveGame Volume"))
	float GetEffectsVolume() const;
	UFUNCTION(BlueprintCallable, Category="Settings", meta=(Keywords="Settings SaveGame Volume"))
	void SetEffectsVolume(const float NewValue);

private:
	UPROPERTY(Config)
	float AimSensitivity = 1.0f;
	UPROPERTY(Config)
	bool bInvertCameraY = false;
	UPROPERTY(Config)
	float MasterVolume = 1.0f;
	UPROPERTY(Config)
	float AmbientVolume = 1.0f;
	UPROPERTY(Config)
	float EffectsVolume = 1.0f;
		
private:
	template<typename T>
	bool ChangeValue(T& CurrentValue, const T& NewValue, const bool bUpdateConfig = true)
	{
		if (CurrentValue != NewValue)
		{
			CurrentValue = NewValue;
			if(bUpdateConfig)
				SaveConfig();
			
			OnSettingChanged.Broadcast(this);
			return true;
		}

		return false;
	}
};
