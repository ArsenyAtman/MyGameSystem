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

	UPROPERTY(BlueprintReadOnly)
	class UObjective* Objective;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UStage> NextStageClass;
};

UCLASS()
class MYGAMESYSTEM_API UStageOptions : public UStage
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UQuest* RelatedQuest) override;

protected:

	virtual void CheckCondition_Implementation() override;

	virtual void AbortAllObjectives_Implementation() override;

	TArray<class UObjective*> GetStageObjectives_Implementation() override;

	UFUNCTION(BlueprintPure)
	class UClass* GetNextStage(class UObjective* OfObjective, TArray<FOption> FromOptions);

	UFUNCTION(BlueprintPure)
	UObjective* GetCompletedObjective(TArray<class UObjective*> Objectives);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FOption> Options;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UStage> NextStageClassIfFailed;

};
