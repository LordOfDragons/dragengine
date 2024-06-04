// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <XTaskQueue.h>

extern "C" 
{

typedef struct XSpeechSynthesizer* XSpeechSynthesizerHandle;
typedef struct XSpeechSynthesizerStream* XSpeechSynthesizerStreamHandle;

enum class XSpeechSynthesizerVoiceGender : uint32_t
{
    Female = 0,
    Male = 1,
};

struct XSpeechSynthesizerVoiceInformation
{
    _Field_z_ const char* Description;
    _Field_z_ const char* DisplayName;
    XSpeechSynthesizerVoiceGender Gender;
    _Field_z_ const char* VoiceId;
    _Field_z_ const char* Language;
};

typedef bool CALLBACK XSpeechSynthesizerInstalledVoicesCallback(
    _In_ const XSpeechSynthesizerVoiceInformation* information,
    _In_ void* context
    );

STDAPI XSpeechSynthesizerEnumerateInstalledVoices(
    _In_opt_ void* context,
    _In_ XSpeechSynthesizerInstalledVoicesCallback* callback
    ) noexcept;

STDAPI XSpeechSynthesizerCreate(
    _Out_ XSpeechSynthesizerHandle* speechSynthesizer
    ) noexcept;

STDAPI XSpeechSynthesizerCloseHandle(
    _In_opt_ XSpeechSynthesizerHandle speechSynthesizer
    ) noexcept;

STDAPI XSpeechSynthesizerSetDefaultVoice(
    _In_ XSpeechSynthesizerHandle speechSynthesizer
    ) noexcept;

STDAPI XSpeechSynthesizerSetCustomVoice(
    _In_ XSpeechSynthesizerHandle speechSynthesizer,
    _In_z_ const char* voiceId
    ) noexcept;

STDAPI XSpeechSynthesizerCreateStreamFromText(
    _In_ XSpeechSynthesizerHandle speechSynthesizer,
    _In_z_ const char* text,
    _Out_ XSpeechSynthesizerStreamHandle* speechSynthesisStream
    ) noexcept;

STDAPI XSpeechSynthesizerCreateStreamFromSsml(
    _In_ XSpeechSynthesizerHandle speechSynthesizer,
    _In_z_ const char* ssml,
    _Out_ XSpeechSynthesizerStreamHandle* speechSynthesisStream
    ) noexcept;

STDAPI XSpeechSynthesizerCloseStreamHandle(
    _In_ XSpeechSynthesizerStreamHandle speechSynthesisStream
    ) noexcept;

STDAPI XSpeechSynthesizerGetStreamDataSize(
    _In_ XSpeechSynthesizerStreamHandle speechSynthesisStream,
    _Out_ size_t* bufferSize
    ) noexcept;

STDAPI XSpeechSynthesizerGetStreamData(
    _In_ XSpeechSynthesizerStreamHandle speechSynthesisStream,
    _In_ size_t bufferSize,
    _Out_writes_to_(bufferSize, *bufferUsed) void* buffer,
    _Out_opt_ size_t* bufferUsed
    ) noexcept;

}
