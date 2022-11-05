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
class MYSAVESYSTEMPLUGIN_API ISaveableInterface
{
	GENERATED_BODY()

public:

	/**
	 * Called before the saving process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveableInterface")
	void BeforeSave();
	void BeforeSave_Implementation() { return; }

	/**
	 * Called after the saving process (and saving of all subobjects).
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveableInterface")
	void AfterSave();
	void AfterSave_Implementation() { return; }

	/**
	 * Called before the loading process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveableInterface")
	void BeforeLoad();
	void BeforeLoad_Implementation() { return; }

	/**
	 * Called after the loading process (and loading of all subobjects, but before the reattachment (of attached actors)!).
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveableInterface")
	void AfterLoad();
	void AfterLoad_Implementation() { return; }

};
