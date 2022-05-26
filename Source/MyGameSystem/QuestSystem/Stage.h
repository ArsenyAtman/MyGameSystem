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
	void Activate(class UQuest* RelatedQuest);
	virtual void Activate_Implementation(class UQuest* RelatedQuest);

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

	UFUNCTION(BlueprintGetter)
	ETaskCondition GetCondition() const { return Condition; }

	UPROPERTY(BlueprintAssignable)
	FStageConditionDelegate OnActivated;

	UPROPERTY(BlueprintAssignable)
	FStageConditionDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FStageConditionDelegate OnFailed;

protected:

	UFUNCTION(BlueprintSetter)
	void SetCondition(ETaskCondition NewCondition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void ActivateObjectives();
	virtual void ActivateObjectives_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void CheckCondition();
	virtual void CheckCondition_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Complete(TSubclassOf<class UStage> NextStage);
	virtual void Complete_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void Fail(TSubclassOf<class UStage> NextStage);
	virtual void Fail_Implementation(TSubclassOf<class UStage> NextStage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageActivated();
	virtual void OnStageActivated_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageCompleted();
	virtual void OnStageCompleted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Control")
	void OnStageFailed();
	virtual void OnStageFailed_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Internal")
	void AbortAllObjectives();
	virtual void AbortAllObjectives_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stage|Objectives")
	TArray<class UObjective*> GetStageObjectives() const;
	virtual TArray<class UObjective*> GetStageObjectives_Implementation() const { return TArray<class UObjective*>(); }

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsAllObjectivesCompleted(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsAllObjectivesFailed(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsOneObjectiveCompleted(const TArray<class UObjective*>& Objectives) const;

	UFUNCTION(BlueprintPure, Category = "Stage|Objectives")
	bool IsOneObjectiveFailed(const TArray<class UObjective*>& Objectives) const;

private:

	UPROPERTY(BlueprintGetter = GetCondition, BlueprintSetter = SetCondition, Replicated = OnRep_Condition)
	ETaskCondition Condition = ETaskCondition::Aborted;

	UFUNCTION()
	void OnRep_Condition();

	UFUNCTION()
	void ConditionChangeBroadcast(ETaskCondition CurrentCondition);
	
};
