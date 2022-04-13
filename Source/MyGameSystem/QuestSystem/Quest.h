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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UQuestData* QuestData;

	UPROPERTY(BlueprintReadWrite)
	ETaskCondition Condition;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingTracked;

	UPROPERTY(BlueprintReadWrite)
	struct FStageInfo CurrentStageInfo;

	UPROPERTY(BlueprintReadWrite)
	TArray<struct FStageInfo> PastStageInfos;

	FQuestInfo(class UQuestData* Data = nullptr, ETaskCondition QuestCondition = ETaskCondition::InProcess, bool bIsTracking = false, struct FStageInfo CurrentStage = FStageInfo(), TArray<struct FStageInfo> PastStages = TArray<struct FStageInfo>())
	{
		QuestData = Data;
		Condition = QuestCondition;
		bIsBeingTracked = bIsTracking;
		CurrentStageInfo = CurrentStage;
		PastStageInfos = PastStages;
	}

	friend bool operator == (const FQuestInfo& Info1, const FQuestInfo& Info2)
	{
		return 	Info1.QuestData == Info2.QuestData &&
				Info1.Condition == Info2.Condition &&
				Info1.bIsBeingTracked == Info2.bIsBeingTracked &&
				Info1.CurrentStageInfo == Info2.CurrentStageInfo &&
				Info1.PastStageInfos == Info2.PastStageInfos;
	}

	friend bool operator != (const FQuestInfo& Info1, const FQuestInfo& Info2)
	{
		return !(Info1 == Info2);
	}
};

UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API UQuest : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void Activate(class UQuestComponent* QuestComponent);
	virtual void Activate_Implementation(class UQuestComponent* QuestComponent);

	UFUNCTION(BlueprintCallable, Category = "Quest|Internal")
	void SetIsBeingTracked(bool bNewIsBeingTracked);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void StagePassed(class UStage* Stage, TSubclassOf<class UStage> NextStageClass);
	virtual void StagePassed_Implementation(class UStage* Stage, TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintPure, Category = "Quest|Info")
	FQuestInfo GetQuestInfo() const;

	UFUNCTION(BlueprintPure, Category = "Quest|OwningQuestComponent")
	FORCEINLINE class UQuestComponent* GetOwningQuestComponent() const { return OwningQuestComponent; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void Fail();
	virtual void Fail_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control")
	void OnQuestActivated();
	virtual void OnQuestActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control")
	void OnQuestCompleted();
	virtual void OnQuestCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control")
	void OnQuestFailed();
	virtual void OnQuestFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control")
	void OnQuestUpdated();
	virtual void OnQuestUpdated_Implementation() { return; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Info")
	UQuestData* QuestData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Stages")
	TSubclassOf<class UStage> InitialStageClass;

	UPROPERTY(BlueprintReadOnly, Category = "Quest|Stages")
	class UStage* ActiveStage;

	UPROPERTY(BlueprintReadOnly, Category = "Quest|Stages")
	TArray<class UStage*> PastStages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|QuestsFlow")
	TSubclassOf<class UQuest> NextQuestIfCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|QuestsFlow")
	TSubclassOf<class UQuest> NextQuestIfFailed;

private:

	class UQuestComponent* OwningQuestComponent;

	ETaskCondition Condition = ETaskCondition::Aborted;
	bool bIsBeingTracked = false;
	
};
