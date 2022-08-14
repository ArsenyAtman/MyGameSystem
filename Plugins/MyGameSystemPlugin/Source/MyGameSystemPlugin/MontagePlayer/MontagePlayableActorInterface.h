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
 * Interface for actors with a montage player component.
 */
class MYGAMESYSTEM_API IMontagePlayableActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get a montage player component of this actor.
	 * @return The MontagePlayerComponent.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MontagePlayableActorInterface")
	class UMontagePlayerComponent* GetMontagePlayerComponent() const;
	virtual class UMontagePlayerComponent* GetMontagePlayerComponent_Implementation() const = 0;

	/**
	 * Get a skeletal mesh of this actor for montage playing.
	 * @return The SkeletalMesh of the actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MontagePlayableActorInterface")
	class USkeletalMeshComponent* GetSkeletalMeshForMontagePlaying() const;
	virtual class USkeletalMeshComponent* GetSkeletalMeshForMontagePlaying_Implementation() const = 0;
};
