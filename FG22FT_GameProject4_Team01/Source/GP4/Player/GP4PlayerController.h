#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GP4PlayerController.generated.h"

class AGP4Character;

UCLASS()
class GP4_API AGP4PlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

private:
	UPROPERTY()
	TObjectPtr<AGP4Character> TargetGP4Character;

public:
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, Category=Pawn)
	AGP4Character* GetGP4Character() const;	
};

FORCEINLINE AGP4Character* AGP4PlayerController::GetGP4Character() const
{
	return TargetGP4Character;
}

