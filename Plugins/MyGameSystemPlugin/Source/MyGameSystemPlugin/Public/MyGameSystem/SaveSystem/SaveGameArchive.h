// Fill out your copyright notice in the Description page of Project Settings.

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

/**
 * 
 */
struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
    FSaveGameArchive(FArchive& InInnerArchive) : FObjectAndNameAsStringProxyArchive(InInnerArchive,true)
    {
        ArIsSaveGame = true;
        ArNoDelta = true;
    }
};
