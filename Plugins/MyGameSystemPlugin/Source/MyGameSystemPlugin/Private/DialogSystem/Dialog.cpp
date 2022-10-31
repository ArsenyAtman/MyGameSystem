// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogSystem/Dialog.h"

#include "DialogSystem/TalkableInterface.h"
#include "DialogSystem/DialogComponent.h"
#include "DialogSystem/DialogUnit.h"
#include "DialogSystem/DialogCue.h"
#include "Net/UnrealNetwork.h"


void UDialog::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDialog, CurrentDialogUnit);
	DOREPLIFETIME(UDialog, bStarted);
}

void UDialog::Begin(AActor* Master, AActor* Initiator, const TArray<AActor*>& OtherInterlocutors)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		DialogMaster = Master;
		DialogInitiator = Initiator;
		AdditionalInterlocutors = OtherInterlocutors;

		TArray<AActor*> InterlocutorsWithoutMaster = GetAdditionalInterlocutors();
		InterlocutorsWithoutMaster.Add(DialogInitiator);
		BeginDialogForInterlocutors(InterlocutorsWithoutMaster);

		SetStarted(true);

		SetCurrentDialogUnit(NewObject<UDialogUnit>(this, InitialDialogUnit));
		if (IsValid(GetCurrentDialogUnit()))
		{
			GetCurrentDialogUnit()->Activate();
		}
	}
}

void UDialog::OnDialogUnitPassed(UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass)
{
	if (GetNetRole() == ENetRole::ROLE_Authority && GetCurrentDialogUnit() == DialogUnit)
	{
		if (IsValid(NextDialogUnitClass))
		{
			SetCurrentDialogUnit(NewObject<UDialogUnit>(this, NextDialogUnitClass));
			if (IsValid(GetCurrentDialogUnit()))
			{
				GetCurrentDialogUnit()->Activate();
			}
		}
		else
		{
			SetCurrentDialogUnit(nullptr);

			TArray<AActor*> InterlocutorsWithoutMaster = GetAdditionalInterlocutors();
			InterlocutorsWithoutMaster.Add(DialogInitiator);
			EndDialogForInterlocutors(InterlocutorsWithoutMaster);

			SetStarted(false);
		}
	}
}

TArray<AActor*> UDialog::GetAllInterlocutors() const
{
	TArray<AActor*> AllInterlocutors = AdditionalInterlocutors;
	AllInterlocutors.Add(DialogInitiator);
	AllInterlocutors.Add(DialogMaster);
	return AllInterlocutors;
}

UDialogComponent* UDialog::GetOwningDialogComponent() const
{
	return Cast<UDialogComponent>(GetOuter());
}

bool UDialog::WasCueFired(TSubclassOf<class UDialogCue> DialogCueClass) const
{
	return FiredCues.Find(DialogCueClass) != INDEX_NONE;
}

void UDialog::BeginDialogForInterlocutors(const TArray<AActor*>& DialogInterlocutorsWithoutMaster)
{
	for (const AActor* Interlocutor : DialogInterlocutorsWithoutMaster)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->DialogStarted(this);
			}
		}
	}
}

void UDialog::EndDialogForInterlocutors(const TArray<AActor*>& DialogInterlocutors)
{
	for (const AActor* Interlocutor : DialogInterlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->DialogEnded(this);
			}
		}
	}
}

void UDialog::SetCurrentDialogUnit(UDialogUnit* NewDialogUnit)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		UDialogUnit* PrevDialogUnit = CurrentDialogUnit;
		CurrentDialogUnit = NewDialogUnit;
		DialogConditionChanged(PrevDialogUnit);
	}
}

void UDialog::OnRep_CurrentDialogUnit(UDialogUnit* PrevDialogUnit)
{
	DialogConditionChanged(PrevDialogUnit);
}

void UDialog::DialogConditionChanged(UDialogUnit* PrevDialogUnit)
{
	UDialogCue* PrevDialogCue = Cast<UDialogCue>(PrevDialogUnit);
	if (IsValid(PrevDialogCue))
	{
		FiredCues.AddUnique(PrevDialogCue->GetClass());
	}

	OnDialogUnitChanged.Broadcast(GetCurrentDialogUnit(), this);
}

void UDialog::SetStarted(bool bNewStarted)
{
	if (bNewStarted != bStarted)
	{
		bStarted = bNewStarted;

		Broadcast_Started();
	}
}

void UDialog::OnRep_Started()
{
	Broadcast_Started();
}

void UDialog::Broadcast_Started()
{
	if(bStarted)
	{
		OnDialogStarted.Broadcast(this);
	}
	else
	{
		OnDialogEnded.Broadcast(this);
	}
}
