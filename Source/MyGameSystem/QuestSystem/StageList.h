// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage.h"
#include "StageList.generated.h"

/**
 * Stage with necessary objectives.
 */
UCLASS()
class MYGAMESYSTEM_API UStageList : public UStage
{
	GENERATED_BODY()

public:

	// Override for the replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	// Override of the objectives activation.
	virtual void ActivateObjectives_Implementation() override;

	// Override of the condition check.
	virtual void CheckCondition_Implementation() override;

	// Override of the objectives abortion.
	virtual void AbortAllObjectives_Implementation() override;

	// Override of the objectives accessor.
	TArray<class UObjective*> GetStageObjectives_Implementation() const override;

	/**
	 * List of objective classes to instance.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageList|Objectives", meta = (BlueprintProtected))
	TArray<TSubclassOf<class UObjective>> InitialObjectives;

	/**
	 * The next stage class to run if this stage completes.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageList|QuestFlow", meta = (BlueprintProtected))
	TSubclassOf<class UStage> NextStageIfCompleted;

	/**
	 * The next stage class to run if this stage fails.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StageList|QuestFlow", meta = (BlueprintProtected))
	TSubclassOf<class UStage> NextStageIfFailed;

	/**
	 * List of all objectives of this stage.
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StageList|Objectives", meta = (BlueprintProtected))
	TArray<class UObjective*> ActiveObjectives;

private:

	// ...
	
};
