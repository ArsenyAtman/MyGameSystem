// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage.h"
#include "StageList.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UStageList : public UStage
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UQuest* RelatedQuest) override;

protected:

	virtual void CheckCondition_Implementation() override;

	virtual void AbortAllObjectives_Implementation() override;

	TArray<class UObjective*> GetStageObjectives_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class UObjective>> InitialObjectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UStage> NextStageIfCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UStage> NextStageIfFailed;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UObjective*> ActiveObjectives;
	
};