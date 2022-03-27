// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogSystemTypes.h"
#include "Sound/DialogueTypes.h"
#include "DialogComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogConditionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogUnitConditionDelegate, class UDialogUnitInfo*, DialogUnitInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void BeginDialogue(class APawn* Initiator, const TArray<class AActor*>& AdditionalInterlocutors);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SelectDialogCue(int CueIndex);

	// SkipCurrentCue

	UFUNCTION(BlueprintCallable)
	void DialogStarted(class UDialogComponent* NewMasterDialogComponent);

	UFUNCTION(BlueprintCallable)
	void DialogEnded();

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialog* GetCurrentDialog() { return CurrentDialog; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialogComponent* GetMasterDialogComponent() { return MasterDialogComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialogUnitInfo* GetCurrentDialogUnitInfo() { return CurrentDialogUnitInfo; }

	UFUNCTION(BlueprintCallable)
	void SetDialogClass(TSubclassOf<UDialog> NewDialogClass) { DialogClass = NewDialogClass; }

	UFUNCTION(BlueprintCallable)
	void AddNote(FString NoteToAdd);

	UFUNCTION(BlueprintCallable)
	void RemoveNote(FString NoteToRemove);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<FString> GetNotes() { return Notepad; }

	UPROPERTY(BlueprintAssignable)
	FDialogUnitConditionDelegate OnDialogUnitStarted;

	UPROPERTY(BlueprintAssignable)
	FDialogUnitConditionDelegate OnDialogUnitEnded;

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogStarted;

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogEnded;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void UnitStarted(class UDialogUnitInfo* DialogUnitInfo);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void UnitPassed(class UDialogUnitInfo* DialogUnitInfo);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDialog> DialogClass;

private:

	UPROPERTY();
	class UDialog* CurrentDialog;

	UPROPERTY(ReplicatedUsing = OnRep_MasterDialogComponent)
	class UDialogComponent* MasterDialogComponent;

	UFUNCTION()
	void OnRep_MasterDialogComponent();

	class UDialogUnitInfo* CurrentDialogUnitInfo;

	UPROPERTY()
	TArray<FString> Notepad;

	// FiredCues
	
};
