// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dialog.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialog : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Begin(class UDialogComponent* OwnDialogComponent, TArray<class AActor*> DialogInterlocutors);

	UFUNCTION(BlueprintCallable)
	void OnDialogUnitPassed(class UObject* DialogUnit, TSubclassOf<UObject> NextDialogUnitClass);

	UFUNCTION(BlueprintPure)
	class AActor* GetOwnerOfVoice(class UDialogueVoice* Voice);

	UFUNCTION(BlueprintPure)
	class UDialogCue* GetCurrentDialogCue(); //was FORCEINLINE

	UFUNCTION(BlueprintPure)
	class UDialogSelection* GetCurrentDialogSelection(); //was FORCEINLINE

	UFUNCTION(BlueprintPure)
	TArray<class UDialogueVoice*> GetInterlocutorsVoices(); //was FORCEINLINE

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<class AActor*> GetInterlocutors() { return Interlocutors; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class AActor* GetInitiator() { return Interlocutors[1]; }

	// GetDialogMaster

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialogComponent* GetOwningDialogComponent() { return OwningDialogComponent; }

	UFUNCTION(BlueprintCallable)
	void BeginDialogForInterlocutors(class UDialogComponent* MasterDialogComponent);

	UFUNCTION(BlueprintCallable)
	void EndDialogForInterlocutors();

	UFUNCTION(BlueprintCallable)
	void UnitStartedForInterlocutors(class UObject* DialogUnit);

	UFUNCTION(BlueprintCallable)
	void UnitEndedForInterlocutors(class UObject* DialogUnit);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MustImplement = "TalkableInterface"))
	TArray<class AActor*> Interlocutors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
	TSubclassOf<UObject> InitialDialogUnit;

private:

	// Initiator

	// DialogMaster

	class UDialogComponent* OwningDialogComponent;

	UPROPERTY()
	class UObject* ActiveDialogUnit;
	
};
