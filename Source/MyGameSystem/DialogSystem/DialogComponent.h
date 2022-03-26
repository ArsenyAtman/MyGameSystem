// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogSystemTypes.h"
#include "Sound/DialogueTypes.h"
#include "DialogComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogConditionDelegate);

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
	FORCEINLINE class UDialogueVoice* GetSpeakerVoice() { return SpeakerVoice; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class USoundAttenuation* GetAttenuation() { return Attenuation; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialogComponent* GetMasterDialogComponent() { return MasterDialogComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE struct FDialogCueStruct GetCurrentDialogCueInfo() { return CurrentDialogCueInfo; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE struct FDialogSelectionStruct GetCurrentDialogSelectionInfo() { return CurrentDialogSelectionInfo; }

	UFUNCTION(BlueprintCallable)
	void SetDialogClass(TSubclassOf<UDialog> NewDialogClass) { DialogClass = NewDialogClass; }

	UFUNCTION(BlueprintCallable)
	void AddNote(FString NoteToAdd);

	UFUNCTION(BlueprintCallable)
	void RemoveNote(FString NoteToRemove);

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<FString> GetNotes() { return Notepad; }

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogCueStarted;

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogSelectionStarted;

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogUnitEnded;

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogStarted;

	UPROPERTY(BlueprintAssignable)
	FDialogConditionDelegate OnDialogEnded;

	UFUNCTION(BlueprintCallable)
	void UnitStarted(class UObject* DialogUnit);

	UFUNCTION(BlueprintCallable)
	void UnitPassed(class UObject* DialogUnit);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* SpeakerVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundAttenuation* Attenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDialog> DialogClass;

private:

	UPROPERTY();
	class UDialog* CurrentDialog;

	UPROPERTY(ReplicatedUsing = OnRep_MasterDialogComponent)
	class UDialogComponent* MasterDialogComponent;

	UFUNCTION()
	void OnRep_MasterDialogComponent();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentDialogCueInfo)
	struct FDialogCueStruct CurrentDialogCueInfo;

	UFUNCTION()
	void OnRep_CurrentDialogCueInfo();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentDialogSelectionInfo)
	struct FDialogSelectionStruct CurrentDialogSelectionInfo;

	UFUNCTION()
	void OnRep_CurrentDialogSelectionInfo();

	UPROPERTY()
	TArray<FString> Notepad;

	// FiredCues
	
};
