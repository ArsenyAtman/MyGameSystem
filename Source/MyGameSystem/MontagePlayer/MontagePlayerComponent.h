// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontagePlayerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UMontagePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMontagePlayerComponent();

	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAnimInstance* PlayAnimationMontageReplicated(class UAnimMontage* Montage, float InPlayRate = 1.0f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float StartTime = 0.0f, bool bStopAllMontages = true);

	UFUNCTION(BlueprintCallable, Category = "MontagePlayerComponent")
	class UAnimInstance* PlayAnimationMontageLocally(class UAnimMontage* Montage, float InPlayRate = 1.0f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float StartTime = 0.0f, bool bStopAllMontages = true);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void PlayAnimationMontageOnClients(class UAnimMontage* Montage, float InPlayRate = 1.0f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float StartTime = 0.0f, bool bStopAllMontages = true);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "MontagePlayerComponent")
	void StopAnimationMontage(class UAnimMontage* Montage);
		
};
