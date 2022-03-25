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

USTRUCT(Blueprintable, BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(BlueprintReadWrite)
	ETaskCondition Condition;

	UPROPERTY(BlueprintReadOnly)
	bool bIsBeingTracked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType Type;

	UPROPERTY(BlueprintReadWrite)
	float Progress;

	UPROPERTY(BlueprintReadWrite)
	struct FStageInfo CurrentStageInfo;

	UPROPERTY(BlueprintReadWrite)
	TArray<struct FStageInfo> PastStagesInfo;

	FQuestInfo(FText ObjectiveName = FText::FromString("None"), FText ObjectiveDescription = FText::FromString("None"), EQuestType QuestType = EQuestType::Usual, float ObjectiveProgress = 0.0f)
	{
		Name = ObjectiveName;
		Description = ObjectiveDescription;
		Condition = ETaskCondition::InProcess;
		bIsBeingTracked = false;
		Type = QuestType;
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
	FQuestInfo GetQuestInfo(); // was FORCEINLINE

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
