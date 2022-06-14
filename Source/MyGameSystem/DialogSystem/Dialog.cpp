// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialog.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogUnit.h"
#include "Net/UnrealNetwork.h"


void UDialog::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDialog, CurrentDialogUnit);
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
		UDialogUnit* PrevDialogUnit = GetCurrentDialogUnit();

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
			EndDialogForInterlocutors(GetAllInterlocutors());
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
		CurrentDialogUnit = NewDialogUnit;

		Broadcast_CurrentDialogUnit();
	}
}

void UDialog::OnRep_CurrentDialogUnit()
{
	Broadcast_CurrentDialogUnit();
}

void UDialog::Broadcast_CurrentDialogUnit()
{
	OnDialogUnitChanged.Broadcast(GetCurrentDialogUnit(), this);
}
