#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Grunt.generated.h"

class AEnemyAIController;
class APatrolRoute;

UENUM()
enum class EEmotionType
{
	Sadness,
	Anger
};

UCLASS()
class GP4_API AGrunt : public AEnemyBase
{
	GENERATED_BODY()

public:
	AGrunt();

	UPROPERTY(EditAnywhere)
	EEmotionType MyEmotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APatrolRoute> PatrolRoute;

	UFUNCTION(BlueprintCallable)
	void Stun(float Duration);

	UPROPERTY()
	TObjectPtr<AEnemyAIController> MyController;

protected:
	virtual void BeginPlay() override;
};
