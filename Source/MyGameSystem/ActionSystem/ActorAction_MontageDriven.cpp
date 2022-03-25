// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorAction_MontageDriven.h"
#include "MyGameSystem/MontagePlayer/MontagePlayerComponent.h"
#include "MyGameSystem/MontagePlayer/MontagePlayableActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UActorAction_MontageDriven::OnActionStarted_Implementation()
{
	AActor* ControlledActor = GetControlledActor();
	if (IsValid(ControlledActor) && ControlledActor->Implements<UMontagePlayableActorInterface>())
	{
		UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(ControlledActor);
		if (IsValid(MontagePlayer))
		{
			PlayingAnimMontage = GetAnimationMontageToPlay();
			if (IsValid(PlayingAnimMontage))
			{
				PlayingAnimInstance = MontagePlayer->PlayAnimationMontageReplicated(PlayingAnimMontage);
				PlayingAnimInstance->OnMontageEnded.AddDynamic(this, &UActorAction_MontageDriven::AnimationPlayed);

				return;
			}
		}
	}

	EndAction();
}

void UActorAction_MontageDriven::OnActionEnded_Implementation()
{
	if (IsValid(PlayingAnimInstance))
	{
		PlayingAnimInstance->OnMontageEnded.RemoveDynamic(this, &UActorAction_MontageDriven::AnimationPlayed);
		if (IsValid(PlayingAnimMontage))
		{
			AActor* ControlledActor = GetControlledActor();
			if (IsValid(ControlledActor) && ControlledActor->Implements<UMontagePlayableActorInterface>())
			{
				UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(ControlledActor);
				if (IsValid(MontagePlayer))
				{
					MontagePlayer->StopAnimationMontage(PlayingAnimMontage);
				}
			}
		}
	}

	PlayingAnimMontage = nullptr;
	PlayingAnimInstance = nullptr;
}

void UActorAction_MontageDriven::AnimationPlayed(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (AnimMontage == PlayingAnimMontage && !bInterrupted)
	{
		EndAction();
	}
}