// Copyright(c) Microsoft Corporation. All rights reserved.

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XAsync.h>
#include <XTaskQueue.h>
#include <XUser.h>

extern "C" 
{

/*
GameSave is a set of API's that enable reading and writing data on behalf of a user / game to be
persisted across game sessions and seemlessly synced with the cloud to be used across multiple devices.

There are 3 major portions to the API.

1. gs_provider_t is the handle that binds a user context to a gamesave and is required for API interactions.
Each provider can contain multiple containers and has a default quota of 256MB.
    - Quota information
    - Container management
    - Provider Metadata (Service Configuration Id, UserContext)
2. Containers are used to logically correlate data into a group to be written / updated / read atomically.
    - Queries on Container info / status
    - Queries on container contents
    - Atomic updates to container data

3. Blobs are the actual storage data that is associated with a container, the actual data can be a maximum of 16MB.
    - blob update / delete are designed to be atomic so a set of changes can be committed all as one.  If any portion fails the whole update fails.
*/


typedef struct XGameSaveProvider* XGameSaveProviderHandle;
typedef struct XGameSaveContainer* XGameSaveContainerHandle;
typedef struct XGameSaveUpdate* XGameSaveUpdateHandle;

static const size_t GS_MAX_CONFIGURATION_ID_SIZE = 65;
static const size_t GS_MAX_CONTAINER_NAME_SIZE = 256;
static const size_t GS_MAX_CONTAINER_DISPLAY_NAME_SIZE = 128;
static const size_t GS_MAX_BLOB_NAME_SIZE = 65;
static const size_t GS_MAX_BLOB_SIZE = (1024 * 1024 * 16); // 16MB

static const HRESULT E_GS_INVALID_CONTAINER_NAME = 0x80830001;
static const HRESULT E_GS_NO_ACCESS = 0x80830002;
static const HRESULT E_GS_OUT_OF_LOCAL_STORAGE = 0x80830003;
static const HRESULT E_GS_USER_CANCELED = 0x80830004;
static const HRESULT E_GS_UPDATE_TOO_BIG = 0x80830005;
static const HRESULT E_GS_QUOTA_EXCEEDED = 0x80830006;
static const HRESULT E_GS_PROVIDED_BUFFER_TOO_SMALL = 0x80830007;
static const HRESULT E_GS_BLOB_NOT_FOUND = 0x80830008;
static const HRESULT E_GS_NO_SERVICE_CONFIGURATION = 0x80830009;
static const HRESULT E_GS_CONTAINER_NOT_IN_SYNC = 0x8083000A;
static const HRESULT E_GS_CONTAINER_SYNC_FAILED = 0x8083000B;
static const HRESULT E_GS_USER_NOT_REGISTERED_IN_SERVICE = 0x8083000C;
static const HRESULT E_GS_HANDLE_EXPIRED = 0x8083000D;
static const HRESULT E_GS_ASYNC_FUNCTION_REQUIRED = 0x8083000E;
static const HRESULT E_GS_PROVIDER_MISMATCH = 0x8083000F;

struct XGameSaveBlobInfo
{
    _Field_z_ const char* name;  // unique blob name (unique to container)
    uint32_t size;           // size of the saved data
};

struct XGameSaveBlob
{
    XGameSaveBlobInfo info;  // metadata
    uint8_t* data;           // saved data
};

struct XGameSaveContainerInfo
{
    _Field_z_ const char*  name;           // unique container name
    _Field_z_ const char*  displayName;    // display name
    uint32_t blobCount;      // number of blobs in the container
    uint64_t totalSize;      // size of all the blobs in the container
    time_t lastModifiedTime; // last time this container was updated
    bool needsSync;          // sync status, if not synced any operation on this container may result in a network call (if using SyncOnDemand) 
};

typedef bool CALLBACK XGameSaveBlobInfoCallback(_In_ const XGameSaveBlobInfo* info, _In_opt_ void* context);
typedef bool CALLBACK XGameSaveContainerInfoCallback(_In_ const XGameSaveContainerInfo* info, _In_opt_ void* context);


// A SyncOnDemand provider will sync the container metadata at initialize time
// it will not sync all the container data unless it is needed for an operation.
// Currently a container sync is a blocking UX operation.
STDAPI XGameSaveInitializeProvider(_In_ XUserHandle requestingUser, _In_z_ const char* configurationId, _In_ bool syncOnDemand, _Outptr_result_nullonfailure_ XGameSaveProviderHandle* provider) noexcept;
STDAPI XGameSaveInitializeProviderAsync(_In_ XUserHandle requestingUser, _In_z_ const char* configurationId, _In_ bool syncOnDemand, _In_ XAsyncBlock* async) noexcept;

STDAPI XGameSaveInitializeProviderResult(_In_ XAsyncBlock* async, _Outptr_result_nullonfailure_ XGameSaveProviderHandle* provider) noexcept;
STDAPI_(void) XGameSaveCloseProvider(_In_ XGameSaveProviderHandle provider) noexcept;


// Quota checks
STDAPI XGameSaveGetRemainingQuota(_In_ XGameSaveProviderHandle provider, _Out_ int64_t* remainingQuota) noexcept;
STDAPI XGameSaveGetRemainingQuotaAsync(_In_ XGameSaveProviderHandle provider, _In_ XAsyncBlock* async) noexcept;
STDAPI XGameSaveGetRemainingQuotaResult(_In_ XAsyncBlock* async, _Out_ int64_t* remainingQuota) noexcept;

// Container Management / Information
STDAPI XGameSaveDeleteContainer(_In_ XGameSaveProviderHandle provider, _In_z_ const char* containerName) noexcept;
STDAPI XGameSaveDeleteContainerAsync(_In_ XGameSaveProviderHandle provider, _In_z_ const char* containerName, _In_ XAsyncBlock* async) noexcept;
STDAPI XGameSaveDeleteContainerResult(_In_ XAsyncBlock* async) noexcept;

STDAPI XGameSaveGetContainerInfo(_In_ XGameSaveProviderHandle provider, _In_z_ const char* containerName, _In_opt_ void* context, _In_ XGameSaveContainerInfoCallback* callback) noexcept;
STDAPI XGameSaveEnumerateContainerInfo(_In_ XGameSaveProviderHandle provider, _In_opt_ void* context, _In_ XGameSaveContainerInfoCallback* callback) noexcept;
STDAPI XGameSaveEnumerateContainerInfoByName(_In_ XGameSaveProviderHandle provider, _In_opt_z_ const char* containerNamePrefix, _In_opt_ void* context, _In_ XGameSaveContainerInfoCallback* callback) noexcept;

STDAPI XGameSaveCreateContainer(_In_ XGameSaveProviderHandle provider, _In_z_ const char* containerName, _Outptr_result_nullonfailure_ XGameSaveContainerHandle* containerContext) noexcept;
STDAPI_(void) XGameSaveCloseContainer(_In_ XGameSaveContainerHandle context) noexcept;

// Available Blob Information
STDAPI XGameSaveEnumerateBlobInfo(_In_ XGameSaveContainerHandle container, _In_opt_ void* context, _In_ XGameSaveBlobInfoCallback* callback) noexcept;
STDAPI XGameSaveEnumerateBlobInfoByName(_In_ XGameSaveContainerHandle container, _In_opt_z_ const char* blobNamePrefix, _In_opt_ void* context, _In_ XGameSaveBlobInfoCallback* callback) noexcept;

// Read Blob Data
STDAPI XGameSaveReadBlobData(_In_ XGameSaveContainerHandle container, _In_opt_z_count_(*countOfBlobs) const char** blobNames, _Inout_ uint32_t* countOfBlobs, _In_ size_t blobsSize, _Out_writes_bytes_(blobsSize) XGameSaveBlob* blobData) noexcept;
STDAPI XGameSaveReadBlobDataAsync(_In_ XGameSaveContainerHandle container, _In_opt_z_count_(countOfBlobs) const char** blobNames, _In_ uint32_t countOfBlobs, _In_ XAsyncBlock* async) noexcept;
STDAPI XGameSaveReadBlobDataResult(_In_ XAsyncBlock* async, _In_ size_t blobsSize, _Out_writes_bytes_(blobsSize) XGameSaveBlob* blobData, _Out_ uint32_t* countOfBlobs) noexcept;


// Update / Write Blob Data
STDAPI XGameSaveCreateUpdate(_In_ XGameSaveContainerHandle container, _In_z_ const char* containerDisplayName, _Outptr_result_nullonfailure_ XGameSaveUpdateHandle* updateContext) noexcept;
STDAPI_(void) XGameSaveCloseUpdate(_In_ XGameSaveUpdateHandle context) noexcept;
STDAPI XGameSaveSubmitBlobWrite(_In_ XGameSaveUpdateHandle updateContext, _In_z_ const char* blobName, _In_reads_bytes_(byteCount) const uint8_t* data, _In_ size_t byteCount) noexcept;
STDAPI XGameSaveSubmitBlobDelete(_In_ XGameSaveUpdateHandle updateContext, _In_z_ const char* blobName) noexcept;
STDAPI XGameSaveSubmitUpdate(_In_ XGameSaveUpdateHandle updateContext) noexcept;
STDAPI XGameSaveSubmitUpdateAsync(_In_ XGameSaveUpdateHandle updateContext, _In_ XAsyncBlock* async) noexcept;
STDAPI XGameSaveSubmitUpdateResult(_In_ XAsyncBlock* async) noexcept;

} // extern "C"
