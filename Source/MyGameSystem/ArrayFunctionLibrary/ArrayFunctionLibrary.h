// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArrayFunctionLibrary.generated.h"

/**
 * Simple functions and operations for arrays.
 */
UCLASS()
class MYGAMESYSTEM_API UArrayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	 * Find missing elements from the second array in the first (Array1 - Array2).
	 * @param InArray - The source array of elements.
	 * @param FromArray - Elements to search.
	 * @return Missing elements.
	 */
	template<typename ElementType>
	static TArray<ElementType> FindMissing(const TArray<ElementType>& InArray, const TArray<ElementType>& FromArray);
};

template<typename ElementType>
static TArray<ElementType> UArrayFunctionLibrary::FindMissing(const TArray<ElementType>& InArray, const TArray<ElementType>& FromArray)
{
	TArray<ElementType> MissingElements;

	for(ElementType Element : FromArray)
	{
		if(InArray.Find(Element) == INDEX_NONE)
		{
			MissingElements.Add(Element);
		}
	}

	return MissingElements;
}
