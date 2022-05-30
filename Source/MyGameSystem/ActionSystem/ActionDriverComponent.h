// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "ActionDriverComponent.generated.h"

/**
 * Delegate for tracking changes of current actions of ActionDriverComponent.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionChangeDelegate);

/**
 * ActorComponent that handles actor actions.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UActionDriverComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties.
	UActionDriverComponent();

	// Setup the replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Start an action. See the Action parameter description.
	 * @param Action - A new action with this ActionDriverComponent as the outer.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActionDriver|ActionControl")
	void StartAction(class UActorAction* Action);

	/**
	 * Abort the current action.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActionDriver|ActionControl")
	void AbortAction();

	/**
	 * Called by the current action when it ends.
	 * @param Action - The action that is ending.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ActionDriver|Internal")
	void ActionCompleted(class UActorAction* Action);

	/**
	 * Get the current action.
	 * @return The current action.
	 */
	UFUNCTION(BlueprintGetter, Category = "ActionDriver|CurrentAction")
	class UActorAction* GetCurrentAction() const{ return CurrentAction; }

	/**
	 * Called after the start of the current action.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ActionDriver|Delegates")
	FActionChangeDelegate OnActionStarted;

	/**
	 * Called after the end of the current action.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ActionDriver|Delegates")
	FActionChangeDelegate OnActionEnded;

protected:

	/**
	 * Set the current action.
	 * @param NewAction - A new action with this ActionDriverComponent as the outer.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintSetter, Category = "ActionDriver|CurrentAction", meta = (BlueprintProtected))
	void SetCurrentAction(class UActorAction* NewAction);

private:	
	
	/**
	 * The current playing action.
	 */
	UPROPERTY(BlueprintGetter = GetCurrentAction, BlueprintSetter = SetCurrentAction, ReplicatedUsing = OnRep_CurrentAction)
	class UActorAction* CurrentAction;

	UFUNCTION()
	void OnRep_CurrentAction();
		
};
