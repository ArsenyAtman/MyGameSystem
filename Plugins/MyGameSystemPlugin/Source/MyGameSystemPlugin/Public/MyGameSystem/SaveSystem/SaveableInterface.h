// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveableInterface.generated.h"

UINTERFACE(MinimalAPI)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for saveable objects.
 */
class MYGAMESYSTEMPLUGIN_API ISaveableInterface
{
	GENERATED_BODY()

public:

	/**
	 * Is this object saving now.
	 * @return Is saving.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsSaving() const;
	virtual bool GetIsSaving_Implementation() const { return bIsSaving; }

	/**
	 * Is this object loading now.
	 * @return Is loading.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsLoading() const;
	virtual bool GetIsLoading_Implementation() const { return bIsLoading; }

	/**
	 * Should this object be saved.
	 * @return Should be saved.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldBeSaved();
	bool ShouldBeSaved_Implementation() { return false; }

	/**
	 * Called before the saving process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeforeSave();
	void BeforeSave_Implementation() { bIsSaving = true; }

	/**
	 * Called after the saving process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AfterSave();
	void AfterSave_Implementation() { bIsSaving = false; }

	/**
	 * Called before the loading process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeforeLoad();
	void BeforeLoad_Implementation() { bIsLoading = true; }

	/**
	 * Called after the loading process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AfterLoad();
	void AfterLoad_Implementation() { bIsLoading = false; }

private:

	bool bIsSaving = false;
	bool bIsLoading = false;

};
