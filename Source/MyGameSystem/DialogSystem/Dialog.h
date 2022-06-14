// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "Dialog.generated.h"

/**
 * Delegate for handling changes of dialog units.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogUnitChangeDelegate, class UDialogUnit*, NewDialogUnit, class UDialog*, OfDialog);

/**
 * Object, that handles dialog units.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialog : public UAdvancedObject
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Start this dialog.
	 * @param Master - The dialog master (owner) actor.
	 * @param Initiator - The actor that has initiated the dialog.
	 * @param OtherInterlocutors - All other interlocutors, that are involved in this dialog.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void Begin(class AActor* Master, class AActor* Initiator, const TArray<class AActor*>& OtherInterlocutors);

	/**
	 * Notify this dialog about the current dialog unit end.
	 * @param DialogUnit - The ending dialog unit
	 * @param NextDialogUnitClass - The next dialog unit class to play.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void OnDialogUnitPassed(class UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass);

	/**
	 * Get the current dialog unit.
	 * @return The current dialog unit.
	 */
	UFUNCTION(BlueprintGetter, Category = "Dialog|CurrentDialogUnit")
	class UDialogUnit* GetCurrentDialogUnit() const {return CurrentDialogUnit; }

	/**
	 * Get the additional interlocutors.
	 * @return The additional interlocutors.
	 */
	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	TArray<class AActor*> GetAdditionalInterlocutors() const { return AdditionalInterlocutors; }

	/**
	 * Get the all interlocutors of this dialog
	 * @return The all interlocutors of this dialog
	 */
	UFUNCTION(BlueprintPure, Category = "Dialog|Interlocutors")
	TArray<class AActor*> GetAllInterlocutors() const;

	/**
	 * Get the dialog master actor.
	 * @return The dialog master actor.
	 */
	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	class AActor* GetDialogMaster() const { return DialogMaster; }

	/**
	 * Get the dialog initiator actor.
	 * @return The dialog initiator.
	 */
	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	class AActor* GetDialogInitiator() const { return DialogInitiator; }

	UFUNCTION(BlueprintPure, Category = "Dialog|OwningDialogComponent")
	class UDialogComponent* GetOwningDialogComponent() const;

	/**
	 * Called after the current dialog unit has changed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Dialog|Delegates")
	FDialogUnitChangeDelegate OnDialogUnitChanged;

protected:

	/**
	 * Involve the listed interlocutors (without the master actor, bcz he already knows about the dialog) in this dialog.
	 * @param DialogInterlocutorsWithoutMaster - All interlocutors without the master (but it isn't critical).
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal", meta = (BlueprintProtected))
	void BeginDialogForInterlocutors(const TArray<class AActor*>& DialogInterlocutorsWithoutMaster);

	/**
	 * End this dialog for the every interlocutor.
	 * @param DialogInterlocutors - All interlocutors of this dialog including the master (IT IS CRITICAL !!1).
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal", meta = (BlueprintProtected))
	void EndDialogForInterlocutors(const TArray<class AActor*>& DialogInterlocutors);

	/**
	 * Set a new active dialog unit.
	 * @param NewDialogUnit - The new current dialog unit.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintSetter, Category = "Dialog|CurrentDialogUnit", meta = (BlueprintProtected))
	void SetCurrentDialogUnit(class UDialogUnit* NewDialogUnit);

	/**
	 * The initial dialog unit.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialog|InitialDialogUnit", meta = (BlueprintProtected))
	TSubclassOf<UDialogUnit> InitialDialogUnit;

private:

	/**
	 * The additional persons that are participating in this dialog.
	 */
	UPROPERTY(BlueprintGetter = GetAdditionalInterlocutors)
	TArray<class AActor*> AdditionalInterlocutors;

	/**
	 * Actor that has initiated this dialog.
	 */
	UPROPERTY(BlueprintGetter = GetDialogInitiator)
	class AActor* DialogInitiator = nullptr;

	/**
	 * Actor that owns the master dialog component that owns this dialog.
	 */
	UPROPERTY(BlueprintGetter = GetDialogMaster)
	class AActor* DialogMaster = nullptr;

	/**
	 * The current dialog unit to process.
	 */
	UPROPERTY(BlueprintGetter = GetCurrentDialogUnit, BlueprintSetter = SetCurrentDialogUnit, ReplicatedUsing = OnRep_CurrentDialogUnit, Category = "Dialog|CurrentDialogUnit")
	class UDialogUnit* CurrentDialogUnit;

	// OnRep event of CurrentDialogUnit
	UFUNCTION()
	void OnRep_CurrentDialogUnit();

	// Broadcast OnDialogUnitChanged delegate.
	void Broadcast_CurrentDialogUnit();
	
};
