// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "QuestSystemTypes.h"
#include "Quest.h"
#include "QuestComponent.generated.h"

/**
 * Delegate for handling changes related to some quest.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventDelegate, class UQuest*, Quest);

/**
 * Delegate for handling quest groups changes.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestsUpdateDelegate);

/**
 * ActorComponent that handles quests of it's actor.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class MYGAMESYSTEMPLUGIN_API UQuestComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UQuestComponent();

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Add a new quest.
	 * @param QuestClass - Class of the new quest.
	 * @param bCheckOnDuplication - Check the new quest class on duplication and don't add if it has already been added.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Control")
	void AddQuestOfClass(TSubclassOf<class UQuest> QuestClass, bool bCheckOnDuplication = true);

	/**
	 * Add a new quest.
	 * @param Quest - The new quest object with this quest component as outer.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Control")
	void AddQuestObject(class UQuest* Quest);

	/**
	 * Notify the component about it's quest completion.
	 * @param Quest - The quest that ends.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestCompleted(class UQuest* Quest);

	/**
	 * Notify the component about it's quest failure.
	 * @param Quest - The quest that ends.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestFailed(class UQuest* Quest);

	/**
	 * Get the active quests of this component.
	 * @return The active quests.
	 */
	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetActiveQuests() const { return ActiveQuests; }

	/**
	 * Get the completed quests of this component.
	 * @return The completed quests.
	 */
	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetCompletedQuests() const { return CompletedQuests; }

	/**
	 * Get the failed quests of this component.
	 * @return The failed quests.
	 */
	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetFailedQuests() const { return FailedQuests; }

	/**
	 * Get all quests of this component.
	 * @return All quests.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetAllQuests() const;

	/**
	 * Called when a new quest is added.
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestAdded;

	/**
	 * Called when an active quest is completed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestCompleted;

	/**
	 * Called when an active quest is failed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestFailed;

protected:

	// BeginPlay override.
	virtual void BeginPlay() override;

	/**
	 * Is a quest already being processed by this component.
	 * @param QuestClass - The quest class to check.
	 * @return True is the specified quest was not assigned to this component.
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal", meta = (BlueprintProtected))
	bool CheckQuestOnDuplication(TSubclassOf<class UQuest> QuestClass) const;

private:

	/**
	 * The active quests of this component.
	 */
	UPROPERTY(BlueprintGetter = GetActiveQuests, ReplicatedUsing = OnRep_ActiveQuests)
	TArray<class UQuest*> ActiveQuests;

	/**
	 * The completed quests of this component
	 */
	UPROPERTY(BlueprintGetter = GetCompletedQuests, ReplicatedUsing = OnRep_CompletedQuests)
	TArray<class UQuest*> CompletedQuests;

	/**
	 * The failed quests of this component.
	 */
	UPROPERTY(BlueprintGetter = GetFailedQuests, ReplicatedUsing = OnRep_FailedQuests)
	TArray<class UQuest*> FailedQuests;

	/**
	 * On replicated event of ActiveQuests.
	 * @param PreReplicationActiveQuests - Array before replication.
	 */
	UFUNCTION()
	void OnRep_ActiveQuests(const TArray<class UQuest*>& PreReplicationActiveQuests);

	/**
	 * On replicated event of CompletedQuests.
	 * @param PreReplicationCompletedQuests - Array before replication.
	 */
	UFUNCTION()
	void OnRep_CompletedQuests(const TArray<class UQuest*>& PreReplicationCompletedQuests);

	/**
	 * On replicated event of FailedQuests.
	 * @param PreReplicationFailedQuests - Array before replication.
	 */
	UFUNCTION()
	void OnRep_FailedQuests(const TArray<class UQuest*>& PreReplicationFailedQuests);

	/**
	 * Broadcast delegate OnQuestAdded.
	 * @param NewQuest - Quest that adds.
	 */
	UFUNCTION()
	void BroadcastChange_QuestAdded(class UQuest* NewQuest);

	/**
	 * Broadcast delegate OnQuestCompleted.
	 * @param Quest - Quest that completes.
	 */
	UFUNCTION()
	void BroadcastChange_QuestCompleted(class UQuest* Quest);

	/**
	 * Broadcast delegate OnQuestFailed.
	 * @param Quest - Quest that fails.
	 */
	UFUNCTION()
	void BroadcastChange_QuestFailed(class UQuest* Quest);

	/**
	 * Find the difference between two arrays and execute a function for each member of the difference.
	 * @param Array1 - The first array.
	 * @param Array2 - The second array.
	 * @param Function - The function pointer.
	 */
	using BroadcastChangeFunction = void (UQuestComponent::*)(class UQuest*);
	void PerformFunctionForArrayDiff(const TArray<class UQuest*>& Array1, const TArray<class UQuest*>& Array2, BroadcastChangeFunction Function);

};
