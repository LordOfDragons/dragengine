// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <XGameRuntimeTypes.h>

extern "C"
{

struct XSystemAnalyticsInfo
{
    XVersion osVersion;
    XVersion hostingOsVersion;
    _Field_z_ char family[64];
    _Field_z_ char form[64];
};

STDAPI_(XSystemAnalyticsInfo) XSystemGetAnalyticsInfo() noexcept;

const size_t XSystemConsoleIdBytes = 39;

STDAPI XSystemGetConsoleId(
    _In_ size_t consoleIdSize,
    _Out_writes_bytes_to_(consoleIdSize, *consoleIdUsed) char* consoleId,
    _Out_opt_ size_t* consoleIdUsed
    ) noexcept;


const size_t XSystemXboxLiveSandboxIdMaxBytes = 16;

STDAPI XSystemGetXboxLiveSandboxId(
    _In_ size_t sandboxIdSize,
    _Out_writes_bytes_to_(sandboxIdSize, *sandboxIdUsed) char* sandboxId,
    _Out_opt_ size_t* sandboxIdUsed
    ) noexcept;

enum class XSystemDeviceType : uint32_t {
    Unknown              = 0x00,
    Pc                   = 0x01,
    XboxOne              = 0x02,
    XboxOneS             = 0x03,
    XboxOneX             = 0x04,
    XboxOneXDevkit       = 0x05,
    XboxScarlettLockhart = 0x06,
    XboxScarlettAnaconda = 0x07,
    XboxScarlettDevkit   = 0x08
};

STDAPI_(XSystemDeviceType) XSystemGetDeviceType() noexcept;

const size_t XSystemAppSpecificDeviceIdBytes = 45;

STDAPI XSystemGetAppSpecificDeviceId(
    _In_ size_t appSpecificDeviceIdSize,
    _Out_writes_bytes_to_(appSpecificDeviceIdSize, *appSpecificDeviceIdUsed) char* appSpecificDeviceId,
    _Out_opt_ size_t* appSpecificDeviceIdUsed
    ) noexcept;


// Handle tracking.

enum class XSystemHandleType : uint32_t
{
    AppCaptureScreenshotStream = 0x00,
    DisplayTimeoutDeferral     = 0x01,
    GameSaveContainer          = 0x02,
    GameSaveProvider           = 0x03,
    GameSaveUpdate             = 0x04,
    PackageInstallationMonitor = 0x05,
    PackageMount               = 0x06,
    SpeechSynthesizer          = 0x07,
    SpeechSynthesizerStream    = 0x08,
    StoreContext               = 0x09,
    StoreLicense               = 0x0a,
    StoreProductQuery          = 0x0b,
    TaskQueue                  = 0x0c,
    User                       = 0x0d,
    UserSignOutDeferral        = 0x0e,
    GameUiTextEntry            = 0x0f,
};

enum class XSystemHandleCallbackReason : uint32_t
{
    Created   = 0x00,
    Destroyed = 0x01,
};

typedef void* XSystemHandle;

typedef void (CALLBACK *XSystemHandleCallback)(
    _In_ XSystemHandle handle,
    _In_ XSystemHandleType type,
    _In_ XSystemHandleCallbackReason reason,
    _In_ void* context
    );

STDAPI XSystemHandleTrack(
    _In_ XSystemHandleCallback callback,
    _In_opt_ void* context
    ) noexcept;

STDAPI_(bool) XSystemIsHandleValid(
    _In_ XSystemHandle handle
    ) noexcept;


struct XSystemRuntimeInfo
{
    XVersion runtimeVersion;
    XVersion availableVersion;
};

STDAPI_(XSystemRuntimeInfo) XSystemGetRuntimeInfo() noexcept;


STDAPI_(void) XSystemAllowFullDownloadBandwidth(
    _In_ bool enable
    ) noexcept;


}
