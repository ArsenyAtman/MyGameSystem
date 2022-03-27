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

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCueInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
	TSubclassOf<UObject> NextDialogUnit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogCueType DialogCueType = EDialogCueType::Usual;
};

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCueInfo_AnimationAndSound : public UDialogCueInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Animation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* Voice = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueWave* Sound = nullptr;
	
};

UCLASS(BlueprintType, Blueprintable)
class UDialogSelectionInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
	TArray<TSubclassOf<class UDialogCue>> Options;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bWithTimer = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Time = -1.0f;
};
