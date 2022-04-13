// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemTypes.h"
#include "Objective.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UObjectiveData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsOptional = false;

};

USTRUCT(Blueprintable, BlueprintType)
struct FObjectiveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UObjectiveData* ObjectiveData;

	UPROPERTY(BlueprintReadWrite)
	ETaskCondition Condition;

	UPROPERTY(BlueprintReadWrite)
	float Progress;

	FObjectiveInfo(class UObjectiveData* Data = nullptr, ETaskCondition ObjectiveCondition = ETaskCondition::InProcess, float ObjectiveProgress = 0.0f)
	{
		ObjectiveData = Data;
		Condition = ObjectiveCondition;
		Progress = ObjectiveProgress;
	}

	friend bool operator == (const FObjectiveInfo& Info1, const FObjectiveInfo& Info2)
	{
		return 	Info1.ObjectiveData == Info2.ObjectiveData &&
				Info1.Condition == Info2.Condition &&
				Info1.Progress == Info2.Progress;
	}

	friend bool operator != (const FObjectiveInfo& Info1, const FObjectiveInfo& Info2)
	{
		return !(Info1 == Info2);
	}
};

UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Activate(class UStage* RelatedStage);
	virtual void Activate_Implementation(class UStage* RelatedStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Abort();
	virtual void Abort_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Mark();

	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Unmark();

	UFUNCTION(BlueprintPure, Category = "Objective|Info")
	FObjectiveInfo GetObjectiveInfo() const { return ObjectiveInfo; }

	UFUNCTION(BlueprintPure, Category = "Objective|OwningStage")
	FORCEINLINE class UStage* GetOwningStage() const { return OwningStage; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Info")
	FObjectiveInfo ObjectiveInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|Marker")
	class TSubclassOf<AActor> MarkerClass;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Fail();
	virtual void Fail_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveActivated();
	virtual void OnObjectiveActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveAborted();
	virtual void OnObjectiveAborted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveCompleted();
	virtual void OnObjectiveCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveFailed();
	virtual void OnObjectiveFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveUpdated();
	virtual void OnObjectiveUpdated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Progress")
	float RecalculateProgress() const;
	virtual float RecalculateProgress_Implementation() const { return -1.0f; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|RelatedActors")
	TArray<class AActor*> FilterActorsForMarking(const TArray<class AActor*>& ActorsToMark) const;
	virtual TArray<class AActor*> FilterActorsForMarking_Implementation(const TArray<class AActor*>& ActorsToMark) const { return ActorsToMark; }

	UFUNCTION(BlueprintGetter, Category = "Objective|RelatedActors")
	FReferencesForQuest GetReferencesForQuest() const { return ReferencesForQuest; }

private:

	class UStage* OwningStage;

	FReferencesForQuest FindReferencesForQuest() const;

	UPROPERTY(BlueprintGetter = GetReferencesForQuest)
	FReferencesForQuest ReferencesForQuest;

	TArray<class AActor*> Markers;
};
