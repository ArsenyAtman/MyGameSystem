// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogComponent.h"
#include "TalkableInterface.h"
#include "Dialog.h"
#include "DialogCue.h"
#include "DialogSelection.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/DialogueWave.h"
#include "Sound/DialogueVoice.h"

// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UDialogComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UDialogComponent, MasterDialogComponent, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(UDialogComponent, CurrentDialogCueInfo, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(UDialogComponent, CurrentDialogSelectionInfo, COND_OwnerOnly);

}

void UDialogComponent::BeginDialogue(APawn* Initiator, const TArray<AActor*>& AdditionalInterlocutors)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(CurrentDialog) && !IsValid(MasterDialogComponent) && IsValid(DialogClass))
		{
			TArray<AActor*> Interlocutors = AdditionalInterlocutors;

			//MasterDialogComponent = this;
			CurrentDialog = NewObject<UDialog>(this, DialogClass);
			if (IsValid(CurrentDialog))
			{
				Interlocutors.Add(this->GetOwner());
				Interlocutors.Add(Initiator);

				CurrentDialog->Begin(this, this->GetOwner(), Initiator, Interlocutors);

				OnDialogStarted.Broadcast();
			}
		}
	}
}

void UDialogComponent::SelectDialogCue_Implementation(int CueIndex)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(MasterDialogComponent))
		{
			UDialog* CurrentDialogOfMaster = MasterDialogComponent->GetCurrentDialog();
			if (IsValid(CurrentDialogOfMaster))
			{
				UDialogSelection* ActiveSelection = Cast<UDialogSelection>(CurrentDialogOfMaster->GetCurrentDialogUnit());
				if (IsValid(ActiveSelection))
				{
					ActiveSelection->SelectNextCue(CueIndex);
				}
			}
		}
	}
}

void UDialogComponent::DialogStarted(class UDialogComponent* NewMasterDialogComponent)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(MasterDialogComponent))
		{
			MasterDialogComponent = NewMasterDialogComponent;
			OnDialogStarted.Broadcast();
		}
	}
}

void UDialogComponent::DialogEnded()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		MasterDialogComponent = nullptr;
		CurrentDialog = nullptr;
		OnDialogEnded.Broadcast();
	}
}

void UDialogComponent::AddNote(FString NoteToAdd)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Notepad.Add(NoteToAdd);
	}
}

void UDialogComponent::RemoveNote(FString NoteToRemove)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Notepad.RemoveSingle(NoteToRemove);
	}
}

void UDialogComponent::UnitStarted(class UObject* DialogUnit)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		UDialogSelection* Selection = Cast<UDialogSelection>(DialogUnit);
		if (IsValid(Selection))
		{
			CurrentDialogCueInfo = nullptr;
			CurrentDialogSelectionInfo = Selection->GetSelectionInfo();
			OnDialogSelectionStarted.Broadcast();
			return;
		}

		UDialogCue* Cue = Cast<UDialogCue>(DialogUnit);
		if (IsValid(Cue))
		{
			CurrentDialogCueInfo = Cue->GetCueInfo();
			CurrentDialogSelectionInfo = nullptr;
			OnDialogCueStarted.Broadcast();
			return;
		}
	}
}

void UDialogComponent::UnitPassed(class UObject* DialogUnit)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		CurrentDialogCueInfo = nullptr;
		CurrentDialogSelectionInfo = nullptr;
		OnDialogUnitEnded.Broadcast();
	}
}

void UDialogComponent::OnRep_MasterDialogComponent()
{
	if (IsValid(MasterDialogComponent))
	{
		OnDialogStarted.Broadcast();
	}
	else
	{
		OnDialogEnded.Broadcast();
	}
}


void UDialogComponent::OnRep_CurrentDialogCueInfo()
{
	if (IsValid(CurrentDialogCueInfo))
	{
		OnDialogCueStarted.Broadcast();
	}
	else
	{
		OnDialogUnitEnded.Broadcast();
	}
}

void UDialogComponent::OnRep_CurrentDialogSelectionInfo()
{
	if (IsValid(CurrentDialogSelectionInfo))
	{
		OnDialogSelectionStarted.Broadcast();
	}
	else
	{
		OnDialogUnitEnded.Broadcast();
	}
}