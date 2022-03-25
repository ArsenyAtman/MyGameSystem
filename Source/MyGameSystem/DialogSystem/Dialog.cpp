// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialog.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogUnitInterface.h"
#include "Sound/DialogueWave.h"
#include "Sound/DialogueVoice.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialog::Begin(UDialogComponent* OwnDialogComponent, TArray<class AActor*> DialogInterlocutors)
{
	OwningDialogComponent = OwnDialogComponent;
	Interlocutors = DialogInterlocutors;
	BeginDialogForInterlocutors(OwningDialogComponent);
	ActiveDialogUnit = NewObject<UObject>(this, InitialDialogUnit);
	if (IsValid(ActiveDialogUnit))
	{
		if (ActiveDialogUnit->Implements<UDialogUnitInterface>())
		{
			IDialogUnitInterface::Execute_Activate(ActiveDialogUnit, this);
			UnitStartedForInterlocutors(ActiveDialogUnit);
		}
	}
}

void UDialog::OnDialogUnitPassed(UObject* DialogUnit, TSubclassOf<UObject> NextDialogUnitClass)
{
	if (ActiveDialogUnit == DialogUnit)
	{
		UObject* PrevDialogUnit = ActiveDialogUnit;
		UnitEndedForInterlocutors(PrevDialogUnit);

		if (IsValid(NextDialogUnitClass))
		{
			ActiveDialogUnit = NewObject<UObject>(this, NextDialogUnitClass);
			if (IsValid(ActiveDialogUnit))
			{
				if (ActiveDialogUnit->Implements<UDialogUnitInterface>())
				{
					IDialogUnitInterface::Execute_Activate(ActiveDialogUnit, this);
					UnitStartedForInterlocutors(ActiveDialogUnit);
				}
			}
		}
		else
		{
			ActiveDialogUnit = nullptr;
			EndDialogForInterlocutors();
		}
	}
}

AActor* UDialog::GetOwnerOfVoice(UDialogueVoice* Voice)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor))
		{
			if (Interlocutor->Implements<UTalkableInterface>())
			{
				UDialogComponent* InterlocutorsDialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
				if (IsValid(InterlocutorsDialogComponent))
				{
					if (Voice == InterlocutorsDialogComponent->GetSpeakerVoice())
					{
						return Interlocutor;
					}
				}
			}
		}
	}
	return nullptr;
}

UDialogCue* UDialog::GetCurrentDialogCue()
{
	return Cast<UDialogCue>(ActiveDialogUnit);
}

UDialogSelection* UDialog::GetCurrentDialogSelection()
{
	return Cast<UDialogSelection>(ActiveDialogUnit);
}

TArray<UDialogueVoice*> UDialog::GetInterlocutorsVoices()
{
	TArray<UDialogueVoice*> Voices;
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor))
		{
			if (Interlocutor->Implements<UTalkableInterface>())
			{
				UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
				if (IsValid(DialogComponent))
				{
					Voices.Add(DialogComponent->GetSpeakerVoice());
				}
			}
		}
	}
	return Voices;
}

void UDialog::BeginDialogForInterlocutors(UDialogComponent* MasterDialogComponent)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor))
		{
			if (Interlocutor->Implements<UTalkableInterface>())
			{
				UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
				if(IsValid(DialogComponent))
				{
					DialogComponent->DialogStarted(MasterDialogComponent);
				}
			}
		}
	}
}

void UDialog::EndDialogForInterlocutors()
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor))
		{
			if (Interlocutor->Implements<UTalkableInterface>())
			{
				UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
				if (IsValid(DialogComponent))
				{
					DialogComponent->DialogEnded();
				}
			}
		}
	}
}

void UDialog::UnitStartedForInterlocutors(UObject* DialogUnit)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor))
		{
			if (Interlocutor->Implements<UTalkableInterface>())
			{
				UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
				if (IsValid(DialogComponent))
				{
					DialogComponent->UnitStarted(DialogUnit);
				}
			}
		}
	}
}

void UDialog::UnitEndedForInterlocutors(UObject* DialogUnit)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor))
		{
			if (Interlocutor->Implements<UTalkableInterface>())
			{
				UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
				if (IsValid(DialogComponent))
				{
					DialogComponent->UnitPassed(DialogUnit);
				}
			}
		}
	}
}