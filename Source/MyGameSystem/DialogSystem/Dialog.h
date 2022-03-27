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

	UFUNCTION(BlueprintGetter)
	FORCEINLINE class UDialogUnit* GetCurrentDialogUnit() {return ActiveDialogUnit; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE TArray<class AActor*> GetInterlocutors() { return DialogInterlocutors; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE class AActor* GetDialogMaster() { return DialogMaster; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE class AActor* GetDialogInitiator() { return DialogInitiator; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE class UDialogComponent* GetOwningDialogComponent() { return OwningDialogComponent; }

	UFUNCTION(BlueprintCallable)
	void BeginDialogForInterlocutors(class UDialogComponent* MasterDialogComponent);

	UFUNCTION(BlueprintCallable)
	void EndDialogForInterlocutors();

	UFUNCTION(BlueprintCallable)
	void UnitStartedForInterlocutors(class UDialogUnit* DialogUnit);

	UFUNCTION(BlueprintCallable)
	void UnitEndedForInterlocutors(class UDialogUnit* DialogUnit);

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetInterlocutors, meta = (MustImplement = "TalkableInterface"))
	TArray<class AActor*> Interlocutors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MustImplement = "DialogUnitInterface"))
	TSubclassOf<UDialogUnit> InitialDialogUnit;

private:

	TArray<class AActor*> DialogInterlocutors;

	UPROPERTY(BlueprintGetter = GetDialogInitiator)
	class AActor* DialogInitiator = nullptr;

	UPROPERTY(BlueprintGetter = GetDialogMaster)
	class AActor* DialogMaster = nullptr;

	UPROPERTY(BlueprintGetter = GetOwningDialogComponent)
	class UDialogComponent* OwningDialogComponent;

	UPROPERTY(BlueprintGetter = GetCurrentDialogUnit)
	class UDialogUnit* ActiveDialogUnit;
	
};
