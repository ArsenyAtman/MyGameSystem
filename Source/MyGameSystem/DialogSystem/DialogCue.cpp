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
#include "MyGameSystem/DialogWavePlayer/DialogWavePlayerComponent.h"
#include "MyGameSystem/DialogWavePlayer/DialogWavePlayableActorInterface.h"

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
		AActor* CueOwner = GetOwnerOfVoice(OwningDialog->GetInterlocutors(), Voice);
		if (IsValid(CueOwner) && CueOwner->Implements<UDialogWavePlayableActorInterface>())
		{
			UDialogWavePlayerComponent* DialogPlayer = IDialogWavePlayableActorInterface::Execute_GetDialogWavePlayerComponent(CueOwner);
			if (IsValid(DialogPlayer))
			{
				TArray<UDialogueVoice*> Interlocutors = GetInterlocutorsVoices(OwningDialog->GetInterlocutors());
				Interlocutors.Remove(Voice);
				PlayingAudioComponent = DialogPlayer->PlayDialogWaveReplicated(Sound, Interlocutors);
				if (IsValid(PlayingAudioComponent))
				{
					PlayingAudioComponent->OnAudioFinished.AddDynamic(this, &UDialogCue::AudioPlayed);
				}
			}
		}
	}

	if (!IsValid(PlayingAudioComponent))
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
		AActor* CueOwner = GetOwnerOfVoice(OwningDialog->GetInterlocutors(), Voice);
		if (IsValid(CueOwner) && CueOwner->Implements<UMontagePlayableActorInterface>())
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

TArray<UDialogueVoice*> UDialogCue::GetInterlocutorsVoices(TArray<AActor*> Interlocutors)
{
	TArray<UDialogueVoice*> Voices;

	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UDialogWavePlayableActorInterface>())
		{
			UDialogWavePlayerComponent* DialogPlayer = IDialogWavePlayableActorInterface::Execute_GetDialogWavePlayerComponent(Interlocutor);
			if (IsValid(DialogPlayer) && IsValid(DialogPlayer->GetSpeakerVoice()))
			{
				Voices.Add(DialogPlayer->GetSpeakerVoice());
			}
		}
	}

	return Voices;
}

AActor* UDialogCue::GetOwnerOfVoice(TArray<AActor*> Interlocutors, UDialogueVoice* SpeakerVoice)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UDialogWavePlayableActorInterface>())
		{
			UDialogWavePlayerComponent* DialogPlayer = IDialogWavePlayableActorInterface::Execute_GetDialogWavePlayerComponent(Interlocutor);
			if (IsValid(DialogPlayer) && DialogPlayer->GetSpeakerVoice() == SpeakerVoice)
			{
				return Interlocutor;
			}
		}
	}

	return nullptr;
}

FDialogCueStruct UDialogCue::GetCueInfo()
{
	return FDialogCueStruct(Voice, Sound, DialogCueType);
}