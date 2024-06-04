// Copyright(c) Microsoft Corporation. All rights reserved.

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XAsync.h>
#include <XTaskQueue.h>
#include <XUser.h>
#include <XGameSave.h>

extern "C" 
{

/*
GameSaveFiles is a set of API's that enable reading and writing data on behalf of a user / game to be
persisted across game sessions and seemlessly synced with the cloud to be used across multiple devices.

This is a variation of the XGameSave API specifically designed to be used with Simple File based access
once the gamesave path has been retrieved.
see XGameSave.h for constants and error codes 
*/


STDAPI XGameSaveFilesGetFolderWithUiAsync(_In_ XUserHandle requestingUser, _In_z_ const char* configurationId, _In_ XAsyncBlock* async) noexcept;
STDAPI XGameSaveFilesGetFolderWithUiResult(_In_ XAsyncBlock* async, _In_ size_t folderSize, _Out_writes_bytes_(folderSize) char* folderResult) noexcept;

// Quota checks
STDAPI XGameSaveFilesGetRemainingQuota(_In_ XUserHandle userContext, _In_z_ const char* configurationId, _Out_ int64_t* remainingQuota) noexcept;

} // extern "C"
