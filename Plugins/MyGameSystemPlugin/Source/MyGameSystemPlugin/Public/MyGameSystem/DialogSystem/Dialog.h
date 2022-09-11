// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedObject/AdvancedObject.h"
#include "Dialog.generated.h"

/**
 * Delegate for handling changes of dialog units.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogUnitChangeDelegate, class UDialogUnit*, NewDialogUnit, class UDialog*, OfDialog);

/**
 * Delegate for handling changes of a dialog.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogConditionDelegate, class UDialog*, Dialog);

/**
 * Object, that handles dialog units.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class MYGAMESYSTEMPLUGIN_API UDialog : public UAdvancedObject
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
	class UDialogUnit* GetCurrentDialogUnit() const { return CurrentDialogUnit; }

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

	/**
	 * Get the owning dialog component.
	 * @return The owning dialog component.
	 */
	UFUNCTION(BlueprintPure, Category = "Dialog|OwningDialogComponent")
	class UDialogComponent* GetOwningDialogComponent() const;

	/**
	 * Check was this cue fired.
	 * @param DialogCueClass - A dialog cue class to check.
	 * @return Was this cue fired.
	 */
	UFUNCTION(BlueprintPure, Category = "DialogComponent|Dialog")
	bool WasCueFired(TSubclassOf<class UDialogCue> DialogCueClass) const;

	/**
	 * Called after the current dialog unit has changed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Dialog|Delegates")
	FDialogUnitChangeDelegate OnDialogUnitChanged;

	/**
	 * Called after the dialog start.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Dialog|Delegates")
	FDialogConditionDelegate OnDialogStarted;

	/**
	 * Called after the dialog end.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Dialog|Delegates")
	FDialogConditionDelegate OnDialogEnded;

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
	 * @warning If NewDialogUnit equals nullptr then the dialog is over!
	 */
	UFUNCTION(BlueprintSetter, Category = "Dialog|CurrentDialogUnit", meta = (BlueprintProtected))
	void SetCurrentDialogUnit(class UDialogUnit* NewDialogUnit);

	/**
	 * The initial dialog unit.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialog|InitialDialogUnit", meta = (BlueprintProtected))
	TSubclassOf<UDialogUnit> InitialDialogUnit;

private:

	// Fired cues.
	TArray<TSubclassOf<class UDialogCue>> FiredCues;

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
	class UDialogUnit* CurrentDialogUnit = nullptr;

	// OnRep event of CurrentDialogUnit.
	UFUNCTION()
	void OnRep_CurrentDialogUnit(class UDialogUnit* PrevDialogUnit);

	// Broadcast the related delegate to the change event.
	void DialogConditionChanged(class UDialogUnit* PrevDialogUnit);

	/**
	 * Set whether this dialog has been started.
	 * @param bNewStarted - Was this dialog started.
	 */
	UFUNCTION(BlueprintSetter)
	void SetStarted(bool bNewStarted);

	/**
	 * Whether this dialog has been started.
	 */
	UPROPERTY(BlueprintSetter = SetStarted, ReplicatedUsing = OnRep_Started)
	bool bStarted = false;

	// OnRep event of bStarted.
	UFUNCTION()
	void OnRep_Started();

	// Broadcast the start/end delegate.
	void Broadcast_Started();
	
};
