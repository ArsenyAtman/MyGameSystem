// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontagePlayerComponent.generated.h"

/**
 * An actor component for playing animation montages over the network.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEMPLUGIN_API UMontagePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMontagePlayerComponent();

	/**
	 * Play a new animation montage on all machines.
	 * @param Montage - The new animation to play.
	 * @param InPlayRate - The rate of the animation.
	 * @param StartTime - The start time of the animation
	 * @param bStopAllMontages - Whether or not to stop all the currently playing montages.
	 * @return A new playing animation instance of the new AnimMontage.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAnimInstance* PlayAnimationMontageReplicated(class UAnimMontage* Montage, float InPlayRate = 1.0f, float StartTime = 0.0f, bool bStopAllMontages = true);

	/**
	 * Play a new animation on this machine.
	 * @param Montage - The new animation to play.
	 * @param InPlayRate - The rate of the animation.
	 * @param StartTime - The start time of the animation
	 * @param bStopAllMontages - Whether or not to stop all the currently playing montages.
	 * @return The new playing animation instance of the new AnimMontage.
	 */
	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAnimInstance* PlayAnimationMontageLocally(class UAnimMontage* Montage, float InPlayRate = 1.0f, float StartTime = 0.0f, bool bStopAllMontages = true);

	/**
	 * Play a new animation on clients.
	 * @param Montage - The new animation to play.
	 * @param InPlayRate - The rate of the animation.
	 * @param StartTime - The start time of the animation
	 * @param bStopAllMontages - Whether or not to stop all the currently playing montages.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void PlayAnimationMontageOnClients(class UAnimMontage* Montage, float InPlayRate = 1.0f, float StartTime = 0.0f, bool bStopAllMontages = true);

	/**
	 * Stop the currently playing animation montage on all machines.
	 * @param Montage - The animation to stop.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void StopAnimationMontage(class UAnimMontage* Montage);
		
};
