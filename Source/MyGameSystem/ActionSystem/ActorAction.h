// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorAction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UActorAction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ActorAction|Internal")
	void StartAction();
	
	UFUNCTION(BlueprintCallable, Category = "ActorAction|ActionControl")
	void EndAction();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void NewActionTriedToStart(class UActorAction* NewAction);
	virtual void NewActionTriedToStart_Implementation(class UActorAction* NewAction) { return; }

	UFUNCTION(BlueprintPure, Category = "ActorAction")
	class AActor* GetControlledActor();

	UFUNCTION(BlueprintPure, Category = "ActorAction")
	class UActionDriverComponent* GetActionDriverComponent();

	UFUNCTION(BlueprintCallable, Category = "ActorAction|ActionEvent")
	void StartActionEvent();

	UFUNCTION(BlueprintCallable, Category = "ActorAction|ActionEvent")
	void EndActionEvent();

	UFUNCTION(BlueprintGetter, Category = "ActorAction|ActionEvent")
	FORCEINLINE bool GetEventStarted() { return bEventStarted; }

	UFUNCTION(BlueprintCallable, Category = "ActorAction|ComboWindow")
	void StartComboWindow();

	UFUNCTION(BlueprintCallable, Category = "ActorAction|ComboWindow")
	void EndComboWindow();

	UFUNCTION(BlueprintGetter, Category = "ActorAction|ComboWindow")
	FORCEINLINE bool GetComboWindowStarted() { return bComboWindowStarted; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void OnActionStarted();
	virtual void OnActionStarted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void OnActionEnded();
	virtual void OnActionEnded_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void OnEventStarted();
	virtual void OnEventStarted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void OnEventEnded();
	virtual void OnEventEnded_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void OnComboWindowStarted();
	virtual void OnComboWindowStarted_Implementation() { return; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void OnComboWindowEnded();
	virtual void OnComboWindowEnded_Implementation() { return; }

private:

	UPROPERTY(BlueprintGetter = GetEventStarted)
	bool bEventStarted = false;

	UPROPERTY(BlueprintGetter = GetComboWindowStarted)
	bool bComboWindowStarted = false;
};
