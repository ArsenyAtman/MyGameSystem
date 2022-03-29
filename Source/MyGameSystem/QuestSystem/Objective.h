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

	bool operator == (const FObjectiveInfo& Info)
	{
		return 	this->ObjectiveData == Info.ObjectiveData &&
				this->Condition == Info.Condition &&
				this->Progress == Info.Progress;
	}

	bool operator != (const FObjectiveInfo& Info)
	{
		return !((*this) == Info);
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

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjectiveInfo ObjectiveInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class TSubclassOf<AActor> MarkerClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<class AActor*> OnLevelObjectives;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fail();
	virtual void Fail_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float RecalculateProgress();
	virtual float RecalculateProgress_Implementation() { return -1.0f; }

	UFUNCTION(BlueprintCallable)
	void MarkObjectives(TArray<class AActor*> Objectives);

	UFUNCTION(BlueprintCallable)
	void UnmarkObjectives(TArray<class AActor*> Objectives);

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> GetOnLevelObjectivesFormManager();

private:

	class UStage* OwningStage;

	TArray<class AActor*> Markers;
};
