// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEffect.h"
#include "Stat.h"
#include "Net/UnrealNetwork.h"

FStatValues UActiveEffect::ApplyDeltaToRelatedStats(FStatValues Delta)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		FStatValues DeltaSum;

		for (UStat* Stat : GetRelatedStats())
		{
			DeltaSum += Stat->ApplyDelta(Delta, this);
		}

		return DeltaSum;
	}

	return FStatValues();
}