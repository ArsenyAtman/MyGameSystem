// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogSystemTypes.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EDialogCueType : uint8
{
	Usual		UMETA(DisplayName = "Usual"),
	Additional	UMETA(DisplayName = "Additional"),
	Important	UMETA(DisplayName = "Important")
};
