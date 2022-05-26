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
	void Activate(class UStage* RelatedStage);
	virtual void Activate_Implementation(class UStage* RelatedStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void Abort();
	virtual void Abort_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Mark();

	UFUNCTION(BlueprintCallable, Category = "Objective|Internal")
	void Unmark();

	UFUNCTION(BlueprintGetter, Category = "Objective|OwningStage")
	class UStage* GetOwningStage() const;

	UFUNCTION(BlueprintGetter)
	ETaskCondition GetCondition() const { return Condition; }

	UFUNCTION(BlueprintGetter)
	bool GetIsOptional() const { return bIsOptional; }

	UPROPERTY(BlueprintAssignable)
	FObjectiveConditionDelegate OnActivated;

	UPROPERTY(BlueprintAssignable)
	FObjectiveConditionDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FObjectiveConditionDelegate OnFailed;

	UPROPERTY(BlueprintAssignable)
	FObjectiveConditionDelegate OnAborted;

	UPROPERTY(BlueprintAssignable)
	FObjectiveConditionDelegate OnUpdated;

protected:

	UFUNCTION(BlueprintSetter)
	void SetCondition(ETaskCondition NewCondition);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective|MarkersManager")
	class TSubclassOf<class UMarkersManagerComponent> MarkersManagerComponentClass;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetIsOptional)
	bool bIsOptional = false;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void Complete();
	virtual void Complete_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void Fail();
	virtual void Fail_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Internal")
	void EndObjective();
	virtual void EndObjective_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveActivated();
	virtual void OnObjectiveActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveAborted();
	virtual void OnObjectiveAborted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveCompleted();
	virtual void OnObjectiveCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|Control")
	void OnObjectiveFailed();
	virtual void OnObjectiveFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective|RelatedActors")
	TArray<class AActor*> FilterActorsForMarking(const TArray<class AActor*>& ActorsToMark) const;
	virtual TArray<class AActor*> FilterActorsForMarking_Implementation(const TArray<class AActor*>& ActorsToMark) const { return ActorsToMark; }

	UFUNCTION(BlueprintGetter, Category = "Objective|RelatedActors")
	FReferencesForQuest GetReferencesForQuest() const { return ReferencesForQuest; }

private:

	FReferencesForQuest FindReferencesForQuest() const;

	class UMarkersManagerComponent* CreateMarkersManager() const;

	UPROPERTY(BlueprintGetter = GetCondition, BlueprintSetter = SetCondition, ReplicatedUsing = OnRep_Condition)
	ETaskCondition Condition = ETaskCondition::Aborted;

	UPROPERTY(BlueprintGetter = GetReferencesForQuest, Replicated)
	FReferencesForQuest ReferencesForQuest;

	UPROPERTY()
	class UMarkersManagerComponent* MarkersManager = nullptr;

	UFUNCTION()
	void OnRep_Condition();

	UFUNCTION()
	void ConditionChangeBroadcast(ETaskCondition CurrentCondition);

	UFUNCTION(NetMulticast, Reliable)
	void OnUpdatedNotify();
};
