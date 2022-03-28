// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogUnit.h"
#include "DialogCue.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EDialogCueType : uint8
{
	Usual		UMETA(DisplayName = "Usual"),
	Additional	UMETA(DisplayName = "Additional"),
	Important	UMETA(DisplayName = "Important")
};

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCueData : public UDialogUnitData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDialogUnit> NextDialogUnit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogCueType DialogCueType = EDialogCueType::Usual;
};

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogCue : public UDialogUnit
{
	GENERATED_BODY()

public:

	virtual void Activate_Implementation(class UDialog* OwnDialog) override;

	virtual class UDialogCueData* GetDialogUnitData_Implementation() override { return CueData; }

	UFUNCTION(BlueprintCallable, Category = "DialogCue|Control")
	void PlayNextDialogCue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Availability")
	bool CheckAvailabilityCondition(class UDialog* CheckingDialog);
	virtual bool CheckAvailabilityCondition_Implementation(class UDialog* CheckingDialog);

	UFUNCTION(BlueprintGetter, Category = "DialogCue|Dialog")
	class UDialog* GetOwningDialog() { return OwningDialog; }

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control")
	void OnCueBeginned();
	virtual void OnCueBeginned_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DialogCue|Control")
	void OnCueEnded();
	virtual void OnCueEnded_Implementation();

	UFUNCTION(BlueprintCallable, Category = "DialogCue|Internal")
	void EndCue();

protected:

	// ...

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogCue|Data", meta = (AllowPrivateAccess = true))
	class UDialogCueData* CueData;

	UPROPERTY(BlueprintGetter = GetOwningDialog)
	class UDialog* OwningDialog;

};
