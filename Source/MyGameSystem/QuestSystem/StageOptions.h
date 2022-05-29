// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage.h"
#include "StageOptions.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FOption
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UObjective> ObjectiveClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UStage> NextStageClass;
};

/**
 * Stage with options.
 */
UCLASS()
class MYGAMESYSTEM_API UStageOptions : public UStage
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
	TArray<class UObjective*> GetStageObjectives_Implementation() const override { return StageObjectives; }

	/**
	 * Get a next stage class to run related to the specified objective from the options.
	 * @param OfObjective - The specified objective.
	 * @param FromOptions - The list of options.
	 * @return The next stage class to run.
	 */
	UFUNCTION(BlueprintPure, Category = "StageOptions|QuestFlow", meta = (BlueprintProtected))
	class UClass* GetNextStage(class UObjective* OfObjective, const TArray<FOption>& FromOptions) const;

	/**
	 * Get the first completed objective from the list.
	 * @param Objectives - TArray of objectives.
	 * @return The first completed objective.
	 */
	UFUNCTION(BlueprintPure, Category = "StageOptions|Objectives", meta = (BlueprintProtected))
	UObjective* GetCompletedObjective(const TArray<class UObjective*>& Objectives) const;

	/**
	 * List of the options of this stage.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "StageOptions|QuestFlow", meta = (BlueprintProtected))
	TArray<FOption> Options;

	/**
	 * List of all objectives of this stage.
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StageOptions|QuestFlow", meta = (BlueprintProtected))
	TArray<class UObjective*> StageObjectives;

	/**
	 * The next stage class to run if this stage fails.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StageOptions|QuestFlow", meta = (BlueprintProtected))
	TSubclassOf<class UStage> NextStageClassIfFailed;

private:

	// ...

};
