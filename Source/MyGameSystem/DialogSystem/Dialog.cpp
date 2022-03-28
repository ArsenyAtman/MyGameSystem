// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialog.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogUnit.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialog::Begin(UDialogComponent* OwnDialogComponent, class AActor* Master, class AActor* Initiator, TArray<class AActor*> OtherInterlocutors)
{
	OwningDialogComponent = OwnDialogComponent;

	DialogMaster = Master;
	DialogInitiator = Initiator;

	Interlocutors = OtherInterlocutors;
	Interlocutors.Add(DialogMaster);
	Interlocutors.Add(DialogInitiator);

	BeginDialogForInterlocutors(OwningDialogComponent);

	ActiveDialogUnit = NewObject<UDialogUnit>(this, InitialDialogUnit);
	if (IsValid(ActiveDialogUnit))
	{
		ActiveDialogUnit->Activate(this);
		UnitStartedForInterlocutors(ActiveDialogUnit);
	}
}

void UDialog::OnDialogUnitPassed(UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass)
{
	if (ActiveDialogUnit == DialogUnit)
	{
		UDialogUnit* PrevDialogUnit = ActiveDialogUnit;
		UnitEndedForInterlocutors(PrevDialogUnit);

		if (IsValid(NextDialogUnitClass))
		{
			ActiveDialogUnit = NewObject<UDialogUnit>(this, NextDialogUnitClass);
			if (IsValid(ActiveDialogUnit))
			{
				ActiveDialogUnit->Activate(this);
				UnitStartedForInterlocutors(ActiveDialogUnit);
			}
		}
		else
		{
			ActiveDialogUnit = nullptr;
			EndDialogForInterlocutors();
		}
	}
}

void UDialog::BeginDialogForInterlocutors(UDialogComponent* MasterDialogComponent)
{
	for (AActor* Interlocutor : Interlocutors)
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

void UDialog::EndDialogForInterlocutors()
{
	for (AActor* Interlocutor : Interlocutors)
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

void UDialog::UnitStartedForInterlocutors(UDialogUnit* DialogUnit)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->UnitStarted(DialogUnit->GetDialogUnitData());
			}
		}
	}
}

void UDialog::UnitEndedForInterlocutors(UDialogUnit* DialogUnit)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->UnitPassed(DialogUnit->GetDialogUnitData());
			}
		}
	}
}