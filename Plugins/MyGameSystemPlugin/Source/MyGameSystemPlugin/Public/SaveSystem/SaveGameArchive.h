// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

/**
 * An archive for object data serialization/deserialization during a saving/loading process.
 */
struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
    FSaveGameArchive(FArchive& InInnerArchive) : FObjectAndNameAsStringProxyArchive(InInnerArchive,true)
    {
        ArIsSaveGame = true;
        ArNoDelta = true;
    }
};
