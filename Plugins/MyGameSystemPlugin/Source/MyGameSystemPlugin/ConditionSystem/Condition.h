// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "Condition.generated.h"

/**
 * Delegate for handling changes of a Condition.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConditionStateDelegate, class UCondition*, Condition);

/**
 * Object that implements a behavior of the controlled actor.
 * @see ConditionComponent
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UCondition : public UAdvancedObject
{
	GENERATED_BODY()

public:

	/**
	 * Start(activate) this condition.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Condition|Internal")
	void StartCondition();

	/**
	 * Get the controlled actor by this condition.
	 * @return The actor that controlled by this condition.
	 */
	UFUNCTION(BlueprintPure, Category = "Condition")
	class AActor* GetControlledActor() const;

	/**
	 * Get the ConditionComponent of this condition.
	 * @return The ConditionComponent that handles this condition.
	 */
	UFUNCTION(BlueprintPure, Category = "Condition")
	class UConditionComponent* GetConditionComponent() const;

	/**
	 * Called after the condition start.
	 */
	UPROPERTY(BlueprintAssignable)
	FConditionStateDelegate OnConditionStarted;

	/**
	 * Called after the condition end.
	 */
	UPROPERTY(BlueprintAssignable)
	FConditionStateDelegate OnConditionEnded;

protected:

	virtual void EndPlay_Implementation() override;

	/**
	 * Abort this condition and start a new one.
	 * @param NextCondition - A new condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "Condition|Control", meta = (BlueprintProtected))
	void ChangeCondition(class UCondition* NextCondition);

	/**
	 * Called once after the start of this condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Condition|Control", meta = (BlueprintProtected))
	void ConditionStarted();
	virtual void ConditionStarted_Implementation() { return; }

	/**
	 * Called once before the end of this condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Condition|Control", meta = (BlueprintProtected))
	void ConditionEnded();
	virtual void ConditionEnded_Implementation() { return; }

private:

	// Notify all machines about the condition start.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_ConditionStart();

	// Broadcast delegates.
	void Broadcast_ConditionStart();
	void Broadcast_ConditionEnd();

};
