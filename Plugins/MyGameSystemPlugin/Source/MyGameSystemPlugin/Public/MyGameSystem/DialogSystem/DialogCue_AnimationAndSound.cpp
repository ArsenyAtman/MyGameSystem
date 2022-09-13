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
#include "Animation/AnimInstance.h"

void UDialogCue_AnimationAndSound::StartAudio()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		PlayAudio();
	}
}

void UDialogCue_AnimationAndSound::StartAnimation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		PlayAnimation();
	}
}

void UDialogCue_AnimationAndSound::Skip_Implementation()
{
	AActor* CueOwner = GetOwnerOfVoice(GetOwningDialog()->GetAllInterlocutors(), Voice);
	if (IsValid(CueOwner) && CueOwner->Implements<UDialogWavePlayableActorInterface>())
	{
		UDialogWavePlayerComponent* DialogPlayer = IDialogWavePlayableActorInterface::Execute_GetDialogWavePlayerComponent(CueOwner);
		if (IsValid(DialogPlayer))
		{
			DialogPlayer->StopDialogWaveReplicated();
		}

		UMontagePlayerComponent* MontagePlayer = IMontagePlayableActorInterface::Execute_GetMontagePlayerComponent(CueOwner);
		if (IsValid(MontagePlayer))
		{
			MontagePlayer->StopAnimationMontageReplicated(Animation);
		}

		Super::Skip_Implementation();
	}
}

void UDialogCue_AnimationAndSound::OnCueBeginned_Implementation()
{
	Super::OnCueBeginned_Implementation();

	if (HasAuthority())
	{
		bIsAudioPlayed = false;
		bIsAnimationPlayed = false;
		PlayAnimation();
	}
}

void UDialogCue_AnimationAndSound::OnCueEnded_Implementation()
{
	if (HasAuthority())
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
	}

	Super::OnCueEnded_Implementation();
}

void UDialogCue_AnimationAndSound::PlayAudio_Implementation()
{
	if (IsValid(Sound))
	{
		AActor* CueOwner = GetOwnerOfVoice(GetOwningDialog()->GetAllInterlocutors(), Voice);
		if (IsValid(CueOwner) && CueOwner->Implements<UDialogWavePlayableActorInterface>())
		{
			UDialogWavePlayerComponent* DialogPlayer = IDialogWavePlayableActorInterface::Execute_GetDialogWavePlayerComponent(CueOwner);
			if (IsValid(DialogPlayer))
			{
				TArray<UDialogueVoice*> InterlocutorsVoices = GetInterlocutorsVoices(GetOwningDialog()->GetAllInterlocutors());
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
		AActor* CueOwner = GetOwnerOfVoice(GetOwningDialog()->GetAllInterlocutors(), Voice);
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
	if ((bIsAudioPlayed || !IsValid(PlayingAudioComponent)) && (bIsAnimationPlayed || !IsValid(PlayingAnimInstance)))
	{
		PlayNextDialogCue();
	}
}

TArray<UDialogueVoice*> UDialogCue_AnimationAndSound::GetInterlocutorsVoices(const TArray<AActor*>& Interlocutors) const
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

AActor* UDialogCue_AnimationAndSound::GetOwnerOfVoice(const TArray<AActor*>& Interlocutors, const UDialogueVoice* SpeakerVoice) const
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

bool UDialogCue_AnimationAndSound::HasAudioStartAnimNotify(const UAnimMontage* Montage) const
{
	if (IsValid(Montage))
	{
		FAnimNotifyContext NotifyContext;
		Montage->GetAnimNotifies(0, Montage->GetPlayLength(), NotifyContext);
		for (const FAnimNotifyEventReference& Notify : NotifyContext.ActiveNotifies)
		{
			if (Cast<UAnimNotify_PlayAudioOfDialogCue>(Notify.GetNotify()->Notify))
			{
				return true;
			}
		}
	}
	return false;
}
