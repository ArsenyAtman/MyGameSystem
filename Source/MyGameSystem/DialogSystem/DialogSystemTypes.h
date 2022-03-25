// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogSystemTypes.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EDialogCueType : uint8
{
	Usual		UMETA(DisplayName = "Usual"),
	Additional	UMETA(DisplayName = "Additional"),
	Important	UMETA(DisplayName = "Important")
};

USTRUCT(BlueprintType, Blueprintable)
struct FDialogCueStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDialogueVoice* DialogueVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDialogueWave* DialogueWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDialogCueType DialogCueType;

	FDialogCueStruct(class UDialogueVoice* Voice = nullptr, class UDialogueWave* Wave = nullptr, EDialogCueType Type = EDialogCueType::Usual)
	{
		DialogueVoice = Voice;
		DialogueWave = Wave;
		DialogCueType = Type;
	}

	bool operator == (const FDialogCueStruct& CueInfo); //was FORCEINLINE
	bool operator != (const FDialogCueStruct& CueInfo); //was FORCEINLINE
};

USTRUCT(BlueprintType, Blueprintable)
struct FDialogSelectionStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogCueStruct> Options;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWithTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;

	FDialogSelectionStruct(bool bHasTimer = false, float TimeRemaining = -1.0f, TArray<FDialogCueStruct> OptionsArray = TArray<FDialogCueStruct>())
	{
		bWithTimer = bHasTimer;
		Time = TimeRemaining;
		Options = OptionsArray;
	}

	bool operator == (const FDialogSelectionStruct& SelectionInfo); //was FORCEINLINE
	bool operator != (const FDialogSelectionStruct& SelectionInfo); //was FORCEINLINE
};