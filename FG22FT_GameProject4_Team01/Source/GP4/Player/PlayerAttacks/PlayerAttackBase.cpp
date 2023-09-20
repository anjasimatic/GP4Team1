#include "PlayerAttackBase.h"
#include "GP4/Components/EmotionComponent.h"
#include "GP4/Player/GP4Character.h"
#include "Kismet/GameplayStatics.h"

void UPlayerAttackBase::AttackSetup()
{
	Player = Cast<AGP4Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerEmotionComponent = Cast<UEmotionComponent>(Player->GetComponentByClass(UEmotionComponent::StaticClass()));
}

void UPlayerAttackBase::Attack()
{
	bCanceled = false;
}

void UPlayerAttackBase::CancelAttack()
{
	bCanceled = true;
	StartCooldown();
	EndFreezePlayer(); //Make sure to unfreeze player if they cancel the attack
}

void UPlayerAttackBase::StartCooldown()
{
	//Start cooldown if cooldown time is specified
	if(CooldownTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(CooldownHandle, this, &UPlayerAttackBase::EndCooldown, CooldownTime);
	}else
	{
		EndCooldown();
	}
}

void UPlayerAttackBase::EndCooldown()
{
	OnAttackFinished.Broadcast(bCanceled);
}

void UPlayerAttackBase::StartFreezePlayer()
{
	//Only freeze player if free time is specified
	if(FreezePlayerTime <= 0.f) return;
	
	Player->SetIsAttacking(true);
	GetWorld()->GetTimerManager().SetTimer(FreezePlayerHandle, this, &UPlayerAttackBase::EndFreezePlayer, FreezePlayerTime);
}

void UPlayerAttackBase::EndFreezePlayer()
{
	Player->SetIsAttacking(false);
}

void UPlayerAttackBase::DamageSelfEmotion()
{
	if(PlayerEmotionComponent->GetEmotionState())
	{
		float AllowedEmotionIncrease = (1 - PlayerEmotionComponent->GetAnger()) - 0.01f;
		if(EmotionIncreaseAmount > AllowedEmotionIncrease)
		{
			PlayerEmotionComponent->AddHatred(AllowedEmotionIncrease);
		}else
		{
			PlayerEmotionComponent->AddHatred(EmotionIncreaseAmount);
		}
	}else
	{
		float AllowedEmotionIncrease = (1 - PlayerEmotionComponent->GetSadness()) - 0.01f;
		if(EmotionIncreaseAmount > AllowedEmotionIncrease)
		{
			PlayerEmotionComponent->AddSorrow(AllowedEmotionIncrease);
		}else
		{
			PlayerEmotionComponent->AddSorrow(EmotionIncreaseAmount);
		}
	}
}
