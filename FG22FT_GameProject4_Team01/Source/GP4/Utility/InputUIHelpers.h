#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputUIHelpers.generated.h"

class UWidget;

UCLASS(DisplayName="InputUIHelpers")
class GP4_API UInputUIHelpers : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TObjectPtr<UWidget> LastSelected;
	UPROPERTY()
	TObjectPtr<UWidget> CurrentSelected;
	UPROPERTY()
	bool bIsKeyboardNavigation;

public:
	UFUNCTION(BlueprintCallable, Category="InputUIHelpers")
	void SelectNewWidget(UWidget* NewSelectedWidget, bool bSaveLastSelection);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="InputUIHelpers")
	UWidget* GetLastSelection() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="InputUIHelpers")
	UWidget* GetCurrentSelection() const;
	UFUNCTION(BlueprintCallable, Category="InputUIHelpers")
	void SetIsKeyboardNavigation(bool Enable);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="InputUIHelpers")
	bool IsKeyboardNavigation() const;
	UFUNCTION(BlueprintCallable, Category="InputUIHelpers")
	void InvalidateSelections();
};
