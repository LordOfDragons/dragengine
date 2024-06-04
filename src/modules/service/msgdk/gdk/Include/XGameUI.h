// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XAsync.h>
#include <XUser.h>

extern "C"
{

// Message dialog

enum class XGameUiMessageDialogButton : uint32_t
{
    First = 0,
    Second = 1,
    Third = 2
};

STDAPI XGameUiShowMessageDialogAsync(
    _In_ XAsyncBlock* async,
    _In_z_ const char* titleText,
    _In_z_ const char* contentText,
    _In_opt_z_ const char* firstButtonText,
    _In_opt_z_ const char* secondButtonText,
    _In_opt_z_ const char* thirdButtonText,
    _In_ XGameUiMessageDialogButton defaultButton,
    _In_ XGameUiMessageDialogButton cancelButton
    ) noexcept;

STDAPI XGameUiShowMessageDialogResult(
    _In_ XAsyncBlock* async,
    _Out_ XGameUiMessageDialogButton* resultButton
    ) noexcept;

// Game invite

STDAPI XGameUiShowSendGameInviteAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser,
    _In_z_ const char* sessionConfigurationId,
    _In_z_ const char* sessionTemplateName,
    _In_z_ const char* sessionId,
    _In_opt_z_ const char* invitationText,
    _In_opt_z_ const char* customActivationContext
    ) noexcept;

STDAPI XGameUiShowSendGameInviteResult(
    _In_ XAsyncBlock* async
    ) noexcept;

// MultiplayerActivity Game Invite

STDAPI XGameUiShowMultiplayerActivityGameInviteAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser
    ) noexcept;

STDAPI XGameUiShowMultiplayerActivityGameInviteResult(
    _In_ XAsyncBlock* async
    ) noexcept;

// Player profile card

STDAPI XGameUiShowPlayerProfileCardAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser,
    _In_ uint64_t targetPlayer
    ) noexcept;

STDAPI XGameUiShowPlayerProfileCardResult(
    _In_ XAsyncBlock* async
    ) noexcept;

// Achievements

STDAPI XGameUiShowAchievementsAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser,
    _In_ uint32_t titleId
    ) noexcept;

STDAPI XGameUiShowAchievementsResult(
    _In_ XAsyncBlock * async
    ) noexcept;

// Player Picker

STDAPI XGameUiShowPlayerPickerAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser,
    _In_z_ const char* promptText,
    _In_ uint32_t selectFromPlayersCount,
    _In_reads_(selectFromPlayersCount) const uint64_t* selectFromPlayers,
    _In_ uint32_t preSelectedPlayersCount,
    _In_reads_opt_(preSelectedPlayersCount) const uint64_t* preSelectedPlayers,
    _In_ uint32_t minSelectionCount,
    _In_ uint32_t maxSelectionCount
    ) noexcept;

STDAPI XGameUiShowPlayerPickerResultCount(
    _In_ XAsyncBlock* async,
    _Out_ uint32_t* resultPlayersCount
    ) noexcept;

STDAPI XGameUiShowPlayerPickerResult(
    _In_ XAsyncBlock* async,
    _In_ uint32_t resultPlayersCount,
    _Out_writes_to_(resultPlayersCount, *resultPlayersUsed) uint64_t* resultPlayers,
    _Out_opt_ uint32_t* resultPlayersUsed
    ) noexcept;

// Error Dialog

STDAPI XGameUiShowErrorDialogAsync(
    _In_ XAsyncBlock* async,
    _In_ HRESULT errorCode,
    _In_opt_z_ const char* context
    ) noexcept;

STDAPI XGameUiShowErrorDialogResult(
    _In_ XAsyncBlock* async
    ) noexcept;

// Notifications

enum class XGameUiNotificationPositionHint : uint32_t
{
    BottomCenter = 0,
    BottomLeft = 1,
    BottomRight = 2,
    TopCenter = 3,
    TopLeft = 4,
    TopRight = 5
};

STDAPI XGameUiSetNotificationPositionHint(
    _In_ XGameUiNotificationPositionHint position
    ) noexcept;

// Text Entry

enum class XGameUiTextEntryInputScope : uint32_t
{
    Default = 0,
    Url = 1,
    EmailSmtpAddress = 5,
    Number = 29,
    Password = 31,
    TelephoneNumber = 32,
    Alphanumeric = 40,
    Search = 50,
    ChatWithoutEmoji = 68,
};

STDAPI XGameUiShowTextEntryAsync(
    _In_ XAsyncBlock* async,
    _In_opt_z_ const char* titleText,
    _In_opt_z_ const char* descriptionText,
    _In_opt_z_ const char* defaultText,
    _In_ XGameUiTextEntryInputScope inputScope,
    _In_ uint32_t maxTextLength
    ) noexcept;

