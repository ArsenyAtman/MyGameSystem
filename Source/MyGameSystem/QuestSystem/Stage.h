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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Activate(class UQuest* RelatedQuest);
	virtual void Activate_Implementation(class UQuest* RelatedQuest);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ObjectiveCompleted(class UObjective* Objective);
	virtual void ObjectiveCompleted_Implementation(class UObjective* Objective);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ObjectiveFailed(class UObjective* Objective);
	virtual void ObjectiveFailed_Implementation(class UObjective* Objective);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Stage|Internal")
	void MarkObjectives();

	UFUNCTION(BlueprintCallable, Category = "Stage|Internal")
	void UnmarkObjectives();

	UFUNCTION(BlueprintPure, Category = "Stage|Info")
	FStageInfo GetStageInfo() const;

	UFUNCTION(BlueprintPure, Category = "Stage|OwningQuest")
	FORCEINLINE class UQuest* GetOwningQuest() const { return OwningQuest; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ActivateObjectives();
	virtual void ActivateObjectives_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void CheckCondition();
	virtual void CheckCondition_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Complete(TSubclassOf<class UStage> NextStage);
	virtual void Complete_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Fail(TSubclassOf<class UStage> NextStage);
	virtual void Fail_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageActivated();
	virtual void OnStageActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageCompleted();
	virtual void OnStageCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageFailed();
	virtual void OnStageFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageUpdated();
	virtual void OnStageUpdated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void AbortAllObjectives();
	virtual void AbortAllObjectives_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Objectives")
	TArray<class UObjective*> GetStageObjectives() const;
	virtual TArray<class UObjective*> GetStageObjectives_Implementation() const { return TArray<class UObjective*>(); }

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsAllObjectivesCompleted(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsAllObjectivesFailed(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsOneObjectiveCompleted(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsOneObjectiveFailed(const TArray<class UObjective*>& Objectives) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stage|Info")
	UStageData* StageData;

private:

	class UQuest* OwningQuest;

	ETaskCondition Condition = ETaskCondition::Aborted;
	
};
