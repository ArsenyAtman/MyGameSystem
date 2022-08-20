// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AdvancedObject/AdvancedObject.h"
#include "ActorAction.generated.h"


/**
 * Delegate for handling changes of an ActorAction.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorActionConditionDelegate, class UActorAction*, ActorAction);

/**
 * Some action that is limited in time. May have some logic and line up in combinations.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UActorAction : public UAdvancedObject
{
	GENERATED_BODY()

public:

	/**
	 * Start this action.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActorAction|Internal")
	void StartAction();

	/**
	 * End this action.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActorAction|ActionControl")
	void EndAction();

	/**
	 * Called when a new action tried to start.
	 * @param NewAction - The action that tried to start.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl")
	void NewActionTriedToStart(class UActorAction* NewAction);
	virtual void NewActionTriedToStart_Implementation(class UActorAction* NewAction) { return; }

	/**
	 * Return the controlled actor by this action.
	 * @return The controlled actor.
	 */
	UFUNCTION(BlueprintPure, Category = "ActorAction")
	class AActor* GetControlledActor() const;

	/**
	 * Return the action driver of this action.
	 * @return The action driver component.
	 */
	UFUNCTION(BlueprintPure, Category = "ActorAction")
	class UActionDriverComponent* GetActionDriverComponent() const;

	/**
	 * Notify the action about the event start.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActorAction|ActionEvent")
	void StartActionEvent();

	/**
	 * Notify the action about the event end.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActorAction|ActionEvent")
	void EndActionEvent();

	/**
	 * Whether the action event started.
	 * @return Whether the action event started.
	 */
	UFUNCTION(BlueprintGetter, Category = "ActorAction|ActionEvent")
	bool GetEventStarted() const { return bEventStarted; }

	/**
	 * Notify the action about the combo window start.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActorAction|ComboWindow")
	void StartComboWindow();

	/**
	 * Notify the action about the combo window end.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActorAction|ComboWindow")
	void EndComboWindow();

	/**
	 * Whether the action combo window started.
	 * @return Whether the action combo window started.
	 */
	UFUNCTION(BlueprintGetter, Category = "ActorAction|ComboWindow")
	FORCEINLINE bool GetComboWindowStarted() const { return bComboWindowStarted; }

	/**
	 * Called after the action start.
	 */
	UPROPERTY(BlueprintAssignable)
	FActorActionConditionDelegate OnActorActionStarted;

	/**
	 * Called after the action end
	 */
	UPROPERTY(BlueprintAssignable)
	FActorActionConditionDelegate OnActorActionEnded;

protected:

	/**
	 * Called once after the action start.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl", meta = (BlueprintProtected))
	void OnActionStarted();
	virtual void OnActionStarted_Implementation() { return; }

	/**
	 * Called once before the action end.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl", meta = (BlueprintProtected))
	void OnActionEnded();
	virtual void OnActionEnded_Implementation() { return; }

	/**
	 * Called when the action event starts.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl", meta = (BlueprintProtected))
	void OnEventStarted();
	virtual void OnEventStarted_Implementation() { return; }

	/**
	 * Called when the action event ends.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl", meta = (BlueprintProtected))
	void OnEventEnded();
	virtual void OnEventEnded_Implementation() { return; }

	/**
	 * Called when the combo window starts.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl", meta = (BlueprintProtected))
	void OnComboWindowStarted();
	virtual void OnComboWindowStarted_Implementation() { return; }

	/**
	 * Called when the combo window ends.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorAction|ActionControl", meta = (BlueprintProtected))
	void OnComboWindowEnded();
	virtual void OnComboWindowEnded_Implementation() { return; }

private:

	// Notify all machines about the action start.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ActionStart();

	// Notify all machines about the action end.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ActionEnd();

	// Notify all machines about the action event start.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ActionEventStart();

	// Notify all machines about the action event end.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ActionEventEnd();

	// Notify all machines about the combo window start.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ComboWindowStart();

	// Notify all machines about the combo window end.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ComboWindowEnd();

	// Broadcast delegates.
	void Broadcast_ActionStart();
	void Broadcast_ActionEnd();

	/**
	 * Whether the action event started.
	 */
	UPROPERTY(BlueprintGetter = GetEventStarted)
	bool bEventStarted = false;

	/**
	 * Whether the action combo window started.
	 */
	UPROPERTY(BlueprintGetter = GetComboWindowStarted)
	bool bComboWindowStarted = false;
};
