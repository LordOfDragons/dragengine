// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <XTaskQueue.h>

extern "C" 
{

typedef void CALLBACK XGameProtocolActivationCallback(
    _In_opt_ void* context,
    _In_ const char* protocolUri
    );

STDAPI XGameProtocolRegisterForActivation(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XGameProtocolActivationCallback * callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XGameProtocolUnregisterForActivation(
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

}
