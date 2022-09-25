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
	void BeforeSave_Implementation() { return; }

	/**
	 * Called after the saving process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AfterSave();
	void AfterSave_Implementation() { return; }

	/**
	 * Called before the loading process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeforeLoad();
	void BeforeLoad_Implementation() { return; }

	/**
	 * Called after the loading process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AfterLoad();
	void AfterLoad_Implementation() { return; }

};
