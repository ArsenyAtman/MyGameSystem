// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "Dialog.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogUnitChangeDelegate, class UDialogUnit*, DialogUnit, class UDialog*, OfDialog);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialog : public UAdvancedObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void Begin(class AActor* Master, class AActor* Initiator, const TArray<class AActor*>& OtherInterlocutors);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void OnDialogUnitPassed(class UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass);

	UFUNCTION(BlueprintGetter, Category = "Dialog|CurrentDialogUnit")
	class UDialogUnit* GetCurrentDialogUnit() const {return CurrentDialogUnit; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	TArray<class AActor*> GetInterlocutors() const { return Interlocutors; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	class AActor* GetDialogMaster() const { return DialogMaster; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	class AActor* GetDialogInitiator() const { return DialogInitiator; }

	UFUNCTION(BlueprintPure, Category = "Dialog|OwningDialogComponent")
	class UDialogComponent* GetOwningDialogComponent() const;

	UPROPERTY(BlueprintAssignable)
	FDialogUnitChangeDelegate OnDialogUnitChanged;

protected:

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void BeginDialogForInterlocutors(class UDialogComponent* MasterDialogComponent, const TArray<class AActor*>& DialogInterlocutors);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void EndDialogForInterlocutors(const TArray<class AActor*>& DialogInterlocutors);

	UFUNCTION(BlueprintSetter)
	void SetCurrentDialogUnit(class UDialogUnit* NewDialogUnit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialog|InitialDialogUnit")
	TSubclassOf<UDialogUnit> InitialDialogUnit;

private:

	UPROPERTY(BlueprintGetter = GetInterlocutors)
	TArray<class AActor*> Interlocutors;

	UPROPERTY(BlueprintGetter = GetDialogInitiator)
	class AActor* DialogInitiator = nullptr;

	UPROPERTY(BlueprintGetter = GetDialogMaster)
	class AActor* DialogMaster = nullptr;

	UPROPERTY(BlueprintGetter = GetCurrentDialogUnit, BlueprintSetter = SetCurrentDialogUnit, ReplicatedUsing = OnRep_CurrentDialogUnit)
	class UDialogUnit* CurrentDialogUnit;

	UFUNCTION()
	void OnRep_CurrentDialogUnit();

	void Broadcast_CurrentDialogUnit();
	
};
