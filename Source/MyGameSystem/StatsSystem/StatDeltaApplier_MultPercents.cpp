// Fill out your copyright notice in the Description page of Project Settings.


#include "StatDeltaApplier_MultPercents.h"
#include "Stat.h"
#include "Effect.h"
#include "PassiveEffect.h"
#include "Kismet/KismetSystemLibrary.h"

FStatValues UStatDeltaApplier_MultPercents::ApplyDelta_Implementation(FStatValues Delta, UEffect* OfEffect)
{
	FStatValues FinalDelta = Delta;

	for (UEffect* Effect : GetOwningStat()->GetEffects())
	{
		UPassiveEffect* PassiveEffect = Cast<UPassiveEffect>(Effect);
		if (IsValid(PassiveEffect))
		{
			FinalDelta = PassiveEffect->ApplyToDelta(FinalDelta, OfEffect, GetOwningStat());
		}
	}

	return FinalDelta;
}