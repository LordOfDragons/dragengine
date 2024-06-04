// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XAsync.h>
#include <XGameRuntimeTypes.h>

extern "C"
{

// A package identifier is a string that identifies a packaged piece of 
// content. It is durable across version updates but unique to a process.
// It's not intended to be human-readable.
#define XPACKAGE_IDENTIFIER_MAX_LENGTH (33)

enum class XPackageChunkSelectorType : uint32_t
{
    Language,
    Tag,
    Chunk,
    Feature
};

enum class XPackageChunkAvailability : uint32_t
{
    Ready,
    Pending,
    Installable,
    Unavailable
};

enum class XPackageKind : uint32_t
{
    Game,
    Content
};

enum class XPackageEnumerationScope : uint32_t
{
    ThisOnly,
    ThisAndRelated,
    ThisPublisher
};

struct XPackageChunkSelector
{
    XPackageChunkSelectorType type;
    union
    {
        _Field_z_ const char* language;
        _Field_z_ const char* tag;
        _Field_z_ const char* feature;
        uint32_t chunkId;
    };
};

struct XPackageInstallationProgress
{
    uint64_t totalBytes;
    uint64_t installedBytes;
    uint64_t launchBytes;
    bool launchable;
    bool completed;
};

struct XPackageDetails
{
    _Field_z_ const char* packageIdentifier;
    XVersion version;
    XPackageKind kind;
    _Field_z_ const char* displayName;
    _Field_z_ const char* description;
    _Field_z_ const char* publisher;
    _Field_z_ const char* storeId;
    bool installing;
    uint32_t index;
    uint32_t count;
    bool ageRestricted;
    _Field_z_ const char* titleId;
};

struct XPackageFeature
{
    _Field_z_ const char* id;
    _Field_z_ const char* displayName;
    _Field_z_ const char* tags;
    bool hidden;
    uint32_t storeIdCount;
    _Field_z_ const char** storeIds;
};

struct XPackageWriteStats
{
    uint64_t interval;
    uint64_t budget;
    uint64_t elapsed;
    uint64_t bytesWritten;
};

typedef struct XPackageInstallationMonitor* XPackageInstallationMonitorHandle;
typedef struct XPackageMountInstance* XPackageMountHandle;

typedef bool CALLBACK XPackageEnumerationCallback(_In_ void* context, _In_ const XPackageDetails* details);
typedef bool CALLBACK XPackageChunkAvailabilityCallback(_In_ void* context, _In_ const XPackageChunkSelector* selector, _In_ XPackageChunkAvailability availability);
typedef bool CALLBACK XPackageFeatureEnumerationCallback(_In_ void* context, _In_ const XPackageFeature* feature);

typedef void CALLBACK XPackageInstalledCallback(_In_ void* context, _In_ const XPackageDetails* details);
typedef void CALLBACK XPackageInstallationProgressCallback(_In_ void* context, _In_ XPackageInstallationMonitorHandle monitor);

STDAPI XPackageGetCurrentProcessPackageIdentifier(
    _In_ size_t bufferSize,
    _Out_writes_(bufferSize) char* buffer
    ) noexcept;

STDAPI_(bool) XPackageIsPackagedProcess() noexcept;

STDAPI XPackageCreateInstallationMonitor(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_opt_(selectorCount) XPackageChunkSelector* selectors,
    _In_ uint32_t minimumUpdateIntervalMs, // 0 == no update
    _In_opt_ XTaskQueueHandle queue,
    _Out_ XPackageInstallationMonitorHandle* installationMonitor
    ) noexcept;

STDAPI_(void) XPackageCloseInstallationMonitorHandle(
    _In_ XPackageInstallationMonitorHandle installationMonitor
    ) noexcept;

STDAPI_(void) XPackageGetInstallationProgress(
    _In_ XPackageInstallationMonitorHandle installationMonitor,
    _Out_ XPackageInstallationProgress* progress
    ) noexcept;

STDAPI_(bool) XPackageUpdateInstallationMonitor(
    _In_ XPackageInstallationMonitorHandle installationMonitor
    ) noexcept;

STDAPI XPackageRegisterInstallationProgressChanged(
    _In_ XPackageInstallationMonitorHandle installationMonitor,
    _In_opt_ void* context,
    _In_ XPackageInstallationProgressCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XPackageUnregisterInstallationProgressChanged(
    _In_ XPackageInstallationMonitorHandle installationMonitor,
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

STDAPI XPackageGetUserLocale(
    _In_ size_t localeSize,
    _Out_writes_(localeSize) char* locale
    ) noexcept;

STDAPI XPackageFindChunkAvailability(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_opt_(selectorCount) XPackageChunkSelector* selectors,
    _Out_ XPackageChunkAvailability* availability
    ) noexcept;

STDAPI XPackageEnumerateChunkAvailability(
    _In_z_ const char* packageIdentifier,
    _In_ XPackageChunkSelectorType type,
    _In_ void* context,
    _In_ XPackageChunkAvailabilityCallback* callback
    ) noexcept;

STDAPI XPackageChangeChunkInstallOrder(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_(selectorCount) XPackageChunkSelector* selectors
    ) noexcept;

STDAPI XPackageInstallChunks(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_(selectorCount) XPackageChunkSelector* selectors,
    _In_ uint32_t minimumUpdateIntervalMs,
    _In_ bool suppressUserConfirmation,
    _In_opt_ XTaskQueueHandle queue,
    _Out_ XPackageInstallationMonitorHandle* installationMonitor
    ) noexcept;

STDAPI XPackageInstallChunksAsync(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_(selectorCount) XPackageChunkSelector* selectors,
    _In_ uint32_t minimumUpdateIntervalMs,
    _In_ bool suppressUserConfirmation,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

STDAPI XPackageInstallChunksResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _Out_ XPackageInstallationMonitorHandle* installationMonitor
    ) noexcept;

STDAPI XPackageEstimateDownloadSize(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_(selectorCount) XPackageChunkSelector* selectors,
    _Out_ uint64_t* downloadSize,
    _Out_opt_ bool* shouldPresentUserConfirmation
    ) noexcept;

STDAPI XPackageUninstallChunks(
    _In_z_ const char* packageIdentifier,
    _In_ uint32_t selectorCount,
    _In_reads_(selectorCount) XPackageChunkSelector* selectors
    ) noexcept;

STDAPI XPackageEnumeratePackages(
    _In_ XPackageKind kind,
    _In_ XPackageEnumerationScope scope,
    _In_opt_ void* context,
    _In_ XPackageEnumerationCallback* callback
    ) noexcept;

STDAPI XPackageRegisterPackageInstalled(
    _In_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XPackageInstalledCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XPackageUnregisterPackageInstalled(
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

STDAPI XPackageEnumerateFeatures(
    _In_z_ const char* packageIdentifier,
    _In_opt_ void* context,
    _In_ XPackageFeatureEnumerationCallback* callback
    ) noexcept;

__declspec(deprecated("Use XPackageMountWithUiAsync instead."))
HRESULT STDAPICALLTYPE XPackageMount(
    _In_z_ const char* packageIdentifier,
    _Out_ XPackageMountHandle* mount
    ) noexcept;

STDAPI XPackageMountWithUiAsync(
    _In_z_ const char* packageIdentifier,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XPackageMountWithUiResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XPackageMountHandle* mount
    ) noexcept;

STDAPI XPackageGetMountPathSize(
    _In_ XPackageMountHandle mount,
    _Out_ size_t* pathSize
    ) noexcept;

STDAPI XPackageGetMountPath(
    _In_ XPackageMountHandle mount,
    _In_ size_t pathSize,
    _Out_writes_(pathSize) char* path
    ) noexcept;

STDAPI_(void) XPackageCloseMountHandle(
    _In_ XPackageMountHandle mount
    ) noexcept;

STDAPI XPackageGetWriteStats(
    _Out_ XPackageWriteStats* writeStats
    ) noexcept;

STDAPI XPackageUninstallUWPInstance(
    _In_z_ const char* packageName
    ) noexcept;

STDAPI_(bool) XPackageUninstallPackage(
    _In_z_ const char* packageIdentifier
    ) noexcept;
}
