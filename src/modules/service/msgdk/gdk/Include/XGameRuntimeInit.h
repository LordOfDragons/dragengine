// Copyright(c) Microsoft Corporation. All rights reserved.

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>

extern "C" 
{

enum class XGameRuntimeGameConfigSource : uint32_t
{
    Default,
    Inline,
    File
};

struct XGameRuntimeOptions
{
    XGameRuntimeGameConfigSource gameConfigSource;
    const char* gameConfig;
};

STDAPI XGameRuntimeInitialize();
STDAPI XGameRuntimeInitializeWithOptions(_In_ const XGameRuntimeOptions* options);

STDAPI_(void) XGameRuntimeUninitialize();

} // extern "C"
