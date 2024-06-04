// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>

extern "C"
{

// Game provided callback that is fired when an error is reported.
typedef bool CALLBACK XErrorCallback(_In_ HRESULT hr, _In_z_ const char* msg, _In_opt_ void* context);

// Allows the game to specify the callback that is fired when an error is reported.
// Passing in nullptr will clear callback.
STDAPI_(void) XErrorSetCallback(_In_opt_ XErrorCallback* callback, _In_opt_ void* context) noexcept;

// Describes how the game wants to receive feedback when XErrorReport is called.
enum class XErrorOptions : uint32_t
{
    None                     = 0x00,
    OutputDebugStringOnError = 0x01,
    DebugBreakOnError        = 0x02,
    FailFastOnError          = 0x04,
};

DEFINE_ENUM_FLAG_OPERATORS(XErrorOptions);

// Allows the game to specify what behavior they want when an error is reported. The
// default is that if a debugger is attached, break. When a debugger is not attached, do nothing.
// The debugger break can be temporarily disabled through the watch window by setting the value of
// the global variable XErrorDisableDebugBreak to true.
STDAPI_(void) XErrorSetOptions(
    _In_ XErrorOptions optionsDebuggerPresent,
    _In_ XErrorOptions optionsDebuggerNotPresent
    ) noexcept;

}
