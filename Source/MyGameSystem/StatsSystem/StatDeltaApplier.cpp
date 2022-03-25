// Fill out your copyright notice in the Description page of Project Settings.


#include "StatDeltaApplier.h"
#include "Stat.h"
#include "Effect.h"

UStat* UStatDeltaApplier::GetOwningStat()
{
	return Cast<UStat>(GetOuter());
}