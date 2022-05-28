// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "QuestSystemTypes.h"
#include "Objective.h"
#include "Stage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStageConditionDelegate, class UStage*, Stage);

UCLASS(Blueprintable, BlueprintType, Abstract)
class MYGAMESYSTEM_API UStage : public UAdvancedObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Activate();
	virtual void Activate_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ObjectiveCompleted(class UObjective* Objective);
	virtual void ObjectiveCompleted_Implementation(class UObjective* Objective);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ObjectiveFailed(class UObjective* Objective);
	virtual void ObjectiveFailed_Implementation(class UObjective* Objective);

	UFUNCTION(BlueprintCallable, Category = "Stage|Internal")
	void MarkObjectives();

	UFUNCTION(BlueprintCallable, Category = "Stage|Internal")
	void UnmarkObjectives();

	UFUNCTION(BlueprintGetter, Category = "Stage|OwningQuest")
	class UQuest* GetOwningQuest() const;

	UFUNCTION(BlueprintGetter, Category = "Stage|Condition")
	ETaskCondition GetCondition() const { return Condition; }

	UPROPERTY(BlueprintAssignable, Category = "Stage|Delegates")
	FStageConditionDelegate OnActivated;

	UPROPERTY(BlueprintAssignable, Category = "Stage|Delegates")
	FStageConditionDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Stage|Delegates")
	FStageConditionDelegate OnFailed;

protected:

	UFUNCTION(BlueprintSetter, Category = "Stage|Condition", meta = (BlueprintProtected))
	void SetCondition(ETaskCondition NewCondition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void ActivateObjectives();
	virtual void ActivateObjectives_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void CheckCondition();
	virtual void CheckCondition_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void Complete(TSubclassOf<class UStage> NextStage);
	virtual void Complete_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void Fail(TSubclassOf<class UStage> NextStage);
	virtual void Fail_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void OnStageActivated();
	virtual void OnStageActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void OnStageCompleted();
	virtual void OnStageCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control", meta = (BlueprintProtected))
	void OnStageFailed();
	virtual void OnStageFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal", meta = (BlueprintProtected))
	void AbortAllObjectives();
	virtual void AbortAllObjectives_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Objectives", meta = (BlueprintProtected))
	TArray<class UObjective*> GetStageObjectives() const;
	virtual TArray<class UObjective*> GetStageObjectives_Implementation() const { return TArray<class UObjective*>(); }

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsAllObjectivesCompleted(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsAllObjectivesFailed(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsOneObjectiveCompleted(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives", meta = (BlueprintProtected))
	bool IsOneObjectiveFailed(const TArray<class UObjective*>& Objectives) const;

private:

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

	UFUNCTION()
	void OnRep_Condition();

	UFUNCTION()
	void BroadcastChange_Condition();
	
};
