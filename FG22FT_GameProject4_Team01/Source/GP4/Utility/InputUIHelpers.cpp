#include "InputUIHelpers.h"
#include "Components/Widget.h"

void UInputUIHelpers::Initialize(FSubsystemCollectionBase& Collection)
{
	InvalidateSelections();
	Super::Initialize(Collection);
}

void UInputUIHelpers::Deinitialize()
{
	InvalidateSelections();
	Super::Deinitialize();
}

void UInputUIHelpers::SelectNewWidget(UWidget* NewSelectedWidget, const bool bSaveLastSelection)
{
	if(bSaveLastSelection)
	{
		LastSelected = CurrentSelected;
	}
	CurrentSelected = NewSelectedWidget;
}

UWidget* UInputUIHelpers::GetLastSelection() const
{
	return LastSelected;
}

UWidget* UInputUIHelpers::GetCurrentSelection() const
{
	return CurrentSelected;
}

void UInputUIHelpers::SetIsKeyboardNavigation(const bool Enable)
{
	bIsKeyboardNavigation = Enable;
}

bool UInputUIHelpers::IsKeyboardNavigation() const
{
	return bIsKeyboardNavigation;
}

void UInputUIHelpers::InvalidateSelections()
{
	LastSelected = nullptr;
	CurrentSelected = nullptr;
	bIsKeyboardNavigation = false;
}
