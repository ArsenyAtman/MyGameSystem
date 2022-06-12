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
	DialogMaster = Master;
	DialogInitiator = Initiator;

	Interlocutors = OtherInterlocutors;
	Interlocutors.Add(DialogMaster);
	Interlocutors.Add(DialogInitiator);

	BeginDialogForInterlocutors(GetOwningDialogComponent(), Interlocutors);

	SetCurrentDialogUnit(NewObject<UDialogUnit>(this, InitialDialogUnit));
	if (IsValid(GetCurrentDialogUnit()))
	{
		GetCurrentDialogUnit()->Activate();
	}
}

void UDialog::OnDialogUnitPassed(UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass)
{
	if (GetCurrentDialogUnit() == DialogUnit)
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
			EndDialogForInterlocutors(Interlocutors);
		}
	}
}

UDialogComponent* UDialog::GetOwningDialogComponent() const
{
	return Cast<UDialogComponent>(GetOuter());
}

void UDialog::BeginDialogForInterlocutors(UDialogComponent* MasterDialogComponent, const TArray<AActor*>& DialogInterlocutors)
{
	for (const AActor* Interlocutor : DialogInterlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->DialogStarted(MasterDialogComponent);
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
				DialogComponent->DialogEnded();
			}
		}
	}
}

void UDialog::SetCurrentDialogUnit(UDialogUnit* NewDialogUnit)
{
	CurrentDialogUnit = NewDialogUnit;

	Broadcast_CurrentDialogUnit();
}

void UDialog::OnRep_CurrentDialogUnit()
{
	Broadcast_CurrentDialogUnit();
}

void UDialog::Broadcast_CurrentDialogUnit()
{
	OnDialogUnitChanged.Broadcast(GetCurrentDialogUnit(), this);
}
