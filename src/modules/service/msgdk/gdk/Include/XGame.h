// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XUser.h>

extern "C"
{

// Returns HRESULT_FROM_WIN32(ERROR_NOT_FOUND) if not present
STDAPI XGameGetXboxTitleId(_Out_ uint32_t* titleId) noexcept;

// Launches a new game, replacing the current game and freeing any allocated resources.
// This function will always terminate the current game; it will never return.
STDAPI_(void) XLaunchNewGame(
    _In_z_ const char* exePath,
    _In_opt_z_ const char* args,
    _In_opt_ XUserHandle defaultUser
    ) noexcept;

}

// Requests for the current game process is restarted when the current instance terminates
// do to a crash. Only enabled during development.
STDAPI XLaunchRestartOnCrash(
    _In_opt_z_ const char* args,
    _In_ uint32_t reserved
    ) noexcept;
