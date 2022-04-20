// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "Condition.generated.h"

/**
 * An object that implements a behaviour of the controlled actor.
 * @see ConditionComponent
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UCondition : public UAdvancedObject
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

protected:

	/**
	 * Abort this condition and start a new one. See the NextCondition description.
	 * @param NextCondition - A new condition with the same outer as this condition has (the same ConditionComponent).
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "Condition|Control", meta = (BlueprintProtected))
	void ChangeCondition(class UCondition* NextCondition);

	/**
	 * Called once after the start of this condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Condition|Control", meta = (BlueprintProtected))
	void OnConditionStarted();
	virtual void OnConditionStarted_Implementation() { return; }

	/**
	 * Called once before the end of this condition.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Condition|Control", meta = (BlueprintProtected))
	void OnConditionEnded();
	virtual void OnConditionEnded_Implementation() { return; }

};
