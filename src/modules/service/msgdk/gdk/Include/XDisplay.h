// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>

extern "C"
{

// Hdr mode

enum class XDisplayHdrModeResult : uint32_t
{
    Unknown = 0,
    Enabled = 1,
    Disabled = 2
};

enum class XDisplayHdrModePreference : uint32_t
{
    PreferHdr = 0,
    PreferRefreshRate = 1
};

struct XDisplayHdrModeInfo
{
    float minToneMapLuminance;
    float maxToneMapLuminance;
    float maxFullFrameToneMapLuminance;
};

STDAPI_(XDisplayHdrModeResult) XDisplayTryEnableHdrMode(
    _In_ XDisplayHdrModePreference displayModePreference,
    _Out_opt_ XDisplayHdrModeInfo* displayHdrModeInfo
    ) noexcept;

// Display timeout

typedef struct XDisplayTimeoutDeferral* XDisplayTimeoutDeferralHandle;

STDAPI XDisplayAcquireTimeoutDeferral(
    _Out_ XDisplayTimeoutDeferralHandle* handle
    ) noexcept;

STDAPI_(void) XDisplayCloseTimeoutDeferralHandle(
    _In_ XDisplayTimeoutDeferralHandle handle
    ) noexcept;

}
