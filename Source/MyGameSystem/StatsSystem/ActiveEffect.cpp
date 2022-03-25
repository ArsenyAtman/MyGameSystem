// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEffect.h"
#include "Stat.h"

FStatValues UActiveEffect::ApplyDeltaToRelatedStats(FStatValues Delta)
{
	FStatValues DeltaSum;

	for (UStat* Stat : GetRelatedStats())
	{
		DeltaSum += Stat->ApplyDelta(Delta, this);
	}

	return DeltaSum;
}