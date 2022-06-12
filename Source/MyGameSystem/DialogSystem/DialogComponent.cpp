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

	DOREPLIFETIME(UDialogComponent, MasterDialogComponent);
	DOREPLIFETIME(UDialogComponent, CurrentDialog);
}

void UDialogComponent::BeginDialogue(AActor* Initiator, const TArray<AActor*>& AdditionalInterlocutors)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(GetCurrentDialog()) && !IsValid(GetMasterDialogComponent()) && IsValid(DialogClass))
		{
			SetCurrentDialog(NewObject<UDialog>(this, DialogClass));
			if (IsValid(GetCurrentDialog()))
			{
				GetCurrentDialog()->Begin(this->GetOwner(), Initiator, AdditionalInterlocutors);
			}
		}
	}
}

void UDialogComponent::SelectDialogCue_Implementation(int CueIndex)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetMasterDialogComponent()))
		{
			UDialog* CurrentDialogOfMaster = GetMasterDialogComponent()->GetCurrentDialog();
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

void UDialogComponent::DialogStarted(UDialogComponent* NewMasterDialogComponent)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!IsValid(GetMasterDialogComponent()))
		{
			SetMasterDialogComponent(NewMasterDialogComponent);
		}
	}
}

void UDialogComponent::DialogEnded()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		SetMasterDialogComponent(nullptr);
		SetCurrentDialog(nullptr);
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
	CurrentDialog = NewDialog;
}

void UDialogComponent::SetMasterDialogComponent(UDialogComponent* NewMasterDialogComponent)
{
	UDialogComponent* PrevMasterDialogComponent = GetMasterDialogComponent();

	MasterDialogComponent = NewMasterDialogComponent;

	Broadcast_MasterDialogComponent(PrevMasterDialogComponent);
}

void UDialogComponent::OnRep_MasterDialogComponent(UDialogComponent* PreReplicationMasterDialogComponent)
{
	Broadcast_MasterDialogComponent(PreReplicationMasterDialogComponent);
}

void UDialogComponent::Broadcast_MasterDialogComponent(UDialogComponent* PrevMasterDialogComponent)
{
	if (IsValid(GetMasterDialogComponent()))
	{
		OnDialogStarted.Broadcast(GetMasterDialogComponent());
	}
	else
	{
		OnDialogEnded.Broadcast(PrevMasterDialogComponent);
	}
}
