// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "ConditionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConditionComponentDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UConditionComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConditionComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "ConditionComponent|Internal")
	void ConditionChange(class UCondition* NewCondition);

	UFUNCTION(BlueprintGetter, Category = "ConditionComponent|CurrentCondition")
	class UCondition* GetCurrentCondition() const { return CurrentCondition; }

	UPROPERTY(BlueprintAssignable, Category = "ConditionComponent|Delegates")
	FConditionComponentDelegate OnConditionChanged;

protected:

	UFUNCTION(BlueprintSetter, Category = "ConditionComponent|Internal")
	void SetCurrentCondition(class UCondition* NewCondition);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConditionComponent|InitialCondition", meta = (ExposeOnSpawn = true))
	TSubclassOf<class UCondition> InitialConditionClass;

private:

	UPROPERTY(BlueprintGetter = GetCurrentCondition, BlueprintSetter = SetCurrentCondition, ReplicatedUsing = OnRep_CurrentCondition)
	class UCondition* CurrentCondition;

	UFUNCTION()
	void OnRep_CurrentCondition();
		
};
