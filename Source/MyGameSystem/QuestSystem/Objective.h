// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "QuestSystemTypes.h"
#include "Objective.generated.h"

/**
 * Delegate for handling changes of an objective.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveConditionDelegate, class UObjective*, Objective);

/**
 * UAdvancedObject for a quest objective representation.
 */
UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API UObjective : public UAdvancedObject
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Activate this objective.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Activate();
	virtual void Activate_Implementation();

	/**
	 * Abort this objective
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Abort();
	virtual void Abort_Implementation();

	/**
	 * Spawn markers of this objective.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Mark();

	/**
	 * Despawn markers of this objective.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Unmark();

	/**
	 * Get the stage that owns this objective.
	 * @return The owning stage.
	 */
	UFUNCTION(BlueprintGetter, Category = "Objective|OwningStage")
	class UStage* GetOwningStage() const;

	/**
	 * Get the condition of this objective.
	 * @return The condition of this objective.
	 */
	UFUNCTION(BlueprintGetter, Category = "Objective|Condition")
	ETaskCondition GetCondition() const { return Condition; }

	/**
	 * Whether this objective is optional.
	 * @return Is this condition optional.
	 */
	UFUNCTION(BlueprintGetter, Category = "Objective|IsOptional")
	bool GetIsOptional() const { return bIsOptional; }

	/**
	 * Called after the activation.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnActivated;

	/**
	 * Called after the completion.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnCompleted;

	/**
	 * Called after the failure.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnFailed;

	/**
	 * Called after the interruption.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnAborted;

	/**
	 * Called after updates.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnUpdated;

protected:

	/**
	 * Set the condition of this objective.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintSetter, Category = "Objective|Condition", meta = (BlueprintProtected))
	void SetCondition(ETaskCondition NewCondition);

	/**
	 * Class of the markers manager.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|MarkersManager", meta = (BlueprintProtected))
	class TSubclassOf<class UMarkersManagerComponent> MarkersManagerComponentClass;

	/**
	 * Complete the objective.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void Complete();
	virtual void Complete_Implementation();

	/**
	 * Fail the objective.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void Fail();
	virtual void Fail_Implementation();

	/**
	 * Update the objective
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void Update();
	virtual void Update_Implementation();

	/**
	 * Cleanup.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal", meta = (BlueprintProtected))
	void EndObjective();
	virtual void EndObjective_Implementation();

	/**
	 * Called after the activation.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveActivated();
	virtual void OnObjectiveActivated_Implementation() { return; }

	/**
	 * Called after the abortion.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveAborted();
	virtual void OnObjectiveAborted_Implementation() { return; }

	/**
	 * Called after the completion.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveCompleted();
	virtual void OnObjectiveCompleted_Implementation() { return; }

	/**
	 * Called after the failure.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveFailed();
	virtual void OnObjectiveFailed_Implementation() { return; }

	/**
	 * Filter actors for marking as objectives.
	 * @param ActorsToMark - Source list of actors to filter.
	 * @return Filtered list of actors.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|RelatedActors", meta = (BlueprintProtected))
	TArray<class AActor*> FilterActorsForMarking(const TArray<class AActor*>& ActorsToMark) const;
	virtual TArray<class AActor*> FilterActorsForMarking_Implementation(const TArray<class AActor*>& ActorsToMark) const { return ActorsToMark; }

	/**
	 * Get related actors for this objective.
	 * @return References for this objective.
	 */
	UFUNCTION(BlueprintGetter, Category = "Objective|RelatedActors", meta = (BlueprintProtected))
	FReferencesForQuest GetReferencesForQuest() const { return ReferencesForQuest; }

private:

	/**
	 * Is this objective optional.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetIsOptional, Category = "Objective|IsOptional", meta = (AllowPrivateAccess = true, BlueprintProtected))
	bool bIsOptional = false;

	// Get all related actors to this objective.
	FReferencesForQuest FindReferencesForQuest() const;

	// Instance a markers manager.
	class UMarkersManagerComponent* CreateMarkersManager() const;

	/**
	 * Condition of the objective
	 */
	UPROPERTY(BlueprintGetter = GetCondition, BlueprintSetter = SetCondition, ReplicatedUsing = OnRep_Condition)
	ETaskCondition Condition = ETaskCondition::Aborted;

	/*
	TMap<ETaskCondition, FObjectiveConditionDelegate> ConditionToDelegateMap =
	{
		{ETaskCondition::InProcess, OnActivated},
		{ETaskCondition::Completed, OnCompleted},
		{ETaskCondition::Failed, OnFailed},
		{ETaskCondition::Aborted, OnAborted}
	};
	*/

	/**
	 * Related actors to this objective.
	 */
	UPROPERTY(BlueprintGetter = GetReferencesForQuest, Replicated)
	FReferencesForQuest ReferencesForQuest;

	// MarkersManager object.
	UPROPERTY()
	class UMarkersManagerComponent* MarkersManager = nullptr;

	// On replicated event for Condition.
	UFUNCTION()
	void OnRep_Condition();

	// Broadcast related delegate to the current condition.
	UFUNCTION()
	void BroadcastChange_Condition();

	// Notify all clients about the update.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_OnUpdated();
};
