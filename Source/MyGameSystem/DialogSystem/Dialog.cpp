// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialog.h"
#include "TalkableInterface.h"
#include "DialogComponent.h"
#include "DialogUnit.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialog::Begin(UDialogComponent* OwnDialogComponent, class AActor* Master, class AActor* Initiator, const TArray<class AActor*>& OtherInterlocutors)
{
	OwningDialogComponent = OwnDialogComponent;

	DialogMaster = Master;
	DialogInitiator = Initiator;

	Interlocutors = OtherInterlocutors;
	Interlocutors.Add(DialogMaster);
	Interlocutors.Add(DialogInitiator);

	BeginDialogForInterlocutors(OwningDialogComponent, Interlocutors);

	ActiveDialogUnit = NewObject<UDialogUnit>(this, InitialDialogUnit);
	if (IsValid(ActiveDialogUnit))
	{
		ActiveDialogUnit->Activate(this);
		UnitStartedForInterlocutors(ActiveDialogUnit, Interlocutors);
	}
}

void UDialog::OnDialogUnitPassed(UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass)
{
	if (ActiveDialogUnit == DialogUnit)
	{
		UDialogUnit* PrevDialogUnit = ActiveDialogUnit;
		UnitEndedForInterlocutors(PrevDialogUnit, Interlocutors);

		if (IsValid(NextDialogUnitClass))
		{
			ActiveDialogUnit = NewObject<UDialogUnit>(this, NextDialogUnitClass);
			if (IsValid(ActiveDialogUnit))
			{
				ActiveDialogUnit->Activate(this);
				UnitStartedForInterlocutors(ActiveDialogUnit, Interlocutors);
			}
		}
		else
		{
			ActiveDialogUnit = nullptr;
			EndDialogForInterlocutors(Interlocutors);
		}
	}
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

void UDialog::UnitStartedForInterlocutors(UDialogUnit* DialogUnit, const TArray<AActor*>& DialogInterlocutors)
{
	for (const AActor* Interlocutor : DialogInterlocutors)
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

void UDialog::UnitEndedForInterlocutors(UDialogUnit* DialogUnit, const TArray<AActor*>& DialogInterlocutors)
{
	for (const AActor* Interlocutor : DialogInterlocutors)
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