// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogComponent.h"
#include "TalkableInterface.h"
#include "Dialog.h"
#include "DialogCue.h"
#include "DialogSelection.h"
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

	SetIsReplicatedByDefault(true);
	
}

void UDialogComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDialogComponent, CurrentDialog);
}

void UDialogComponent::BeginDialogue(AActor* Initiator, const TArray<AActor*>& AdditionalInterlocutors)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(GetCurrentDialog()) && IsValid(DialogClass))
		{
			SetCurrentDialog(NewObject<UDialog>(this, DialogClass));
			if (IsValid(GetCurrentDialog()))
			{
				AActor* DialogMaster = this->GetOwner();
				GetCurrentDialog()->Begin(DialogMaster, Initiator, AdditionalInterlocutors);
			}
		}
	}
}

void UDialogComponent::SelectDialogCue_Implementation(int CueIndex)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetCurrentDialog()))
		{
			UDialogSelection* ActiveSelection = Cast<UDialogSelection>(GetCurrentDialog()->GetCurrentDialogUnit());
			if (IsValid(ActiveSelection))
			{
				ActiveSelection->SelectNextCue(CueIndex);
			}
		}
	}
}

void UDialogComponent::DialogStarted(UDialog* NewDialog)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(GetCurrentDialog()))
		{
			SetCurrentDialog(NewDialog);
		}
	}
}

void UDialogComponent::DialogEnded(UDialog* Dialog)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if(Dialog == GetCurrentDialog())
		{
			SetCurrentDialog(nullptr);
		}
	}
}

void UDialogComponent::AddNote(const FString& NoteToAdd)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Notepad.Add(NoteToAdd);
	}
}

void UDialogComponent::RemoveNote(const FString& NoteToRemove)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Notepad.RemoveSingle(NoteToRemove);
	}
}

void UDialogComponent::SetCurrentDialog(UDialog* NewDialog)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		UDialog* PrevDialog = GetCurrentDialog();

		CurrentDialog = NewDialog;

		Broadcast_CurrentDialog(PrevDialog);
	}
}

void UDialogComponent::OnRep_CurrentDialog(UDialog* PreReplicationCurrentDialog)
{
	Broadcast_CurrentDialog(PreReplicationCurrentDialog);
}

void UDialogComponent::Broadcast_CurrentDialog(UDialog* PrevDialog)
{
	if (IsValid(GetCurrentDialog()))
	{
		OnDialogStarted.Broadcast(GetCurrentDialog());
	}
	else
	{
		OnDialogEnded.Broadcast(PrevDialog);
	}
}
