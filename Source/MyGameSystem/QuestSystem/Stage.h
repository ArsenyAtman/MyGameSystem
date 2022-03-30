// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemTypes.h"
#include "Objective.h"
#include "Stage.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UStageData : public UDataAsset
{
	GENERATED_BODY()

public:

	// ...

};

USTRUCT(Blueprintable, BlueprintType)
struct FStageInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	class UStageData* StageData;

	UPROPERTY(BlueprintReadWrite)
	ETaskCondition Condition;

	UPROPERTY(BlueprintReadWrite)
	TArray<FObjectiveInfo> ObjectivesInfo;

	FStageInfo(class UStageData* Data = nullptr, ETaskCondition StageCondition = ETaskCondition::InProcess, TArray<FObjectiveInfo> Objectives = TArray<FObjectiveInfo>())
	{
		StageData = Data;
		Condition = StageCondition;
		ObjectivesInfo = Objectives;
	}

	friend bool operator == (const FStageInfo& Info1, const FStageInfo& Info2)
	{
		return 	Info1.StageData == Info2.StageData &&
				Info1.Condition == Info2.Condition &&
				Info1.ObjectivesInfo == Info2.ObjectivesInfo;
	}

	friend bool operator != (const FStageInfo& Info1, const FStageInfo& Info2)
	{
		return !(Info1 == Info2);
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
	FStageInfo GetStageInfo();

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
