// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsSystem/StatDeltaApplier.h"

#include "StatsSystem/Stat.h"
#include "StatsSystem/Effect.h"

UStat* UStatDeltaApplier::GetOwningStat() const
{
	return Cast<UStat>(GetOuter());
}