// Copyright(c) Microsoft Corporation. All rights reserved.

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

extern "C" 
{

STDAPI XGameRuntimeInitialize();
STDAPI_(void) XGameRuntimeUninitialize();

} // extern "C"
