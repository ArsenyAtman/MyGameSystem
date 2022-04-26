// Fill out your copyright notice in the Description page of Project Settings.


#include "MontagePlayerComponent.h"
#include "MontagePlayableActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMontagePlayerComponent::UMontagePlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// This component is replicated by default.
	SetIsReplicatedByDefault(true);
	
}

UAnimInstance* UMontagePlayerComponent::PlayAnimationMontageReplicated(UAnimMontage* Montage, float InPlayRate, float StartTime, bool bStopAllMontages)
{
	// If this machine has authority...
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		// than play tha new montage on all clients, ...
		PlayAnimationMontageOnClients(Montage, InPlayRate, StartTime, bStopAllMontages);
		// and return a new animation instance for this machine.
		return PlayAnimationMontageLocally(Montage, InPlayRate, StartTime, bStopAllMontages);
	}
	return nullptr;
}

UAnimInstance* UMontagePlayerComponent::PlayAnimationMontageLocally(UAnimMontage* Montage, float InPlayRate, float StartTime, bool bStopAllMontages)
{
	// If the owner exists and can play animation montages...
	AActor* Owner = GetOwner();
	if (IsValid(Owner) && Owner->Implements<UMontagePlayableActorInterface>())
	{
		USkeletalMeshComponent* SkeletalMeshComponent = IMontagePlayableActorInterface::Execute_GetSkeletalMeshForMontagePlaying(Owner);
		if (IsValid(SkeletalMeshComponent))
		{
			UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
			if (IsValid(AnimInstance))
			{
				// than play the new montage.
				AnimInstance->Montage_Play(Montage, InPlayRate, EMontagePlayReturnType::Duration, StartTime, bStopAllMontages);
				return AnimInstance;
			}
		}
	}
	return nullptr;
}

void UMontagePlayerComponent::PlayAnimationMontageOnClients_Implementation(UAnimMontage* Montage, float InPlayRate, float StartTime, bool bStopAllMontages)
{
	// If this machine is a client...
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		// than play the new montage.
		PlayAnimationMontageLocally(Montage, InPlayRate, StartTime, bStopAllMontages);
	}
}

void UMontagePlayerComponent::StopAnimationMontage_Implementation(class UAnimMontage* Montage)
{
	// If the Montage is valid ...
	if (IsValid(Montage))
	{
		AActor* Owner = GetOwner();
		if (IsValid(Owner) && Owner->Implements<UMontagePlayableActorInterface>())
		{
			USkeletalMeshComponent* SkeletalMeshComponent = IMontagePlayableActorInterface::Execute_GetSkeletalMeshForMontagePlaying(Owner);
			if (IsValid(SkeletalMeshComponent))
			{
				// and the owner's mesh has a valid animation instance...
				UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
				if (IsValid(AnimInstance))
				{
					// than stop the currently playing montage.
					AnimInstance->Montage_Stop(Montage->GetDefaultBlendOutTime(), Montage);
				}
			}
		}
	}
}
