// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "QuestSystemTypes.h"
#include "Quest.h"
#include "QuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventDelegate, class UQuest*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestsUpdateDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class MYGAMESYSTEM_API UQuestComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UQuestComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Control")
	void AddQuest(TSubclassOf<class UQuest> QuestClass);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestCompleted(class UQuest* Quest);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestFailed(class UQuest* Quest);

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetActiveQuests() const { return ActiveQuests; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetCompletedQuests() const { return CompletedQuests; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetFailedQuests() const { return FailedQuests; }

	UFUNCTION(BlueprintPure, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetAllQuests() const;

	/**
	 * @warning Not synced!!!
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestAdded;

	/**
	 * @warning Not synced!!!
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestCompleted;

	/**
	 * @warning Not synced!!!
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestFailed;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestsUpdateDelegate OnActiveQuestsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestsUpdateDelegate OnCompletedQuestsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestsUpdateDelegate OnFailedQuestsUpdated;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	bool CheckQuestOnDuplication(TSubclassOf<class UQuest> QuestClass) const;

private:

	UFUNCTION(NetMulticast, Reliable)
	void QuestAddedNotify(class UQuest* Quest);

	UFUNCTION(NetMulticast, Reliable)
	void QuestCompletedNotify(class UQuest* Quest);

	UFUNCTION(NetMulticast, Reliable)
	void QuestFailedNotify(class UQuest* Quest);

	UPROPERTY(BlueprintGetter = GetActiveQuests, ReplicatedUsing = OnRep_ActiveQuests)
	TArray<class UQuest*> ActiveQuests;

	UPROPERTY(BlueprintGetter = GetCompletedQuests, ReplicatedUsing = OnRep_CompletedQuests)
	TArray<class UQuest*> CompletedQuests;

	UPROPERTY(BlueprintGetter = GetFailedQuests, ReplicatedUsing = OnRep_FailedQuests)
	TArray<class UQuest*> FailedQuests;

	UFUNCTION()
	void OnRep_ActiveQuests();

	UFUNCTION()
	void OnRep_CompletedQuests();

	UFUNCTION()
	void OnRep_FailedQuests();

};
