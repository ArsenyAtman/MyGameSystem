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

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void Begin(class UDialogComponent* OwnDialogComponent, class AActor* Master, class AActor* Initiator, TArray<class AActor*> OtherInterlocutors);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void OnDialogUnitPassed(class UObject* DialogUnit, TSubclassOf<UObject> NextDialogUnitClass);

	UFUNCTION(BlueprintGetter, Category = "Dialog|CurrentDialogUnit")
	FORCEINLINE class UDialogUnit* GetCurrentDialogUnit() {return ActiveDialogUnit; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	FORCEINLINE TArray<class AActor*> GetInterlocutors() { return DialogInterlocutors; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	FORCEINLINE class AActor* GetDialogMaster() { return DialogMaster; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	FORCEINLINE class AActor* GetDialogInitiator() { return DialogInitiator; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|OwningDialogComponent")
	FORCEINLINE class UDialogComponent* GetOwningDialogComponent() { return OwningDialogComponent; }

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void BeginDialogForInterlocutors(class UDialogComponent* MasterDialogComponent);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void EndDialogForInterlocutors();

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void UnitStartedForInterlocutors(class UDialogUnit* DialogUnit);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void UnitEndedForInterlocutors(class UDialogUnit* DialogUnit);

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetInterlocutors, meta = (MustImplement = "TalkableInterface"), Category = "Dialog|Interlocutors")
	TArray<class AActor*> Interlocutors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialog|InitialDialogUnit")
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
