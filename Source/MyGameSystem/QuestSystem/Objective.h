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

	UPROPERTY(BlueprintReadWrite)
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Activate(class UStage* RelatedStage);
	virtual void Activate_Implementation(class UStage* RelatedStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Abort();
	virtual void Abort_Implementation();

	UFUNCTION(BlueprintCallable)
	void Mark();

	UFUNCTION(BlueprintCallable)
	void Unmark();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintPure)
	FObjectiveInfo GetObjectiveInfo();

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UStage* GetOwningStage() { return OwningStage; }

	UFUNCTION(BlueprintGetter)
	TArray<class AActor*> GetActorsForQuest() { return ActorsForQuest; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjectiveInfo ObjectiveInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class TSubclassOf<AActor> MarkerClass;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fail();
	virtual void Fail_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float RecalculateProgress();
	virtual float RecalculateProgress_Implementation() { return -1.0f; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<class AActor*> FilterActorsForMarking(const TArray<class AActor*>& ActorsToMark);
	virtual TArray<class AActor*> FilterActorsForMarking_Implementation(const TArray<class AActor*>& ActorsToMark) { return ActorsToMark; }

private:

	class UStage* OwningStage;

	TArray<class AActor*> FindActorsForQuest();

	UPROPERTY(BlueprintGetter = GetActorsForQuest)
	TArray<class AActor*> ActorsForQuest;

	TArray<class AActor*> FindActorsForMarking();
	TArray<class AActor*> ActorsForMarking;
	TArray<class AActor*> MarkedActors;
};
