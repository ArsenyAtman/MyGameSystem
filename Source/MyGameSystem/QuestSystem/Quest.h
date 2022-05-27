// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "QuestSystemTypes.h"
#include "Stage.h"
#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestConditionDelegate, class UQuest*, Quest);

UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API UQuest : public UAdvancedObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void Activate();
	virtual void Activate_Implementation();

	UFUNCTION(BlueprintSetter, Category = "Quest|Internal")
	void SetIsBeingTracked(bool bNewIsBeingTracked);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void StagePassed(class UStage* Stage, TSubclassOf<class UStage> NextStageClass);
	virtual void StagePassed_Implementation(class UStage* Stage, TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintGetter, Category = "Quest|OwningQuestComponent")
	class UQuestComponent* GetOwningQuestComponent() const;

	UFUNCTION(BlueprintGetter, Category = "Quest|Info")
	ETaskCondition GetCondition() const { return Condition; }

	UFUNCTION(BlueprintGetter, Category = "Quest|Info")
	bool GetIsBeingTracked() const { return bIsBeingTracked; }

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnActiveStageChanged;

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnPastStagesChanged;

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnActivated;

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnFailed;

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnBecomeTracked;

	UPROPERTY(BlueprintAssignable)
	FQuestConditionDelegate OnBecomeUntracked;

protected:

	UFUNCTION(BlueprintSetter)
	void SetCondition(ETaskCondition NewCondition);

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

	UFUNCTION(BlueprintCallable)
	void ChangeActiveStage(TSubclassOf<class UStage> NewStageClass);

	UFUNCTION(BlueprintGetter)
	class UStage* GetActiveStage() const { return ActiveStage; }

	UFUNCTION(BlueprintGetter)
	TArray<class UStage*> GetPastStages() const { return PastStages; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Stages")
	TSubclassOf<class UStage> InitialStageClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|QuestsFlow")
	TSubclassOf<class UQuest> NextQuestIfCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|QuestsFlow")
	TSubclassOf<class UQuest> NextQuestIfFailed;

private:

	// Event on condition
	UPROPERTY(BlueprintGetter = GetActiveStage, ReplicatedUsing = OnRep_ActiveStage, Category = "Quest|Stages")
	class UStage* ActiveStage;

	UFUNCTION()
	void OnRep_ActiveStage();

	UFUNCTION()
	void BroadcastChange_ActiveStage();

	// Event on condition
	UPROPERTY(BlueprintGetter = GetPastStages, ReplicatedUsing = OnRep_PastStages, Category = "Quest|Stages")
	TArray<class UStage*> PastStages;

	UFUNCTION()
	void OnRep_PastStages();

	UFUNCTION()
	void BroadcastChange_PastStages();

	UPROPERTY(BlueprintGetter = GetCondition, BlueprintSetter = SetCondition, ReplicatedUsing = OnRep_Condition)
	ETaskCondition Condition = ETaskCondition::Aborted;

	UFUNCTION()
	void OnRep_Condition();

	UFUNCTION()
	void BroadcastChange_Condition();

	TMap<ETaskCondition, FQuestConditionDelegate> ConditionToDelegateMap =
	{
		{ETaskCondition::InProcess, OnActivated},
		{ETaskCondition::Completed, OnCompleted},
		{ETaskCondition::Failed, OnFailed}
	};

	UPROPERTY(BlueprintGetter = GetIsBeingTracked, BlueprintSetter = SetIsBeingTracked, ReplicatedUsing = OnRep_IsBeingTracked)
	bool bIsBeingTracked = false;

	UFUNCTION()
	void OnRep_IsBeingTracked();

	UFUNCTION()
	void BroadcastChange_IsBeingTracked();
	
};
