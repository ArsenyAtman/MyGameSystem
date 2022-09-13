// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedObject/ReplicatingActorComponent.h"
#include "Sound/DialogueTypes.h"
#include "DialogComponent.generated.h"

/**
 * Delegate for handling changes of a dialog.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogComponentCurrentDialogChangeDelegate, class UDialog*, NewDialog, class UDialog*, PrevDialog);

/**
 * ActorComponent that handles dialogs.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEMPLUGIN_API UDialogComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UDialogComponent();

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Start a new dialog of the preset dialog class.
	 * @param Initiator - The intitiator of the dialog.
	 * @param AdditionalInterlocutors - The additional interlocutors that are participating in the new dialog.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Control")
	void BeginDialogue(class AActor* Initiator, const TArray<class AActor*>& AdditionalInterlocutors);

	/**
	 * Select the next dialog cue from the current selection.
	 * @param NextDialogUnit - The dialog unit to select.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "DialogComponent|Control")
	void SelectNextDialogUnit(TSubclassOf<class UDialogUnit> NextDialogUnit);

	/**
	 * Skip the current dialog cue.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "DialogComponent|Control")
	void SkipCurrentDialogCue();

	/**
	 * Notify about a dialog start, that involves this component.
	 * @param NewDialog - The dialog that has started.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogStarted(class UDialog* NewDialog);

	/**
	 * Notify this component about the current dialog end.
	 * @param Dialog - The dialog that ends.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogEnded(class UDialog* Dialog);

	/**
	 * Get the current dialog.
	 * @return The current handling dialog.
	 */
	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	class UDialog* GetCurrentDialog() const { return CurrentDialog; }

	/**
	 * Set a new dialog to handle.
	 * @param NewDialog - The new dialog.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Dialog")
	bool SetCurrentDialog(class UDialog* NewDialog);

	/**
	 * Add a note to the notepad of this dialog component.
	 * @param NoteToAdd - The note to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Notes")
	void AddNote(const FString& NoteToAdd);

	/**
	 * Remove a note from the notepad of this dialog component.
	 * @param NoteToRemove - The note to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Notes")
	void RemoveNote(const FString& NoteToRemove);

	/**
	 * Get notes of this dialog component.
	 * @return The list of notes.
	 */
	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Notes")
	TArray<FString> GetNotes() const { return Notepad; }

	/**
	 * Called after the current dialog has changed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogComponentCurrentDialogChangeDelegate OnDialogChanged;

private:

	/**
	 * The current dilaog to process.
	 */
	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintGetter = GetCurrentDialog, ReplicatedUsing = OnRep_CurrentDialog);
	class UDialog* CurrentDialog;

	// OnRep event of CurrentDialog.
	UFUNCTION()
	void OnRep_CurrentDialog(class UDialog* PreReplicationCurrentDialog);

	// Broadcast a delegate od CurrentDialog.
	void Broadcast_CurrentDialog(class UDialog* PrevDialog);

	// Notes for dialogs.
	UPROPERTY(BlueprintGetter = GetNotes, Category = "DialogComponent|Notes")
	TArray<FString> Notepad;
	
};
