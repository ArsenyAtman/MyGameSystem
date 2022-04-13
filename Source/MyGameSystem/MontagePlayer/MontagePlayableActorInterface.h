// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MontagePlayableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMontagePlayableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IMontagePlayableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MontagePlayableActorInterface")
	class UMontagePlayerComponent* GetMontagePlayerComponent() const;
	virtual class UMontagePlayerComponent* GetMontagePlayerComponent_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MontagePlayableActorInterface")
	class USkeletalMeshComponent* GetSkeletalMeshForMontagePlaying() const;
	virtual class USkeletalMeshComponent* GetSkeletalMeshForMontagePlaying_Implementation() const = 0;
};
