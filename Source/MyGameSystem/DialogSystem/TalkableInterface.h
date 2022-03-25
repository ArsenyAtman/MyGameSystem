// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TalkableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTalkableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API ITalkableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TalkableInterface")
	class UDialogComponent* GetDialogComponent();
	virtual class UDialogComponent* GetDialogComponent_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TalkableInterface")
	class USkeletalMeshComponent* GetSkeletalMeshForDialog();
	virtual class USkeletalMeshComponent* GetSkeletalMeshForDialog_Implementation() = 0;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//class UAudioComponent* GetAudioComponent();
	//virtual class UAudioComponent* GetAudioComponent_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TalkableInterface")
	class USceneComponent* GetMouthComponent();
	virtual class USceneComponent* GetMouthComponent_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TalkableInterface")
	FName GetMouthPoint();
	virtual FName GetMouthPoint_Implementation() { return "None"; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TalkableInterface")
	FVector GetMouthRelativeLocation();
	virtual FVector GetMouthRelativeLocation_Implementation() { return FVector(); }

};
