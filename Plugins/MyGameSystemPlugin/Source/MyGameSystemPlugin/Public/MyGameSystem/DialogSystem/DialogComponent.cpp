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
		if (IsValid(GetCurrentDialog()))
		{
			AActor* DialogMaster = this->GetOwner();
			GetCurrentDialog()->Begin(DialogMaster, Initiator, AdditionalInterlocutors);
		}
	}
}

void UDialogComponent::SelectNextDialogUnit_Implementation(TSubclassOf<UDialogUnit> NextDialogUnit)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetCurrentDialog()))
		{
			UDialogSelection* ActiveSelection = Cast<UDialogSelection>(GetCurrentDialog()->GetCurrentDialogUnit());
			if (IsValid(ActiveSelection))
			{
				ActiveSelection->SelectNextDialogUnit(NextDialogUnit);
			}
		}
	}
}

void UDialogComponent::SkipCurrentDialogCue_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority && IsValid(GetCurrentDialog()))
	{
		UDialogCue* CurrentDialogCue = Cast<UDialogCue>(GetCurrentDialog()->GetCurrentDialogUnit());
		if(IsValid(CurrentDialogCue))
		{
			CurrentDialogCue->Skip();
		}
	}
}

void UDialogComponent::DialogStarted(UDialog* NewDialog)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (NewDialog->GetDialogMaster() != this->GetOwner())
		{
			SetCurrentDialog(NewDialog);
		}
	}
}

void UDialogComponent::DialogEnded(UDialog* Dialog)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if(Dialog == GetCurrentDialog() && Dialog->GetDialogMaster() != this->GetOwner())
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

bool UDialogComponent::SetCurrentDialog(UDialog* NewDialog)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!(IsValid(GetCurrentDialog()) && GetCurrentDialog()->GetDialogMaster() == this->GetOwner()))
		{
			UDialog* PrevDialog = GetCurrentDialog();
			CurrentDialog = NewDialog;
			Broadcast_CurrentDialog(PrevDialog);

			return true;
		}
	}

	return false;
}

void UDialogComponent::OnRep_CurrentDialog(UDialog* PreReplicationCurrentDialog)
{
	Broadcast_CurrentDialog(PreReplicationCurrentDialog);
}

void UDialogComponent::Broadcast_CurrentDialog(UDialog* PrevDialog)
{
	OnDialogChanged.Broadcast(GetCurrentDialog(), PrevDialog);
}
