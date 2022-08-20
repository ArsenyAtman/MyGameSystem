// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorAction_MontageDriven.h"
#include "MyGameSystem/MontagePlayer/MontagePlayerComponent.h"
#include "MyGameSystem/MontagePlayer/MontagePlayableActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UActorAction_MontageDriven::OnActionStarted_Implementation()
{
	if (HasAuthority())
	{
		// If the controlled actor is valid and has a montage player...
		AActor* ControlledActor = GetControlledActor();
		if (IsValid(ControlledActor) && ControlledActor->Implements<UMontagePlayableActorInterface>())
		{
			UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(ControlledActor);
			if (IsValid(MontagePlayer))
			{
				// And if the animation montage to play is valid...
				PlayingAnimMontage = GetAnimationMontageToPlay();
				if (IsValid(PlayingAnimMontage))
				{
					// than play this animation on the montage player
					PlayingAnimInstance = MontagePlayer->PlayAnimationMontageReplicated(PlayingAnimMontage);
					// and bind the AnimationPlayed function.
					PlayingAnimInstance->OnMontageEnded.AddDynamic(this, &UActorAction_MontageDriven::AnimationPlayed);

					return;
				}
			}
		}

		// Else end this action.
		EndAction();
	}
}

void UActorAction_MontageDriven::OnActionEnded_Implementation()
{
	if (HasAuthority())
	{
		// If the anim instance is valid...
		if (IsValid(PlayingAnimInstance))
		{
			// than unbind the AnimationPlayed function.
			PlayingAnimInstance->OnMontageEnded.RemoveDynamic(this, &UActorAction_MontageDriven::AnimationPlayed);

			// If the playing montage and montage player are valid...
			if (IsValid(PlayingAnimMontage))
			{
				AActor* ControlledActor = GetControlledActor();
				if (IsValid(ControlledActor) && ControlledActor->Implements<UMontagePlayableActorInterface>())
				{
					UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(ControlledActor);
					if (IsValid(MontagePlayer))
					{
						// than stop the animation.
						MontagePlayer->StopAnimationMontage(PlayingAnimMontage);
					}
				}
			}
		}

		// Clean up the pointers.
		PlayingAnimMontage = nullptr;
		PlayingAnimInstance = nullptr;
	}
}

void UActorAction_MontageDriven::AnimationPlayed(UAnimMontage* AnimMontage, bool bInterrupted)
{
	// If the ended animation is the animation of this action and it wasn't interrupted (played to the end)...
	if (AnimMontage == PlayingAnimMontage && !bInterrupted)
	{
		// than end this action.
		EndAction();
	}
}