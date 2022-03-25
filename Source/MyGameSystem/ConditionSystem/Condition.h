// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Condition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UCondition : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Condition|Internal")
	void StartCondition();

	UFUNCTION(BlueprintPure, Category = "Condition")
	class AActor* GetControlledActor();

	UFUNCTION(BlueprintPure, Category = "Condition")
	class UConditionComponent* GetConditionComponent();

protected:

	UFUNCTION(BlueprintCallable, Category = "Condition|Control")
	void ChangeCondition(class UCondition* NextCondition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Condition|Control")
	void OnConditionStarted();
	virtual void OnConditionStarted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Condition|Control")
	void OnConditionEnded();
	virtual void OnConditionEnded_Implementation() { return; }

};