STDAPI XGameUiShowTextEntryResultSize(
    _In_ XAsyncBlock* async,
    _Out_ uint32_t* resultTextBufferSize
    ) noexcept;

STDAPI XGameUiShowTextEntryResult(
    _In_ XAsyncBlock* async,
    _In_ uint32_t resultTextBufferSize,
    _Out_writes_to_(resultTextBufferSize, *resultTextBufferUsed) char* resultTextBuffer,
    _Out_opt_ uint32_t* resultTextBufferUsed
    ) noexcept;

}

// Authentication

enum class XGameUiWebAuthenticationOptions : uint32_t
{
    None                     = 0x00,
    PreferFullscreen         = 0x01
};

DEFINE_ENUM_FLAG_OPERATORS(XGameUiWebAuthenticationOptions);

struct XGameUiWebAuthenticationResultData
{
    HRESULT responseStatus;
    size_t responseCompletionUriSize;
    _Field_size_opt_(responseCompletionUriSize) _Null_terminated_ const char* responseCompletionUri;
};

STDAPI XGameUiShowWebAuthenticationAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser,
    _In_z_ const char* requestUri,
    _In_z_ const char* completionUri
    ) noexcept;

STDAPI XGameUiShowWebAuthenticationWithOptionsAsync(
    _In_ XAsyncBlock* async,
    _In_ XUserHandle requestingUser,
    _In_z_ const char* requestUri,
    _In_z_ const char* completionUri,
    _In_ XGameUiWebAuthenticationOptions options
    ) noexcept;

STDAPI XGameUiShowWebAuthenticationResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
    ) noexcept;

STDAPI XGameUiShowWebAuthenticationResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ XGameUiWebAuthenticationResultData** ptrToBuffer,
    _Out_opt_ size_t* bufferUsed
    ) noexcept;

// Non-Modal Text Entry

typedef struct XGameUiTextEntry* XGameUiTextEntryHandle;

enum class XGameUiTextEntryChangeTypeFlags : uint32_t
{
    None                = 0x0,
    TextChanged         = 0x1,
    Dismissed   = 0x2,
};

DEFINE_ENUM_FLAG_OPERATORS(XGameUiTextEntryChangeTypeFlags);

enum class XGameUiTextEntryVisibilityFlags : uint32_t
{
    Default = 0x0,
    OnlyShowCandidates = 0x1,
};

DEFINE_ENUM_FLAG_OPERATORS(XGameUiTextEntryVisibilityFlags);

enum class XGameUiTextEntryPositionHint : uint32_t
{
    Bottom = 0,
    Top = 1,
};

struct XGameUiTextEntryOptions
{
    XGameUiTextEntryInputScope inputScope;
    XGameUiTextEntryPositionHint positionHint;
    XGameUiTextEntryVisibilityFlags visibilityFlags;
};

STDAPI XGameUiTextEntryOpen(
    _In_ const XGameUiTextEntryOptions* options,
    _In_ uint32_t maxLength,
    _In_opt_z_ const char* initialText,
    _In_ uint32_t cursorIndex,
    _Out_ XGameUiTextEntryHandle* handle
) noexcept;

STDAPI_(void) XGameUiTextEntryClose(
    _In_ XGameUiTextEntryHandle handle
) noexcept;

STDAPI XGameUiTextEntryGetState(
    _In_ XGameUiTextEntryHandle handle,
    _Out_ XGameUiTextEntryChangeTypeFlags* changeType,
    _Out_opt_ uint32_t* cursorIndex,
    _Out_opt_ uint32_t* imeClauseStartIndex,
    _Out_opt_ uint32_t* imeClauseEndIndex,
    _In_ uint32_t bufferSize,
    _Out_writes_z_(bufferSize) char* buffer
) noexcept;

struct XGameUiTextEntryExtents
{
    float left;
    float top;
    float right;
    float bottom;
};

STDAPI XGameUiTextEntryGetExtents(
    _In_ XGameUiTextEntryHandle handle,
    _Out_ XGameUiTextEntryExtents* extents
) noexcept;

STDAPI XGameUiTextEntryUpdatePositionHint(
    _In_ XGameUiTextEntryHandle handle,
    _In_ XGameUiTextEntryPositionHint positionHint
) noexcept;

STDAPI XGameUiTextEntryUpdateVisibility(
    _In_ XGameUiTextEntryHandle handle,
    _In_ XGameUiTextEntryVisibilityFlags visibilityFlags
) noexcept;
