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
	void Begin(class UDialogComponent* OwnDialogComponent, class AActor* Master, class AActor* Initiator, const TArray<class AActor*>& OtherInterlocutors);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void OnDialogUnitPassed(class UDialogUnit* DialogUnit, TSubclassOf<UDialogUnit> NextDialogUnitClass);

	UFUNCTION(BlueprintGetter, Category = "Dialog|CurrentDialogUnit")
	FORCEINLINE class UDialogUnit* GetCurrentDialogUnit() const {return ActiveDialogUnit; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	FORCEINLINE TArray<class AActor*> GetInterlocutors() const { return Interlocutors; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	FORCEINLINE class AActor* GetDialogMaster() const { return DialogMaster; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|Interlocutors")
	FORCEINLINE class AActor* GetDialogInitiator() const { return DialogInitiator; }

	UFUNCTION(BlueprintGetter, Category = "Dialog|OwningDialogComponent")
	FORCEINLINE class UDialogComponent* GetOwningDialogComponent() const { return OwningDialogComponent; }

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void BeginDialogForInterlocutors(class UDialogComponent* MasterDialogComponent);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void EndDialogForInterlocutors();

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void UnitStartedForInterlocutors(class UDialogUnit* DialogUnit);

	UFUNCTION(BlueprintCallable, Category = "Dialog|Internal")
	void UnitEndedForInterlocutors(class UDialogUnit* DialogUnit);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialog|InitialDialogUnit")
	TSubclassOf<UDialogUnit> InitialDialogUnit;

private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetInterlocutors)
	TArray<class AActor*> Interlocutors;

	UPROPERTY(BlueprintGetter = GetDialogInitiator)
	class AActor* DialogInitiator = nullptr;

	UPROPERTY(BlueprintGetter = GetDialogMaster)
	class AActor* DialogMaster = nullptr;

	UPROPERTY(BlueprintGetter = GetOwningDialogComponent)
	class UDialogComponent* OwningDialogComponent;

	UPROPERTY(BlueprintGetter = GetCurrentDialogUnit)
	class UDialogUnit* ActiveDialogUnit;
	
};
