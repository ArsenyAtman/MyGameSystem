// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogCue_AnimationAndSound.h"
#include "MyGameSystem/DialogWavePlayer/DialogWavePlayerComponent.h"
#include "MyGameSystem/DialogWavePlayer/DialogWavePlayableActorInterface.h"
#include "Components/AudioComponent.h"
#include "Sound/DialogueWave.h"
#include "Sound/DialogueVoice.h"
#include "Sound/SoundCue.h"
#include "MyGameSystem/MontagePlayer/MontagePlayerComponent.h"
#include "MyGameSystem/MontagePlayer/MontagePlayableActorInterface.h"
#include "AnimNotify_PlayAudioOfDialogCue.h"
#include "Dialog.h"

void UDialogCue_AnimationAndSound::OnCueBeginned_Implementation()
{
	Super::OnCueBeginned_Implementation();

	bIsAudioPlayed = false;
	bIsAnimationPlayed = false;
	PlayAnimation();
}

void UDialogCue_AnimationAndSound::OnCueEnded_Implementation()
{
	if (IsValid(PlayingAudioComponent))
	{
		PlayingAudioComponent->OnAudioFinished.RemoveDynamic(this, &UDialogCue_AnimationAndSound::AudioPlayed);
		PlayingAudioComponent = nullptr;
	}

	if (IsValid(PlayingAnimInstance))
	{
		PlayingAnimInstance->OnMontageEnded.RemoveDynamic(this, &UDialogCue_AnimationAndSound::AnimationPlayed);
		PlayingAnimInstance = nullptr;
	}

	Super::OnCueEnded_Implementation();
}

FText UDialogCue_AnimationAndSound::GetSpeakerName_Implementation()
{
	return FText::FromString(Voice->GetName());
}

FText UDialogCue_AnimationAndSound::GetCueText_Implementation()
{
	return FText::FromString(Sound->SpokenText);
}

void UDialogCue_AnimationAndSound::StartAudio()
{
	PlayAudio();
}

void UDialogCue_AnimationAndSound::StartAnimation()
{
	PlayAnimation();
}

void UDialogCue_AnimationAndSound::PlayAudio_Implementation()
{
	if (IsValid(Sound))
	{
		AActor* CueOwner = GetOwnerOfVoice(GetOwningDialog()->GetInterlocutors(), Voice);
		if (IsValid(CueOwner) && CueOwner->Implements<UDialogWavePlayableActorInterface>())
		{
			UDialogWavePlayerComponent* DialogPlayer = IDialogWavePlayableActorInterface::Execute_GetDialogWavePlayerComponent(CueOwner);
			if (IsValid(DialogPlayer))
			{
				TArray<UDialogueVoice*> InterlocutorsVoices = GetInterlocutorsVoices(GetOwningDialog()->GetInterlocutors());
				InterlocutorsVoices.Remove(Voice);
				PlayingAudioComponent = DialogPlayer->PlayDialogWaveReplicated(Sound, InterlocutorsVoices);
				if (IsValid(PlayingAudioComponent))
				{
					PlayingAudioComponent->OnAudioFinished.AddDynamic(this, &UDialogCue_AnimationAndSound::AudioPlayed);
				}
			}
		}
	}

	if (!IsValid(PlayingAudioComponent))
	{
		AudioPlayed();
	}
}

void UDialogCue_AnimationAndSound::AudioPlayed()
{
	if (IsValid(PlayingAudioComponent))
	{
		PlayingAudioComponent->OnAudioFinished.RemoveDynamic(this, &UDialogCue_AnimationAndSound::AudioPlayed);
		PlayingAudioComponent = nullptr;
	}
	bIsAudioPlayed = true;
	CheckTransitionCondition();
}

void UDialogCue_AnimationAndSound::PlayAnimation_Implementation()
{
	if (IsValid(Animation))
	{
		AActor* CueOwner = GetOwnerOfVoice(GetOwningDialog()->GetInterlocutors(), Voice);
		if (IsValid(CueOwner) && CueOwner->Implements<UMontagePlayableActorInterface>())
		{
			UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(CueOwner);
			PlayingAnimInstance = MontagePlayer->PlayAnimationMontageReplicated(Animation);
			if (IsValid(PlayingAnimInstance))
			{
				PlayingAnimInstance->OnMontageEnded.AddDynamic(this, &UDialogCue_AnimationAndSound::AnimationPlayed);
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

void UDialogCue_AnimationAndSound::AnimationPlayed(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(PlayingAnimInstance))
	{
		PlayingAnimInstance->OnMontageEnded.RemoveDynamic(this, &UDialogCue_AnimationAndSound::AnimationPlayed);
		PlayingAnimInstance = nullptr;
	}
	bIsAnimationPlayed = true;
	CheckTransitionCondition();
}

void UDialogCue_AnimationAndSound::CheckTransitionCondition()
{
	if (bIsAudioPlayed && bIsAnimationPlayed)
	{
		PlayNextDialogCue();
	}
}

TArray<UDialogueVoice*> UDialogCue_AnimationAndSound::GetInterlocutorsVoices(TArray<AActor*> Interlocutors)
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

AActor* UDialogCue_AnimationAndSound::GetOwnerOfVoice(TArray<AActor*> Interlocutors, UDialogueVoice* SpeakerVoice)
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

bool UDialogCue_AnimationAndSound::HasAudioStartAnimNotify(UAnimMontage* Montage)
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