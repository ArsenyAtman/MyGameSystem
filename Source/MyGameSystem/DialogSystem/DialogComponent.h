// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "Sound/DialogueTypes.h"
#include "DialogComponent.generated.h"

/**
 * Delegate for handling changes of a dialog.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogConditionDelegate, class UDialog*, Dialog);

/**
 * ActorComponent that handles dialogs.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UDialogComponent : public UReplicatingActorComponent
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
	 * @param CueIndex - The cue index to select.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "DialogComponent|Control")
	void SelectDialogCue(const int CueIndex);

	/**
	 * Notify about a dialog start, that involves this component.
	 * @param NewDialog - The dialog that has started.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogStarted(class UDialog* NewDialog);

	/**
	 * Notify this component about the current dialog end.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogEnded();

	/**
	 * Get the current dialog.
	 * @return The current handling dialog.
	 */
	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	class UDialog* GetCurrentDialog() const { return CurrentDialog; }

	/**
	 * Get the dialog class to start with.
	 * @return The dialog class to start with.
	 */
	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	TSubclassOf<UDialog> GetDialogClass() const { return DialogClass; }

	/**
	 * Set a new dialog class to start with and handle.
	 * @param NewDialogClass - The new dialog class to start with.
	 */
	UFUNCTION(BlueprintSetter, Category = "DialogComponent|Dialog")
	void SetDialogClass(TSubclassOf<UDialog> NewDialogClass) { DialogClass = NewDialogClass; }

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
	 * Called after the current dialog has started.
	 */
	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogConditionDelegate OnDialogStarted;

	/**
	 * Called after the current dialog has ended.
	 */
	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogConditionDelegate OnDialogEnded;

protected:

	/**
	 * Set a new dialog to handle.
	 * @param NewDialog - The new dialog.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintSetter, Category = "DialogComponent|Internal", meta = (BlueprintProtected))
	void SetCurrentDialog(class UDialog* NewDialog);

private:

	/**
	 * The dialog class to start and process.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetDialogClass, BlueprintSetter = SetDialogClass, Category = "DialogComponent|Dialog", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDialog> DialogClass;

	/**
	 * The current dilaog to process.
	 */
	UPROPERTY(BlueprintGetter = GetCurrentDialog, BlueprintSetter = SetCurrentDialog, ReplicatedUsing = OnRep_CurrentDialog);
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
