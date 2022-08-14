// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "QuestSystemTypes.h"
#include "Objective.h"
#include "Stage.generated.h"

/**
 * Delegate for handling changes of a stage.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStageConditionDelegate, class UStage*, Stage);

/**
 * Object for objectives handling.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class MYGAMESYSTEMPLUGIN_API UStage : public UAdvancedObject
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Activate this stage.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Activate();
	virtual void Activate_Implementation();

	/**
	 * Notify the stage about its objective completion.
	 * @param Objective - Objective that completes.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ObjectiveCompleted(class UObjective* Objective);
	virtual void ObjectiveCompleted_Implementation(class UObjective* Objective);

	/**
	 * Notify the stage about its objective failure.
	 * @param Objective - Objective that fails.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ObjectiveFailed(class UObjective* Objective);
	virtual void ObjectiveFailed_Implementation(class UObjective* Objective);

	/**
	 * Mark objectives of the stage.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Stage|Internal")
	void MarkObjectives();

	/**
	 * Unmark objectives of the stage.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Stage|Internal")
	void UnmarkObjectives();

	/**
	 * Get the quest that owns this stage.
	 * @return The owning quest.
	 */
	UFUNCTION(BlueprintGetter, Category = "Stage|OwningQuest")
	class UQuest* GetOwningQuest() const;

	/**
	 * Get the conditon of this stage.
	 * @return The condition of this stage.
	 */
	UFUNCTION(BlueprintGetter, Category = "Stage|Condition")
	ETaskCondition GetCondition() const { return Condition; }

	/**
	 * Called after the activation.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stage|Delegates")
	FStageConditionDelegate OnActivated;

	/**
	 * Called after the completion.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stage|Delegates")
	FStageConditionDelegate OnCompleted;

	/**
	 * Called after the failure.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stage|Delegates")
	FStageConditionDelegate OnFailed;

protected:

	/**
	 * Set the current condition of the stage.
	 * @param NewCondition - A new value of the condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintSetter, Category = "Stage|Condition", meta = (BlueprintProtected))
	void SetCondition(ETaskCondition NewCondition);

	/**
	 * Activate objectives of this stage.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void ActivateObjectives();
	virtual void ActivateObjectives_Implementation() { return; }

	/**
	 * Check the state of the stage and determine next actions and behaviors.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void CheckCondition();
	virtual void CheckCondition_Implementation() { return; }

	/**
	 * Complete this stage.
	 * @param NextStage - Class of the next stage to run.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void Complete(TSubclassOf<class UStage> NextStage);
	virtual void Complete_Implementation(TSubclassOf<class UStage> NextStage);

	/**
	 * Fail this stage.
	 * @param NextStage - Class of the next stage to run.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void Fail(TSubclassOf<class UStage> NextStage);
	virtual void Fail_Implementation(TSubclassOf<class UStage> NextStage);

	/**
	 * Called after the activation.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void OnStageActivated();
	virtual void OnStageActivated_Implementation() { return; }

	/**
	 * Called after the completion.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void OnStageCompleted();
	virtual void OnStageCompleted_Implementation() { return; }

	/**
	 * Called after the failure.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void OnStageFailed();
	virtual void OnStageFailed_Implementation() { return; }

	/**
	 * Called after the abortion.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void AbortAllObjectives();
	virtual void AbortAllObjectives_Implementation();

	/**
	 * Get all objectives of the stage.
	 * @return All objectives of this stage.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Objectives", meta = (BlueprintProtected))
	TArray<class UObjective*> GetStageObjectives() const;
	virtual TArray<class UObjective*> GetStageObjectives_Implementation() const { return TArray<class UObjective*>(); }

	/**
	 * Check every objective on completion.
	 * @param Objectives - Objectives to check.
	 * @return Check result.
	 */
	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsAllObjectivesCompleted(const TArray<class UObjective*>& Objectives) const;

	/**
	 * Check every objective on failure.
	 * @param Objectives - Objectives to check.
	 * @return Check result.
	 */
	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsAllObjectivesFailed(const TArray<class UObjective*>& Objectives) const;

	/**
	 * Whether one of the objectives is completed.
	 * @param Objectives - Objectives to check.
	 * @return Check result.
	 */
	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsOneObjectiveCompleted(const TArray<class UObjective*>& Objectives) const;

	/**
	 * Whether one of the objectives is failed.
	 * @param Objectives - Objectives to check.
	 * @return Check result.
	 */
	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsOneObjectiveFailed(const TArray<class UObjective*>& Objectives) const;

private:

	/**
	 * Сondition of the stage.
	 */
	UPROPERTY(BlueprintGetter = GetCondition, BlueprintSetter = SetCondition, Replicated = OnRep_Condition)
	ETaskCondition Condition = ETaskCondition::Aborted;

	/*
	TMap<ETaskCondition, FStageConditionDelegate> ConditionToDelegateMap =
	{
		{ETaskCondition::InProcess, OnActivated},
		{ETaskCondition::Completed, OnCompleted},
		{ETaskCondition::Failed, OnFailed}
	};
	*/

	// On replicated event for Condition.
	UFUNCTION()
	void OnRep_Condition();

	// Broadcast delegate that related to the current condition.
	UFUNCTION()
	void BroadcastChange_Condition();
	
};
