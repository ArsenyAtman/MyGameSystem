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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void ActivateObjectives_Implementation() override;

	virtual void CheckCondition_Implementation() override;

	virtual void AbortAllObjectives_Implementation() override;

	TArray<class UObjective*> GetStageObjectives_Implementation() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageList|Objectives")
	TArray<TSubclassOf<class UObjective>> InitialObjectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageList|QuestFlow")
	TSubclassOf<class UStage> NextStageIfCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageList|QuestFlow")
	TSubclassOf<class UStage> NextStageIfFailed;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StageList|Objectives")
	TArray<class UObjective*> ActiveObjectives;
	
};
