// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XGameRuntimeTypes.h>
#include <XTaskQueue.h>

extern "C" 
{

enum class XClosedCaptionFontEdgeAttribute : uint32_t
{
    Default = 0,
    NoEdgeAttribute = 1,
    RaisedEdges = 2,
    DepressedEdges = 3,
    UniformedEdges = 4,
    DropShadowedEdges= 5
};

enum class XClosedCaptionFontStyle : uint32_t
{
    Default = 0,
    MonospacedWithSerifs = 1,
    ProportionalWithSerifs = 2,
    MonospacedWithoutSerifs = 3,
    ProportionalWithoutSerifs = 4,
    Casual = 5,
    Cursive = 6,
    SmallCapitals = 7
};

struct XClosedCaptionProperties
{
    XColor BackgroundColor;
    XColor FontColor;
    XColor WindowColor;
    XClosedCaptionFontEdgeAttribute FontEdgeAttribute;
    XClosedCaptionFontStyle FontStyle;
    float FontScale;
    bool Enabled;
};

STDAPI XClosedCaptionGetProperties(
    _Out_ XClosedCaptionProperties* properties
    ) noexcept;

STDAPI XClosedCaptionSetEnabled(
    _In_ bool enabled
    ) noexcept;

enum class XHighContrastMode : uint32_t
{
    Off = 0,
    Dark = 1,
    Light = 2,
    Other = 3,
};

STDAPI XHighContrastGetMode(
    _Out_ XHighContrastMode* mode
    ) noexcept;

// Speech to Text

enum class XSpeechToTextPositionHint : uint32_t
{
    BottomCenter = 0,
    BottomLeft = 1,
    BottomRight = 2,
    MiddleRight = 3,
    MiddleLeft = 4,
    TopCenter = 5,
    TopLeft = 6,
    TopRight = 7
};

enum class XSpeechToTextType : uint32_t
{
    Voice = 0,
    Text = 1
};

STDAPI XSpeechToTextSetPositionHint(
    _In_ XSpeechToTextPositionHint position
    ) noexcept;

STDAPI XSpeechToTextSendString(
    _In_z_ const char* speakerName,
    _In_z_ const char* content,
    _In_ XSpeechToTextType type
    ) noexcept;

STDAPI XSpeechToTextBeginHypothesisString(
    _In_z_ const char* speakerName,
    _In_z_ const char* content,
    _In_ XSpeechToTextType type,
    _Out_ uint32_t* hypothesisId
    ) noexcept;

STDAPI XSpeechToTextUpdateHypothesisString(
    _In_ uint32_t hypothesisId,
    _In_z_ const char* content
    ) noexcept;

STDAPI XSpeechToTextFinalizeHypothesisString(
    _In_ uint32_t hypothesisId,
    _In_z_ const char* content
    ) noexcept;

STDAPI XSpeechToTextCancelHypothesisString(
    _In_ uint32_t hypothesisId
    ) noexcept;

}
