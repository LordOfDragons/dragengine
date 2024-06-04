// Copyright(c) Microsoft Corporation. All rights reserved.

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>

extern "C" 
{

enum class XGameRuntimeFeature : uint32_t
{
    XAccessibility               = 0,
    XAppCapture                  = 1,
    XAsync                       = 2,
    XAsyncProvider               = 3,
    XDisplay                     = 4,
    XGame                        = 5,
    XGameInvite                  = 6,
    XGameSave                    = 7,
    XGameUI                      = 8,
    XLauncher                    = 9,
    XNetworking                  = 10,
    XPackage                     = 11,
    XPersistentLocalStorage      = 12,
    XSpeechSynthesizer           = 13,
    XStore                       = 14,
    XSystem                      = 15,
    XTaskQueue                   = 16,
    XThread                      = 17,
    XUser                        = 18,
    XError                       = 19,
    XGameEvent                   = 20,
    XGameStreaming               = 21,
};

// Must call XGameRuntimeInitialize
STDAPI_(bool) XGameRuntimeIsFeatureAvailable(XGameRuntimeFeature feature) noexcept;

} // extern "C"
