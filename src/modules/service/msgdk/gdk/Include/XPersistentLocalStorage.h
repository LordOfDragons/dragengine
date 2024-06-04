// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <XAsync.h>
#include <XPackage.h>

extern "C"
{

struct XPersistentLocalStorageSpaceInfo
{
    // Bytes that are available to be written to PLS.
    uint64_t availableFreeBytes;

    // Bytes left in PLS allocation. May require prompting user to free up
    // space to make these bytes available.
    uint64_t totalFreeBytes;

    // Bytes already used in PLS.
    uint64_t usedBytes;

    // Maximum total bytes that can be stored in PLS.
    uint64_t totalBytes;
};

STDAPI XPersistentLocalStorageGetPathSize(
    _Out_ size_t* pathSize
    ) noexcept;

STDAPI XPersistentLocalStorageGetPath(
    _In_ size_t pathSize,
    _Out_writes_to_(pathSize, *pathUsed) char* path,
    _Out_opt_ size_t* pathUsed
    ) noexcept;

STDAPI XPersistentLocalStorageGetSpaceInfo(
    _Out_ XPersistentLocalStorageSpaceInfo* spaceInfo
    ) noexcept;

STDAPI XPersistentLocalStoragePromptUserForSpaceAsync(
    _In_ uint64_t requestedBytes,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

STDAPI XPersistentLocalStoragePromptUserForSpaceResult(
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

STDAPI XPersistentLocalStorageMountForPackage(
    _In_z_ const char* packageIdentifier,
    _Out_ XPackageMountHandle* mountHandle
    ) noexcept;
}
