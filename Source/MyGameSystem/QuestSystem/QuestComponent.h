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

	UFUNCTION(BlueprintCallable)
	void AddQuest(TSubclassOf<class UQuest> QuestClass);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SelectQuestToTrackByIndex(int QuestIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SelectQuestToTrackByInfo(FQuestInfo QuestInfo);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UntrackQuest();

	UFUNCTION(BlueprintCallable)
	void QuestCompleted(class UQuest* Quest);

	UFUNCTION(BlueprintCallable)
	void QuestFailed(class UQuest* Quest);

	UFUNCTION(BlueprintCallable)
	void QuestUpdated(class UQuest* Quest);

	UFUNCTION(BlueprintGetter)
	TArray<class UQuest*> GetActiveQuests() { return ActiveQuests; }

	UFUNCTION(BlueprintGetter)
	TArray<class UQuest*> GetCompletedQuests() { return CompletedQuests; }

	UFUNCTION(BlueprintGetter)
	TArray<class UQuest*> GetFailedQuests() { return FailedQuests; }

	UFUNCTION(BlueprintPure)
	TArray<class UQuest*> GetAllQuests();

	UFUNCTION(BlueprintGetter)
	TArray<struct FQuestInfo> GetActiveQuestsInfo() { return ActiveQuestsInfo; }

	UFUNCTION(BlueprintGetter)
	TArray<struct FQuestInfo> GetCompletedQuestsInfo() { return CompletedQuestsInfo; }

	UFUNCTION(BlueprintGetter)
	TArray<struct FQuestInfo> GetFailedQuestsInfo() { return FailedQuestsInfo; }

	UFUNCTION(BlueprintPure)
	bool HasTrackedQuest();

	UFUNCTION(BlueprintGetter)
	class UQuest* GetTrackedQuest() { return TrackedQuest; }

	UFUNCTION(BlueprintGetter)
	struct FQuestInfo GetTrackedQuestInfo() { return TrackedQuestInfo; }

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void MarkActors(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark); // MarkerComponent

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void UnmarkActors(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToUnmark); // MarkerComponent

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnQuestAdded;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnQuestCompleted;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnQuestFailed;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnQuestsUpdated;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnTrackedQuestSelected;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnTrackedQuestDeselected;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnTrackedQuestUpdated;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnTrackedQuestCompleted;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnTrackedQuestFailed;

protected:

	UFUNCTION(BlueprintCallable)
	void UpdateAllQuestsInfo();

	UFUNCTION(BlueprintCallable)
	TArray<struct FQuestInfo> GetQuestsInfo(TArray<class UQuest*> Quests);

	UFUNCTION(BlueprintCallable)
	bool CheckQuestOnDuplication(TSubclassOf<class UQuest> QuestClass);

	UFUNCTION(BlueprintCallable)
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

	TArray<class AActor*> Markers;

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
