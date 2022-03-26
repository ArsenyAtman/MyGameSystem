// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialog.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogUnitInterface.h"
#include "DialogCue.h"
#include "DialogSelection.h"
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

	ActiveDialogUnit = NewObject<UObject>(this, InitialDialogUnit);
	if (IsValid(ActiveDialogUnit) && ActiveDialogUnit->Implements<UDialogUnitInterface>())
	{
		IDialogUnitInterface::Execute_Activate(ActiveDialogUnit, this);
		UnitStartedForInterlocutors(ActiveDialogUnit);
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
			if (IsValid(ActiveDialogUnit) && ActiveDialogUnit->Implements<UDialogUnitInterface>())
			{
				IDialogUnitInterface::Execute_Activate(ActiveDialogUnit, this);
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

UDialogCue* UDialog::GetCurrentDialogCue()
{
	return Cast<UDialogCue>(ActiveDialogUnit);
}

UDialogSelection* UDialog::GetCurrentDialogSelection()
{
	return Cast<UDialogSelection>(ActiveDialogUnit);
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

void UDialog::UnitStartedForInterlocutors(UObject* DialogUnit)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->UnitStarted(DialogUnit);
			}
		}
	}
}

void UDialog::UnitEndedForInterlocutors(UObject* DialogUnit)
{
	for (AActor* Interlocutor : Interlocutors)
	{
		if (IsValid(Interlocutor) && Interlocutor->Implements<UTalkableInterface>())
		{
			UDialogComponent* DialogComponent = ITalkableInterface::Execute_GetDialogComponent(Interlocutor);
			if (IsValid(DialogComponent))
			{
				DialogComponent->UnitPassed(DialogUnit);
			}
		}
	}
}