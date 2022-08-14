// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "QuestSystemTypes.h"
#include "Stage.h"
#include "Quest.generated.h"

/**
 * Delegate for observing quest changes.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestConditionDelegate, class UQuest*, Quest);

/**
 * UAdvancedObject that handles stages and describes a quest behavior.
 */
UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEMPLUGIN_API UQuest : public UAdvancedObject
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Activate this quest.
	 * @warning Use this function only if you know what you are doing!
 	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void Activate();
	virtual void Activate_Implementation();

	/**
	 * Track or untrack this quest.
	 * @param bNewIsBeingTracked - Track or not this quest.
	 * @warning Server-only!
 	 */
	UFUNCTION(BlueprintSetter, Category = "Quest|Control")
	void SetIsBeingTracked(bool bNewIsBeingTracked);

	/**
	 * Notify the quest about its active stage pass.
	 * @param Stage - The current stage that has been passed.
	 * @param NextStageClass - The next stage class to run.
	 * @warning Use this function only if you know what you are doing!
 	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal")
	void StagePassed(class UStage* Stage, TSubclassOf<class UStage> NextStageClass);
	virtual void StagePassed_Implementation(class UStage* Stage, TSubclassOf<class UStage> NextStage);

	/**
	 * Get the QuestComponent that owns this quest.
	 * @return The QuestComponent that owns this quest.
	 */
	UFUNCTION(BlueprintGetter, Category = "Quest|OwningQuestComponent")
	class UQuestComponent* GetOwningQuestComponent() const;

	/**
	 * Get the quest condition.
	 * @return The consition of the quest
	 */
	UFUNCTION(BlueprintGetter, Category = "Quest|Info")
	ETaskCondition GetCondition() const { return Condition; }

	/**
	 * Whether or not this quest is being tracked.
	 * @return Is this quest being tracked.
 	 */
	UFUNCTION(BlueprintGetter, Category = "Quest|Info")
	bool GetIsBeingTracked() const { return bIsBeingTracked; }

	/**
	 * Called after the change of the active stage.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnActiveStageChanged;

	/**
	 * Called after the change of the past stages array.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnPastStagesChanged;

	/**
	 * Called after the actvation.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnActivated;

	/**
	 * Called after the completion.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnCompleted;

	/**
	 * Called after the failure.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnFailed;

	/**
	 * Called when tracking starts.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnBecomeTracked;

	/**
	 * Called when tracking ends.
 	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
	FQuestConditionDelegate OnBecomeUntracked;

protected:

	/**
	 * Set the quest condition.
	 * @param NewCondition - A new value of the condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintSetter, Category = "Quest|Condition", meta = (BlueprintProtected))
	void SetCondition(ETaskCondition NewCondition);

	/**
	 * Complete this quest.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal", meta = (BlueprintProtected))
	void Complete();
	virtual void Complete_Implementation();

	/**
	 * Fail this quest.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Internal", meta = (BlueprintProtected))
	void Fail();
	virtual void Fail_Implementation();

	/**
	 * Called after the activation.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control", meta = (BlueprintProtected))
	void OnQuestActivated();
	virtual void OnQuestActivated_Implementation() { return; }

	/**
	 * Called after the completion.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control", meta = (BlueprintProtected))
	void OnQuestCompleted();
	virtual void OnQuestCompleted_Implementation() { return; }

	/**
	 * Called after the failure.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest|Control", meta = (BlueprintProtected))
	void OnQuestFailed();
	virtual void OnQuestFailed_Implementation() { return; }

	/**
	 * Change the active stage.
	 * @param NewStageClass - Class of the new stage.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest|Internal", meta = (BlueprintProtected))
	void ChangeActiveStage(TSubclassOf<class UStage> NewStageClass);

	/**
	 * Get the active stage of this quest.
	 * @return The active stage.
	 */
	UFUNCTION(BlueprintGetter, Category = "Quest|Stages", meta = (BlueprintProtected))
	class UStage* GetActiveStage() const { return ActiveStage; }

	/**
	 * Get the past stages of this quest.
	 * @return The past stages.
	 */
	UFUNCTION(BlueprintGetter, Category = "Quest|Stages", meta = (BlueprintProtected))
	TArray<class UStage*> GetPastStages() const { return PastStages; }

	/**
	 * Class of the initial stage.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Stages", meta = (BlueprintProtected))
	TSubclassOf<class UStage> InitialStageClass;

	/**
	 * Class of the next quest if this completes.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|QuestsFlow", meta = (BlueprintProtected))
	TSubclassOf<class UQuest> NextQuestIfCompleted;

	/**
	 * Class of the next quest if this fails.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|QuestsFlow", meta = (BlueprintProtected))
	TSubclassOf<class UQuest> NextQuestIfFailed;

private:

	/**
	 * The active stage of this quest.
	 */
	UPROPERTY(BlueprintGetter = GetActiveStage, ReplicatedUsing = OnRep_ActiveStage)
	class UStage* ActiveStage;

	/**
	 * On replicated event of ActiveStage.
	 */
	UFUNCTION()
	void OnRep_ActiveStage();

	/**
	 * Broadcast change of ActiveStage.
	 */
	UFUNCTION()
	void BroadcastChange_ActiveStage();

	/**
	 * The past stages of this quest.
	 */
	UPROPERTY(BlueprintGetter = GetPastStages, ReplicatedUsing = OnRep_PastStages)
	TArray<class UStage*> PastStages;

	/**
	 * On replicated event of PastStages.
	 */
	UFUNCTION()
	void OnRep_PastStages();

	/**
	 * Broadcast change of PastStages.
	 */
	UFUNCTION()
	void BroadcastChange_PastStages();

	/**
	 * Condition of this Quest.
	 */
	UPROPERTY(BlueprintGetter = GetCondition, BlueprintSetter = SetCondition, ReplicatedUsing = OnRep_Condition)
	ETaskCondition Condition = ETaskCondition::Aborted;

	/**
	 * On replicated event of Condition.
	 */
	UFUNCTION()
	void OnRep_Condition();

	/**
	 * Broadcast change of Condition.
	 */
	UFUNCTION()
	void BroadcastChange_Condition();

	/*
	TMap<ETaskCondition, FQuestConditionDelegate> ConditionToDelegateMap =
	{
		{ETaskCondition::InProcess, OnActivated},
		{ETaskCondition::Completed, OnCompleted},
		{ETaskCondition::Failed, OnFailed}
	};
	*/

	/**
	 * Is this quest being tracked.
	 */
	UPROPERTY(BlueprintGetter = GetIsBeingTracked, BlueprintSetter = SetIsBeingTracked, ReplicatedUsing = OnRep_IsBeingTracked)
	bool bIsBeingTracked = false;

	/**
	 * On replicated event of bIsBeingTracked.
	 */
	UFUNCTION()
	void OnRep_IsBeingTracked();

	/**
	 * Broadcast OnBecomeTracked/Untracked delegate.
	 */
	UFUNCTION()
	void BroadcastChange_IsBeingTracked();
	
};
