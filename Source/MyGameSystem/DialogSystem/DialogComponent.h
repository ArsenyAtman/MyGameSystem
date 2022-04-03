// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/DialogueTypes.h"
#include "DialogComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogConditionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogUnitConditionDelegate, class UDialogUnitDataAsset*, DialogUnitData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Control")
	void BeginDialogue(class APawn* Initiator, const TArray<class AActor*>& AdditionalInterlocutors);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "DialogComponent|Control")
	void SelectDialogCue(int CueIndex);

	// SkipCurrentCue

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogStarted(class UDialogComponent* NewMasterDialogComponent);

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Internal")
	void DialogEnded();

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	class UDialog* GetCurrentDialog() { return CurrentDialog; }

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	class UDialogComponent* GetMasterDialogComponent() { return MasterDialogComponent; }

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	class UDialogUnitDataAsset* GetCurrentDialogUnitData() { return CurrentDialogUnitData; }

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	TSubclassOf<UDialog> GetDialogClass() { return DialogClass; }

	UFUNCTION(BlueprintSetter, Category = "DialogComponent|Dialog")
	void SetDialogClass(TSubclassOf<UDialog> NewDialogClass) { DialogClass = NewDialogClass; }

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Dialog")
	void AddNote(FString NoteToAdd);

	UFUNCTION(BlueprintCallable, Category = "DialogComponent|Dialog")
	void RemoveNote(FString NoteToRemove);

	UFUNCTION(BlueprintGetter, Category = "DialogComponent|Dialog")
	TArray<FString> GetNotes() { return Notepad; }

	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogUnitConditionDelegate OnDialogUnitStarted;

	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogUnitConditionDelegate OnDialogUnitEnded;

	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogConditionDelegate OnDialogStarted;

	UPROPERTY(BlueprintAssignable, Category = "DialogComponent|Delegates")
	FDialogConditionDelegate OnDialogEnded;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "DialogComponent|Internal")
	void UnitStarted(class UDialogUnitDataAsset* DialogUnitData);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "DialogComponent|Internal")
	void UnitPassed(class UDialogUnitDataAsset* DialogUnitData);

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetDialogClass, BlueprintSetter = SetDialogClass, Category = "DialogComponent|Dialog", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDialog> DialogClass;

	UPROPERTY(BlueprintGetter = GetCurrentDialog);
	class UDialog* CurrentDialog;

	UPROPERTY(BlueprintGetter = GetMasterDialogComponent, ReplicatedUsing = OnRep_MasterDialogComponent)
	class UDialogComponent* MasterDialogComponent;

	UFUNCTION()
	void OnRep_MasterDialogComponent();

	UPROPERTY(BlueprintGetter = GetCurrentDialogUnitData)
	class UDialogUnitDataAsset* CurrentDialogUnitData;

	UPROPERTY(BlueprintGetter = GetNotes)
	TArray<FString> Notepad;

	// FiredCues
	
};
