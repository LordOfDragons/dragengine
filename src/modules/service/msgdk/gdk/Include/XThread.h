// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

extern "C" 
{

/// <summary>
/// Call this to setup a thread as "time sensitive".  APIs that should not be called from
/// time sensitive threads call XThreadAssertNotTimeSensitive, which will assert in debug
/// if the calling therad is marked as time sensitive.
/// </summary>
/// <param name='isTimeSensitiveThread'>True if the current thread should be marked time sensitive.</param>
STDAPI XThreadSetTimeSensitive(
    _In_ bool isTimeSensitiveThread
    ) noexcept;

/// <summary>
/// Returns true if the calling thread is marked as time sensitive.
/// </summary>
STDAPI_(bool) XThreadIsTimeSensitive() noexcept;

/// <summary>
/// Asserts if the calling thread is marked as time sensitive.
/// </summary>
STDAPI_(void) XThreadAssertNotTimeSensitive() noexcept;

}