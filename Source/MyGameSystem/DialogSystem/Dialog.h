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
	void Begin(class UDialogComponent* OwnDialogComponent, class AActor* Master, class AActor* Initiator, TArray<class AActor*> OtherInterlocutors);

	UFUNCTION(BlueprintCallable)
	void OnDialogUnitPassed(class UObject* DialogUnit, TSubclassOf<UObject> NextDialogUnitClass);

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UDialogUnit* GetCurrentDialogUnit() {return ActiveDialogUnit; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<class AActor*> GetInterlocutors() { return DialogInterlocutors; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class AActor* GetDialogMaster() { return DialogMaster; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class AActor* GetDialogInitiator() { return DialogInitiator; }

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
	TSubclassOf<UDialogUnit> InitialDialogUnit;

private:

	TArray<class AActor*> DialogInterlocutors;

	class AActor* DialogInitiator = nullptr;
	class AActor* DialogMaster = nullptr;

	class UDialogComponent* OwningDialogComponent;

	UPROPERTY()
	class UDialogUnit* ActiveDialogUnit;
	
};
