// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <XTaskQueue.h>
#include <XUser.h>

extern "C"
{

// Launcher

STDAPI XLaunchUri(
    _In_opt_ XUserHandle requestingUser,
    _In_z_ const char* uri
    ) noexcept;

}
