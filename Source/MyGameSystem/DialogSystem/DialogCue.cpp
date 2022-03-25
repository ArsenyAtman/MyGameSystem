// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogCue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Dialog.h"
#include "DialogComponent.h"
#include "TalkableInterface.h"
#include "Components/AudioComponent.h"
#include "Sound/DialogueWave.h"
#include "Sound/DialogueVoice.h"
#include "Sound/SoundCue.h"
#include "MyGameSystem/MontagePlayer/MontagePlayerComponent.h"
#include "MyGameSystem/MontagePlayer/MontagePlayableActorInterface.h"
#include "AnimNotify_PlayAudioOfDialogCue.h"

void UDialogCue::Activate_Implementation(class UDialog* OwnDialog)
{
	OwningDialog = OwnDialog;
	bIsAudioPlayed = false;
	bIsAnimationPlayed = false;
	OnCueBeginned();
	PlayAnimation();
	Sound->GetLocalizedSubtitle();
}

void UDialogCue::StartAudio()
{
	PlayAudio();
}

void UDialogCue::StartAnimation()
{
	PlayAnimation();
}

void UDialogCue::PlayNextDialogCue()
{
	if (IsValid(PlayingAudioComponent))
	{
		PlayingAudioComponent->OnAudioFinished.RemoveDynamic(this, &UDialogCue::AudioPlayed);
		PlayingAudioComponent = nullptr;
	}

	if (IsValid(PlayingAnimInstance))
	{
		PlayingAnimInstance->OnMontageEnded.RemoveDynamic(this, &UDialogCue::AnimationPlayed);
		PlayingAnimInstance = nullptr;
	}

	EndCue();
}

bool UDialogCue::CheckAvailabilityCondition_Implementation(UDialog* CheckingDialog)
{
	return true;
}

void UDialogCue::OnCueBeginned_Implementation()
{
	return;
}

void UDialogCue::OnCueEnded_Implementation()
{
	return;
}

void UDialogCue::PlayAudio_Implementation()
{
	if (IsValid(Sound))
	{
		UDialogComponent* DialogComponent = OwningDialog->GetOwningDialogComponent();//->GetMasterDialogComponent();
		if (IsValid(DialogComponent))
		{
			PlayingAudioComponent = DialogComponent->PlayDialogueWaveReplicated(Sound, Voice);
			if (IsValid(PlayingAudioComponent))
			{
				PlayingAudioComponent->OnAudioFinished.AddDynamic(this, &UDialogCue::AudioPlayed);
			}
			else
			{
				AudioPlayed();
			}
		}
	}
	else
	{
		AudioPlayed();
	}
}

void UDialogCue::AudioPlayed()
{
	if (IsValid(PlayingAudioComponent))
	{
		PlayingAudioComponent->OnAudioFinished.RemoveDynamic(this, &UDialogCue::AudioPlayed);
		PlayingAudioComponent = nullptr;
	}
	bIsAudioPlayed = true;
	CheckTransitionCondition();
}

void UDialogCue::PlayAnimation_Implementation()
{
	if (IsValid(Animation))
	{
		/*
		UDialogComponent* DialogComponent = OwningDialog->GetOwningDialogComponent();//->GetMasterDialogComponent();
		if (IsValid(DialogComponent))
		{
			PlayingAnimInstance = DialogComponent->PlayAnimationMontageReplicated(Animation, Voice);
			if (IsValid(PlayingAnimInstance))
			{
				PlayingAnimInstance->OnMontageEnded.AddDynamic(this, &UDialogCue::AnimationPlayed);
			}
			else
			{
				AnimationPlayed(nullptr, true);
			}
		}
		*/

		AActor* CueOwner = OwningDialog->GetOwnerOfVoice(this->Voice);
		if (IsValid(CueOwner) && CueOwner->Implements<UMontagePlayableActorInterface>() && Animation)
		{
			UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(CueOwner);
			PlayingAnimInstance = MontagePlayer->PlayAnimationMontageReplicated(Animation);
			if (IsValid(PlayingAnimInstance))
			{
				PlayingAnimInstance->OnMontageEnded.AddDynamic(this, &UDialogCue::AnimationPlayed);
			}
		}
	}

	if (!IsValid(PlayingAnimInstance))
	{
		this->AnimationPlayed(nullptr, true);
	}

	if (!HasAudioStartAnimNotify(Animation))
	{
		PlayAudio();
	}
}

void UDialogCue::AnimationPlayed(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(PlayingAnimInstance))
	{
		PlayingAnimInstance->OnMontageEnded.RemoveDynamic(this, &UDialogCue::AnimationPlayed);
		PlayingAnimInstance = nullptr;
	}
	bIsAnimationPlayed = true;
	CheckTransitionCondition();
}

void UDialogCue::CheckTransitionCondition()
{
	if (bIsAudioPlayed && bIsAnimationPlayed)
	{
		PlayNextDialogCue();
	}
}

void UDialogCue::EndCue()
{
	OnCueEnded();
	OwningDialog->OnDialogUnitPassed(this, NextDialogUnit);
}

bool UDialogCue::HasAudioStartAnimNotify(UAnimMontage* Montage)
{
	if (IsValid(Montage))
	{
		TArray<FAnimNotifyEventReference> Notifies;
		Montage->GetAnimNotifies(0, Montage->GetPlayLength(), false, Notifies);
		for (FAnimNotifyEventReference& Notify : Notifies)
		{
			if (Cast<UAnimNotify_PlayAudioOfDialogCue>(Notify.GetNotify()->Notify))
			{
				return true;
			}
		}
	}
	return false;
}

FDialogCueStruct UDialogCue::GetCueInfo()
{
	return FDialogCueStruct(Voice, Sound, DialogCueType);
}