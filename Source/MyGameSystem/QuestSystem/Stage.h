// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemTypes.h"
#include "Objective.h"
#include "Stage.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FStageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(BlueprintReadWrite)
	ETaskCondition Condition;

	UPROPERTY(BlueprintReadOnly)
	TArray<FObjectiveInfo> ObjectivesInfo;

	FStageInfo(FText ObjectiveName = FText::FromString("None"), FText ObjectiveDescription = FText::FromString("None"))
	{
		Name = ObjectiveName;
		Description = ObjectiveDescription;
		Condition = ETaskCondition::InProcess;
	}
};

UCLASS(Blueprintable, BlueprintType, Abstract)
class MYGAMESYSTEM_API UStage : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Activate(class UQuest* RelatedQuest);
	virtual void Activate_Implementation(class UQuest* RelatedQuest);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ObjectiveCompleted(class UObjective* Objective);
	virtual void ObjectiveCompleted_Implementation(class UObjective* Objective);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ObjectiveFailed(class UObjective* Objective);
	virtual void ObjectiveFailed_Implementation(class UObjective* Objective);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintCallable)
	void MarkObjectives();

	UFUNCTION(BlueprintCallable)
	void UnmarkObjectives();

	UFUNCTION(BlueprintPure)
	FStageInfo GetStageInfo(); // was FORCEINLINE

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UQuest* GetOwningQuest() { return OwningQuest; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CheckCondition();
	virtual void CheckCondition_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Complete(class UClass* NextStage);
	virtual void Complete_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fail(class UClass* NextStage);
	virtual void Fail_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AbortAllObjectives();
	virtual void AbortAllObjectives_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<class UObjective*> GetStageObjectives();
	virtual TArray<class UObjective*> GetStageObjectives_Implementation() { return TArray<class UObjective*>(); }

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> GetAllActorsOfClass(class UClass* ActorClass);

	UFUNCTION(BlueprintPure)
	bool IsAllObjectivesCompleted(TArray<class UObjective*> Objectives);

	UFUNCTION(BlueprintPure)
	bool IsAllObjectivesFailed(TArray<class UObjective*> Objectives);

	UFUNCTION(BlueprintPure)
	bool IsOneObjectiveCompleted(TArray<class UObjective*> Objectives);

	UFUNCTION(BlueprintPure)
	bool IsOneObjectiveFailed(TArray<class UObjective*> Objectives);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FStageInfo StageInfo;

private:

	class UQuest* OwningQuest;
	
};
