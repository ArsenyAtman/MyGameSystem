// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "QuestSystemTypes.h"
#include "Objective.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveConditionDelegate, class UObjective*, Objective);

UCLASS(Blueprintable, BlueprintType)
class MYGAMESYSTEM_API UObjective : public UAdvancedObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Activate();
	virtual void Activate_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Abort();
	virtual void Abort_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Mark();

	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Unmark();

	UFUNCTION(BlueprintGetter, Category = "Objective|OwningStage")
	class UStage* GetOwningStage() const;

	UFUNCTION(BlueprintGetter, Category = "Objective|Condition")
	ETaskCondition GetCondition() const { return Condition; }

	UFUNCTION(BlueprintGetter, Category = "Objective|IsOptional")
	bool GetIsOptional() const { return bIsOptional; }

	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnActivated;

	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnFailed;

	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnAborted;

	UPROPERTY(BlueprintAssignable, Category = "Objective|Delegates")
	FObjectiveConditionDelegate OnUpdated;

protected:

	UFUNCTION(BlueprintSetter, Category = "Objective|Condition", meta = (BlueprintProtected))
	void SetCondition(ETaskCondition NewCondition);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|MarkersManager", meta = (BlueprintProtected))
	class TSubclassOf<class UMarkersManagerComponent> MarkersManagerComponentClass;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetIsOptional, Category = "Objective|IsOptional", meta = (BlueprintProtected))
	bool bIsOptional = false;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void Fail();
	virtual void Fail_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal", meta = (BlueprintProtected))
	void EndObjective();
	virtual void EndObjective_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveActivated();
	virtual void OnObjectiveActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveAborted();
	virtual void OnObjectiveAborted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveCompleted();
	virtual void OnObjectiveCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control", meta = (BlueprintProtected))
	void OnObjectiveFailed();
	virtual void OnObjectiveFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|RelatedActors", meta = (BlueprintProtected))
	TArray<class AActor*> FilterActorsForMarking(const TArray<class AActor*>& ActorsToMark) const;
	virtual TArray<class AActor*> FilterActorsForMarking_Implementation(const TArray<class AActor*>& ActorsToMark) const { return ActorsToMark; }

	UFUNCTION(BlueprintGetter, Category = "Objective|RelatedActors", meta = (BlueprintProtected))
	FReferencesForQuest GetReferencesForQuest() const { return ReferencesForQuest; }

private:

	FReferencesForQuest FindReferencesForQuest() const;

	class UMarkersManagerComponent* CreateMarkersManager() const;

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

	UPROPERTY(BlueprintGetter = GetReferencesForQuest, Replicated)
	FReferencesForQuest ReferencesForQuest;

	UPROPERTY()
	class UMarkersManagerComponent* MarkersManager = nullptr;

	UFUNCTION()
	void OnRep_Condition();

	UFUNCTION()
	void BroadcastChange_Condition();

	UFUNCTION(NetMulticast, Reliable)
	void Notify_OnUpdated();
};
