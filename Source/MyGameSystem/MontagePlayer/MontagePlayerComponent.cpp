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

	// ...
}

UAnimInstance* UMontagePlayerComponent::PlayAnimationMontageReplicated(UAnimMontage* Montage, float InPlayRate, EMontagePlayReturnType ReturnValueType, float StartTime, bool bStopAllMontages)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		PlayAnimationMontageOnClients(Montage, InPlayRate, ReturnValueType, StartTime, bStopAllMontages);
		return PlayAnimationMontageLocally(Montage, InPlayRate, ReturnValueType, StartTime, bStopAllMontages);
	}
	return nullptr;
}

UAnimInstance* UMontagePlayerComponent::PlayAnimationMontageLocally(UAnimMontage* Montage, float InPlayRate, EMontagePlayReturnType ReturnValueType, float StartTime, bool bStopAllMontages)
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		if (Owner->Implements<UMontagePlayableActorInterface>())
		{
			USkeletalMeshComponent* SkeletalMeshComponent = IMontagePlayableActorInterface::Execute_GetSkeletalMeshForMontagePlaying(Owner);
			if (IsValid(SkeletalMeshComponent))
			{
				UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
				if (IsValid(AnimInstance))
				{
					AnimInstance->Montage_Play(Montage, InPlayRate, ReturnValueType, StartTime, bStopAllMontages);
					return AnimInstance;
				}
			}
		}
	}
	return nullptr;
}

void UMontagePlayerComponent::PlayAnimationMontageOnClients_Implementation(UAnimMontage* Montage, float InPlayRate, EMontagePlayReturnType ReturnValueType, float StartTime, bool bStopAllMontages)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		PlayAnimationMontageLocally(Montage, InPlayRate, ReturnValueType, StartTime, bStopAllMontages);
	}
}

void UMontagePlayerComponent::StopAnimationMontage_Implementation(class UAnimMontage* Montage)
{
	if (IsValid(Montage))
	{
		AActor* Owner = GetOwner();
		if (IsValid(Owner))
		{
			if (Owner->Implements<UMontagePlayableActorInterface>())
			{
				USkeletalMeshComponent* SkeletalMeshComponent = IMontagePlayableActorInterface::Execute_GetSkeletalMeshForMontagePlaying(Owner);
				if (IsValid(SkeletalMeshComponent))
				{
					UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
					if (IsValid(AnimInstance))
					{
						AnimInstance->Montage_Stop(Montage->GetDefaultBlendOutTime(), Montage);
					}
				}
			}
		}
	}
}
