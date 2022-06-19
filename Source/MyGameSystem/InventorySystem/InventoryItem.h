// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReplicableObject.h"
#include "InventoryItem.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y = 0;

	FPoint()
	{
		X = 0;
		Y = 0;
	}

	FPoint(int x, int y)
	{
		X = x;
		Y = y;
	}

	FPoint operator + (FPoint Point)
	{
		return FPoint(this->X + Point.X, this->Y + Point.Y);
	}

	FPoint operator - (FPoint Point)
	{
		return FPoint(this->X - Point.X, this->Y - Point.Y);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FSocketInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* Component;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform Transform;

	FSocketInfo()
	{
		Component = nullptr;
		Name = "None";
		Transform = FTransform::Identity;
	}
};

UCLASS()
class MYGAMESYSTEM_API UInventoryItem : public UReplicableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class AInstance* InstanceOnMesh(class USceneComponent* Mesh = nullptr, FName Socket = "None");
	virtual class AInstance* InstanceOnMesh_Implementation(class USceneComponent* Mesh = nullptr, FName Socket = "None");

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitializeInActor(class AInstance* InstanceActor);

	virtual void InitializeInActor_Implementation(class AInstance* InstanceActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class AInstance* Spawn(FTransform Transform, bool bUseCurrentTransform = false);
	virtual class AInstance* Spawn_Implementation(FTransform Transform, bool bUseCurrentTransform = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyInstance();
	virtual void DestroyInstance_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroySubinstances();
	virtual void DestroySubinstances_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryItem* RemoveSelf();
	virtual UInventoryItem* RemoveSelf_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Place(FPoint NewLocation, class UPlace* NewOwningPlace);
	virtual bool Place_Implementation(FPoint NewLocation, class UPlace* NewOwningPlace);

	UFUNCTION(BlueprintSetter)
	void SetOwningPlace(class UPlace* NewOwningPlace);

	UFUNCTION(BlueprintGetter)
	FPoint GetSizeOnGrid();

	UFUNCTION(BlueprintGetter)
	int GetSizeOnLine();

	UFUNCTION(BlueprintGetter)
	FPoint GetLocation();

	UFUNCTION(BlueprintCallable)
	void Move(FPoint DeltaLocation);

	UFUNCTION(BlueprintGetter)
	class UStaticMesh* GetStaticMesh();

	UFUNCTION(BlueprintGetter)
	class USkeletalMesh* GetSkeletalMesh();

	UFUNCTION(BlueprintCallable)
	FSocketInfo GetSocketInfo(FName SocketName);

protected:

	UFUNCTION(BlueprintSetter)
	void SetLocation(FPoint NewLocation);

	UFUNCTION(BlueprintGetter)
	class UPlace* GetOwningPlace();

	UFUNCTION(BlueprintGetter)
	class AInstance* GetInstance() { return Instance; }

private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetLocation, BlueprintSetter = SetLocation, meta=(AllowPrivateAccess = true, ExposeOnSpawn = true))
	FPoint Location;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSizeOnGrid, meta = (AllowPrivateAccess = true))
	FPoint SizeOnGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSizeOnLine, meta = (AllowPrivateAccess = true))
	int SizeOnLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FName Name = FName("None");
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AInstance> InstanceClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class APickableInstance> PickableInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UTexture2D* Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStaticMesh, meta = (AllowPrivateAccess = true))
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSkeletalMesh, meta = (AllowPrivateAccess = true))
	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetInstance, meta=(AllowPrivateAccess = true, ExposeOnSpawn = true))
	class AInstance* Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true, ExposeOnSpawn = true))
	class UPlace* OwningPlace;

};
