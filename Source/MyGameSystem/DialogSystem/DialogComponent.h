// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/ReplicatingActorComponent.h"
#include "Sound/DialogueTypes.h"
#include "DialogComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogConditionDelegate, class UDialog*, Dialog);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UDialogComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UDialogComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Control")
	void BeginDialogue(class AActor* Initiator, const TArray<class AActor*>& AdditionalInterlocutors);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "DialogComponent|Control")
	void SelectDialogCue(const int CueIndex);

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogStarted(class UDialog* NewDialog);

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogEnded();

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	class UDialog* GetCurrentDialog() const { return CurrentDialog; }

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	TSubclassOf<UDialog> GetDialogClass() const { return DialogClass; }

	UFUNCTION(BlueprintSetter, Category = "DialogComponent|Dialog")
	void SetDialogClass(TSubclassOf<UDialog> NewDialogClass) { DialogClass = NewDialogClass; }

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Dialog")
	void AddNote(const FString& NoteToAdd);

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Dialog")
	void RemoveNote(const FString& NoteToRemove);

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	TArray<FString> GetNotes() const { return Notepad; }

	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogConditionDelegate OnDialogStarted;

	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogConditionDelegate OnDialogEnded;

protected:

	UFUNCTION(BlueprintSetter, Category = "DialogComponent|Dialog", meta = (BlueprintProtected))
	void SetCurrentDialog(class UDialog* NewDialog);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetDialogClass, BlueprintSetter = SetDialogClass, Category = "DialogComponent|Dialog", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDialog> DialogClass;

	UPROPERTY(BlueprintGetter = GetCurrentDialog, BlueprintSetter = SetCurrentDialog, ReplicatedUsing = OnRep_CurrentDialog);
	class UDialog* CurrentDialog;

	UFUNCTION()
	void OnRep_CurrentDialog(class UDialog* PreReplicationCurrentDialog);

	void Broadcast_CurrentDialog(class UDialog* PrevDialog);

	UPROPERTY(BlueprintGetter = GetNotes)
	TArray<FString> Notepad;
	
};
