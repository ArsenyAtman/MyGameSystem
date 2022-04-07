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

}

void UDialogComponent::BeginDialogue(AActor* Initiator, const TArray<AActor*>& AdditionalInterlocutors)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(CurrentDialog) && !IsValid(MasterDialogComponent) && IsValid(DialogClass))
		{
			CurrentDialog = NewObject<UDialog>(this, DialogClass);
			if (IsValid(CurrentDialog))
			{
				CurrentDialog->Begin(this, this->GetOwner(), Initiator, AdditionalInterlocutors);

				if (OnDialogStarted.IsBound())
				{
					OnDialogStarted.Broadcast();
				}
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

			if (OnDialogStarted.IsBound())
			{
				OnDialogStarted.Broadcast();
			}
		}
	}
}

void UDialogComponent::DialogEnded()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		MasterDialogComponent = nullptr;
		CurrentDialog = nullptr;

		if (OnDialogEnded.IsBound())
		{
			OnDialogEnded.Broadcast();
		}
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

void UDialogComponent::UnitStarted_Implementation(UDialogUnitDataAsset* DialogUnitData)
{
	CurrentDialogUnitData = DialogUnitData;

	if (OnDialogUnitStarted.IsBound())
	{
		OnDialogUnitStarted.Broadcast(DialogUnitData);
	}
}

void UDialogComponent::UnitPassed_Implementation(UDialogUnitDataAsset* DialogUnitData)
{
	CurrentDialogUnitData = nullptr;

	if (OnDialogUnitEnded.IsBound())
	{
		OnDialogUnitEnded.Broadcast(DialogUnitData);
	}
}

void UDialogComponent::OnRep_MasterDialogComponent()
{
	if (IsValid(MasterDialogComponent))
	{
		if (OnDialogStarted.IsBound())
		{
			OnDialogStarted.Broadcast();
		}
	}
	else
	{
		if (OnDialogEnded.IsBound())
		{
			OnDialogEnded.Broadcast();
		}
	}
}