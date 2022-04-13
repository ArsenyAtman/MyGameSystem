// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestSystemTypes.h"
#include "Quest.h"
#include "QuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestEventDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestInfoEventDelegate, FQuestInfo, QuestInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class MYGAMESYSTEM_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Control")
	void AddQuest(TSubclassOf<class UQuest> QuestClass);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "QuestComponent|Control")
	void SelectQuestToTrackByIndex(int QuestIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "QuestComponent|Control")
	void SelectQuestToTrackByInfo(FQuestInfo QuestInfo);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "QuestComponent|Control")
	void UntrackQuest();

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestCompleted(class UQuest* Quest);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestFailed(class UQuest* Quest);

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void QuestUpdated(class UQuest* Quest);

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetActiveQuests() const { return ActiveQuests; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetCompletedQuests() const { return CompletedQuests; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetFailedQuests() const { return FailedQuests; }

	UFUNCTION(BlueprintPure, Category = "QuestComponent|Quests")
	TArray<class UQuest*> GetAllQuests() const;

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<struct FQuestInfo> GetActiveQuestsInfo() const { return ActiveQuestsInfo; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<struct FQuestInfo> GetCompletedQuestsInfo() const { return CompletedQuestsInfo; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	TArray<struct FQuestInfo> GetFailedQuestsInfo() const { return FailedQuestsInfo; }

	UFUNCTION(BlueprintPure, Category = "QuestComponent|Quests")
	bool HasTrackedQuest() const;

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	class UQuest* GetTrackedQuest() const { return TrackedQuest; }

	UFUNCTION(BlueprintGetter, Category = "QuestComponent|Quests")
	struct FQuestInfo GetTrackedQuestInfo() const { return TrackedQuestInfo; }

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestInfoEventDelegate OnQuestAdded;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestInfoEventDelegate OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestInfoEventDelegate OnQuestFailed;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnQuestsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnTrackedQuestSelected;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnTrackedQuestDeselected;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestEventDelegate OnTrackedQuestUpdated;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestInfoEventDelegate OnTrackedQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "QuestComponent|Delegates")
	FQuestInfoEventDelegate OnTrackedQuestFailed;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void UpdateAllQuestsInfo();

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	TArray<struct FQuestInfo> GetQuestsInfo(const TArray<class UQuest*>& Quests) const;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	bool CheckQuestOnDuplication(TSubclassOf<class UQuest> QuestClass) const;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent|Internal")
	void SetTrackedQuest(class UQuest* NewTrackedQuest);

private:

	UFUNCTION(Client, Reliable)
	void TrackedQuestSelectedNotify(struct FQuestInfo NewTrackedQuestInfo);

	UFUNCTION(Client, Reliable)
	void TrackedQuestDeselectedNotify();

	UFUNCTION(Client, Reliable)
	void TrackedQuestUpdatedNotify(struct FQuestInfo NewTrackedQuestInfo);

	UFUNCTION(Client, Reliable)
	void TrackedQuestCompletedNotify(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void TrackedQuestFailedNotify(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void QuestAddedNotify(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void QuestCompletedNotify(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void QuestFailedNotify(struct FQuestInfo QuestInfo);

	UFUNCTION()
	void OnRep_ActiveQuestsInfo();

	UFUNCTION()
	void OnRep_CompletedQuestsInfo();

	UFUNCTION()
	void OnRep_FailedQuestsInfo();

	UPROPERTY(BlueprintGetter = GetActiveQuests)
	TArray<class UQuest*> ActiveQuests;

	UPROPERTY(BlueprintGetter = GetCompletedQuests)
	TArray<class UQuest*> CompletedQuests;

	UPROPERTY(BlueprintGetter = GetFailedQuests)
	TArray<class UQuest*> FailedQuests;

	UPROPERTY(BlueprintGetter = GetActiveQuestsInfo, Replicated, ReplicatedUsing = OnRep_ActiveQuestsInfo)
	TArray<struct FQuestInfo> ActiveQuestsInfo;

	UPROPERTY(BlueprintGetter = GetCompletedQuestsInfo, Replicated, ReplicatedUsing = OnRep_CompletedQuestsInfo)
	TArray<struct FQuestInfo> CompletedQuestsInfo;

	UPROPERTY(BlueprintGetter = GetFailedQuestsInfo, Replicated, ReplicatedUsing = OnRep_FailedQuestsInfo)
	TArray<struct FQuestInfo> FailedQuestsInfo;

	UPROPERTY(BlueprintGetter = GetTrackedQuest)
	class UQuest* TrackedQuest;

	UPROPERTY(BlueprintGetter = GetTrackedQuestInfo)
	struct FQuestInfo TrackedQuestInfo;

	UPROPERTY(BlueprintGetter = HasTrackedQuest)
	bool bHasTrackedQuest = false;
};
