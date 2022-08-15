// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

/**
 * Delegate for situations where an interactable becomes available.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionAvailableDelegate, FText, Descriprion);

/**
 * Delegate for situations where an interactable becomes unavailable.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionUnavailableDelegate);

/**
 * ActorComponent for interactions handling.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEMPLUGIN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	// Override for the replication setup.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Interact with a possible interactable.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InteractionComponent|Interaction")
	void Interact();

	/**
	 * Set is tracing for an interactable.
	 * @param NewIsTracing - Trace or not.
	 */
	UFUNCTION(BlueprintSetter, Category = "InteractionComponent|Tracing")
	void SetIsTracing(bool NewIsTracing);

	/**
	 * Get is tracing for an interactable now.
	 * @return Is tracing.
	 */
	UFUNCTION(BlueprintGetter, Category = "InteractionComponent|Tracing")
	bool GetIsTracing() const { return bIsTracing; }

	/**
	 * Can interact with a possible interactable.
	 * @return Can interact.
	 */
	UFUNCTION(BlueprintPure, Category = "InteractionComponent|Interaction")
	bool CanInteract() const;

	/**
	 * Called when an interactable is found and the owner can interact with it.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InteractionComponent|Delegates")
	FInteractionAvailableDelegate OnInteractionAvailable;

	/**
	 * Called when an interactable is lost or the owner can't interact with it.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InteractionComponent|Delegates")
	FInteractionUnavailableDelegate OnInteractionUnavailable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Start tracing for interactables.
	 */
	UFUNCTION(BlueprintCallable, Category = "InteractionComponent|Internal", meta = (BlueprintProtected))
	void StartTracing();

	/**
	 * End tracing for interactables.
	 */
	UFUNCTION(BlueprintCallable, Category = "InteractionComponent|Internal", meta = (BlueprintProtected))
	void EndTracing();

	/**
	 * A single trace logic.
	 */
	UFUNCTION(BlueprintCallable, Category = "InteractionComponent|Internal", meta = (BlueprintProtected))
	void Trace();

	/**
	 * Set a possible interactable.
	 * @param Actor - A new interactable.
	 */
	UFUNCTION(BlueprintSetter, Category = "InteractionComponent|Internal", meta = (BlueprintProtected))
	void SetActorForInteraction(class AActor* Actor);

	/**
	 * Get a possible interactable.
	 * @return A possible interactable.
	 */
	UFUNCTION(BlueprintGetter, Category = "InteractionComponent|Interactable", meta = (BlueprintProtected))
	class AActor* GetActorForInteraction() const;

	/**
	 * An interval between sphere traces.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing", meta = (BlueprintProtected))
	float TraceInterval = 1.0f;

	/**
	 * A length of the tracing line.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing", meta = (BlueprintProtected))
	float TraceLength = 300.0f;

	/**
	 * A radius of the tracing sphere.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing", meta = (BlueprintProtected))
	float TraceRadius = 15.0f;

	/**
	 * A collision channel for the tracing process.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Tracing", meta = (BlueprintProtected))
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel = ECollisionChannel::ECC_Visibility;

	/**
	 * Whether or not to draw debug lines.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractionComponent|Debug", meta = (BlueprintProtected))
	bool bDrawDebugTrace = false;

private:

	/**
	 * Notify the owning client of a possible interaction.
	 */
	UFUNCTION(Client, Reliable)
	void InteractionAvailable(const FText& Description);

	/**
	 * Notify the owning client that interactions are currently unavailable.
	 */
	UFUNCTION(Client, Reliable)
	void InteractionUnavailable();

	/**
	 * Is tracing for interactables.
	 */
	UPROPERTY(EditAnywhere, BlueprintSetter = SetIsTracing, BlueprintGetter = GetIsTracing, Replicated, meta = (AllowPrivateAccess = true), Category = "InteractionComponent|Tracing")
	bool bIsTracing = true;

	// Timer for a delay between single traces.
	FTimerHandle TraceTimer;

	// Current interactable actor.
	class AActor* ActorForInteraction;
		
};
