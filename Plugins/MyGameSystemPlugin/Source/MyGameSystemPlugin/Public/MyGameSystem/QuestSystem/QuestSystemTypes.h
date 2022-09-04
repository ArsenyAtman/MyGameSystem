// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemTypes.generated.h"

/**
 * Condition of a quest system object.
 */
UENUM(Blueprintable, BlueprintType)
enum class ETaskCondition : uint8
{
	InProcess	UMETA(DisplayName = "InProcess"),
	Completed	UMETA(DisplayName = "Completed"),
	Failed		UMETA(DisplayName = "Failed"),
	Aborted		UMETA(DisplayName = "Aborted")
};
