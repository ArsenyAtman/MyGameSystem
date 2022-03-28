// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemTypes.h"
#include "Stage.h"
#include "Quest.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EQuestType : uint8
{
	Usual		UMETA(DisplayName = "Usual"),
	Main		UMETA(DisplayName = "Main"),
	Secondary	UMETA(DisplayName = "Secondary"),
	Challenge	UMETA(DisplayName = "Challenge")
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYGAMESYSTEM_API UQuestData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestType Type = EQuestType::Usual;

};

USTRUCT(Blueprintable, BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite)
	class UQuestData* QuestData;

	UPROPERTY(BlueprintReadWrite)
	ETaskCondition Condition;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingTracked;

	UPROPERTY(BlueprintReadWrite)
	float Progress;

	UPROPERTY(BlueprintReadWrite)
	struct FStageInfo CurrentStageInfo;

	UPROPERTY(BlueprintReadWrite)
	TArray<struct FStageInfo> PastStagesInfo;

	FQuestInfo(class UQuestData* Data = nullptr, ETaskCondition QuestCondition = ETaskCondition::InProcess, bool bIsTracking = false, float ObjectiveProgress = 0.0f)
	{
		QuestData = Data;
		Condition = QuestCondition;
		bIsBeingTracked = bIsTracking;
		Progress = ObjectiveProgress;
	}
};

UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API UQuest : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Activate(class UQuestComponent* QuestComponent);
	virtual void Activate_Implementation(class UQuestComponent* QuestComponent);

	UFUNCTION(BlueprintCallable)
	void SetIsBeingTracked(bool bNewIsBeingTracked);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StagePassed(class UStage* Stage, TSubclassOf<class UStage> NextStageClass);
	virtual void StagePassed_Implementation(class UStage* Stage, TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintPure)
	FQuestInfo GetQuestInfo();

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UQuestComponent* GetOwningQuestComponent() { return OwningQuestComponent; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fail();
	virtual void Fail_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FQuestInfo QuestInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UStage> InitialStageClass;

	UPROPERTY(BlueprintReadOnly)
	class UStage* ActiveStage;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UStage*> PastStages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UQuest> NextQuestIfCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UQuest> NextQuestIfFailed;

private:

	class UQuestComponent* OwningQuestComponent;
	
};
