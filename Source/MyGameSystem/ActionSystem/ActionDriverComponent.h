// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionDriverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionEndDelegate, UClass*, ActionClass);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UActionDriverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionDriverComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "ActionDriver|ActionControl")
	void StartAction(class UActorAction* Action);

	UFUNCTION(BlueprintCallable, Category = "ActionDriver|ActionControl")
	void AbortAction();

	UFUNCTION(BlueprintCallable, Category = "ActionDriver|Internal")
	void ActionCompleted(class UActorAction* Action);

	UFUNCTION(BlueprintGetter, Category = "ActionDriver|CurrentAction")
	class UActorAction* GetCurrentAction() const{ return CurrentAction; }

	UFUNCTION(BlueprintGetter, Category = "ActionDriver|CurrentAction")
	class UClass* GetCurrentActionClass() const { return CurrentActionClass; }

	UPROPERTY(BlueprintAssignable, Category = "ActionDriver|Delegates")
	FActionStartDelegate OnActionStarted;

	UPROPERTY(BlueprintAssignable, Category = "ActionDriver|Delegates")
	FActionEndDelegate OnActionEnded;

protected:

	UFUNCTION(BlueprintSetter, Category = "ActionDriver|CurrentAction")
	void SetCurrentAction(class UActorAction* NewAction);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ActionDriver|Internal")
	void ActionStarted(class UClass* ActionClass);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ActionDriver|Internal")
	void ActionEnded(class UClass* ActionClass);

private:	
	
	UPROPERTY(BlueprintGetter = GetCurrentAction, BlueprintSetter = SetCurrentAction)
	class UActorAction* CurrentAction;

	UPROPERTY(BlueprintGetter = GetCurrentActionClass)
	class UClass* CurrentActionClass;
		
};
