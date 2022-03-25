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
	void SelectQuestToTrack(int QuestIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UntrackQuest();

	UFUNCTION(BlueprintCallable)
	void QuestCompleted(class UQuest* Quest);

	UFUNCTION(BlueprintCallable)
	void QuestFailed(class UQuest* Quest);

	UFUNCTION(BlueprintCallable)
	void QuestUpdated(class UQuest* Quest);

	UFUNCTION(BlueprintPure)
	TArray<class UQuest*> GetActiveQuests() { return ActiveQuests; }

	UFUNCTION(BlueprintPure)
	TArray<class UQuest*> GetCompletedQuests() { return CompletedQuests; }

	UFUNCTION(BlueprintPure)
	TArray<class UQuest*> GetFailedQuests() { return FailedQuests; }

	UFUNCTION(BlueprintPure)
	TArray<class UQuest*> GetAllQuests();

	UFUNCTION(BlueprintPure)
	struct FQuestInfo GetTrackedQuestInfo() { return TrackedQuestInfo; }

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void MarkActors(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void UnmarkActors(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToUnmark);

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnQuestAdded;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnQuestCompleted;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnQuestFailed;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnUpdated;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnTrackedQuestSelected;

	UPROPERTY(BlueprintAssignable)
	FQuestEventDelegate OnTrackedQuestUpdated;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnTrackedQuestCompleted;

	UPROPERTY(BlueprintAssignable)
	FQuestInfoEventDelegate OnTrackedQuestFailed;

protected:

	UFUNCTION(BlueprintCallable)
	void UpdateInfos();

	UFUNCTION(BlueprintCallable)
	TArray<struct FQuestInfo> GetQuestsInfo(TArray<class UQuest*> Quests);

	UFUNCTION(BlueprintCallable)
	bool CheckQuestOnDuplication(TSubclassOf<class UQuest> QuestClass);

	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuest*> ActiveQuests;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuest*> CompletedQuests;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuest*> FailedQuests;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ActiveQuestsInfo, BlueprintReadOnly)
	TArray<struct FQuestInfo> ActiveQuestsInfo;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CompletedQuestsInfo, BlueprintReadOnly)
	TArray<struct FQuestInfo> CompletedQuestsInfo;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_FailedQuestsInfo, BlueprintReadOnly)
	TArray<struct FQuestInfo> FailedQuestsInfo;

	UPROPERTY(BlueprintReadOnly)
	class UQuest* TrackedQuest;

	UPROPERTY(BlueprintReadOnly)
	struct FQuestInfo TrackedQuestInfo;

	UFUNCTION(BlueprintCallable)
	void SetTrackedQuest(class UQuest* NewTrackedQuest);

private:

	UFUNCTION(Client, Reliable)
	void TrackedQuestSelected(struct FQuestInfo NewTrackedQuestInfo);

	UFUNCTION(Client, Reliable)
	void TrackedQuestUpdated(struct FQuestInfo NewTrackedQuestInfo);

	UFUNCTION(Client, Reliable)
	void TrackedQuestCompleted(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void TrackedQuestFailed(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void QuestAddedMessage(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void QuestCompletedMessage(struct FQuestInfo QuestInfo);

	UFUNCTION(Client, Reliable)
	void QuestFailedMessage(struct FQuestInfo QuestInfo);

	UFUNCTION()
	void OnRep_ActiveQuestsInfo();

	UFUNCTION()
	void OnRep_CompletedQuestsInfo();

	UFUNCTION()
	void OnRep_FailedQuestsInfo();

	TArray<class AActor*> Markers;
};
