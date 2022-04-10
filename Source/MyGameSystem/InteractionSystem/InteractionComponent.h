// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionAvailableDelegate, FText, Descriprion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionUnavailableDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InteractionComponent|Interaction")
	void Interact();

	UFUNCTION(BlueprintSetter)
	void SetIsTracing(bool NewIsTracing);

	UFUNCTION(BlueprintGetter)
	bool GetIsTracing() const { return bIsTracing; }

	UFUNCTION(BlueprintPure)
	bool CanInteract() const;

	UPROPERTY(BlueprintAssignable)
	FInteractionAvailableDelegate OnInteractionAvailable;

	UPROPERTY(BlueprintAssignable)
	FInteractionUnavailableDelegate OnInteractionUnavailable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent|Internal")
	void StartTracing();

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent|Internal")
	void EndTracing();

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent|Internal")
	void Trace();

	UFUNCTION(BlueprintSetter)
	void SetActorForInteraction(class AActor* Actor);

	UFUNCTION(BlueprintGetter)
	class AActor* GetActorForInteraction() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing")
	float TraceInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing")
	float TraceLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing")
	float TraceRadius = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Debug")
	bool bDrawDebugTrace = false;

private:

	UFUNCTION(Client, Reliable)
	void InteractionAvailable(const FText& Description);

	UFUNCTION(Client, Reliable)
	void InteractionUnavailable();

	UPROPERTY(EditAnywhere, BlueprintSetter = SetIsTracing, BlueprintGetter = GetIsTracing, Replicated, meta = (AllowPrivateAccess = true), Category = "InteractionComponent|Tracing")
	bool bIsTracing = true;

	FTimerHandle TraceTimer;

	class AActor* ActorForInteraction;
		
};
