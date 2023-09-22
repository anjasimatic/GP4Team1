#include "EnemyAIController.h"
#include "BrainComponent.h"
#include "Grunt.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP4/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	//Start behavior tree
	if(!BehaviorTree) return;
	RunBehaviorTree(BehaviorTree);

	if(!GetPawn()) return;

	//Default values in blackboard
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsAggro"), false);
	GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint"), GetPawn()->GetActorLocation());
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsActive"), true);

	//Aggro enemy when damaged
	MyHealthComp = Cast<UHealthComponent>(GetPawn()->GetComponentByClass(UHealthComponent::StaticClass()));
	MyHealthComp->OnDamage.AddDynamic(this, &AEnemyAIController::OnDamaged);
}

void AEnemyAIController::BecomeAggro()
{
	GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsAggro"), true);
	OnAggro.Broadcast(GetPawn());
}

void AEnemyAIController::OnDamaged(int DamageTaken)
{
	BecomeAggro();
	//Only allow this to happen once
	MyHealthComp->OnDamage.RemoveDynamic(this, &AEnemyAIController::OnDamaged);
}

//Set the controllers rotation to the rotation of the pawn so perception senses rotate with pawn
FRotator AEnemyAIController::GetControlRotation() const
{
	if(!GetPawn()) return FRotator::ZeroRotator;
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AEnemyAIController::Stagger(float Duration)
{
	BrainComponent->PauseLogic(TEXT(""));
	GetWorld()->GetTimerManager().SetTimer(StaggerStunHandle, this, &AEnemyAIController::FinishStagger, Duration);
}

void AEnemyAIController::Stun(float Duration)
{
	BrainComponent->StopLogic(TEXT(""));
	GetWorld()->GetTimerManager().SetTimer(StaggerStunHandle, this, &AEnemyAIController::FinishStun, Duration);
}

void AEnemyAIController::FinishStagger()
{
	BrainComponent->ResumeLogic(TEXT(""));
}

void AEnemyAIController::FinishStun()
{
	BrainComponent->StartLogic();
}
