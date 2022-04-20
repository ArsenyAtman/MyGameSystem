// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "ConditionComponent.generated.h"

/**
 * A delegate of the ConditionComponent class for tracking condition changes. 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConditionComponentDelegate);

/**
 * A component that handles and switches actor conditions. 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UConditionComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConditionComponent();

	// Setup the replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Start the initial condition.
	virtual void BeginPlay() override;

	/**
	 * Abort the current condition and start a new one.
	 * @param NewCondition - A new condition with this ConditionComponent as the owner.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "ConditionComponent|Internal")
	void ConditionChange(class UCondition* NewCondition);

	/**
	 * Get the current condition that is handling by this component.
	 * @return The current condition.
	 */
	UFUNCTION(BlueprintGetter, Category = "ConditionComponent|CurrentCondition")
	class UCondition* GetCurrentCondition() const { return CurrentCondition; }

	/**
	 * Called after the current condition has changed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ConditionComponent|Delegates")
	FConditionComponentDelegate OnConditionChanged;

protected:

	/**
	 * Set the current condition.
	 * @param NewCondition - A new condition with this ConditionComponent as the owner.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintSetter, Category = "ConditionComponent|Internal")
	void SetCurrentCondition(class UCondition* NewCondition);

	/**
	 * The class of the condition that will be constructed and set as the current after the game start.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConditionComponent|InitialCondition", meta = (ExposeOnSpawn = true))
	TSubclassOf<class UCondition> InitialConditionClass;

private:

	UPROPERTY(BlueprintGetter = GetCurrentCondition, BlueprintSetter = SetCurrentCondition, ReplicatedUsing = OnRep_CurrentCondition)
	class UCondition* CurrentCondition;

	UFUNCTION()
	void OnRep_CurrentCondition();
		
};
