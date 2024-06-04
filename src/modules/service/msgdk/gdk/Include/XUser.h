// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XAsync.h>
#include <XTaskQueue.h>

extern "C"
{

typedef struct XUser* XUserHandle;

struct XUserLocalId
{
    uint64_t value;
};

const XUserLocalId XUserNullUserLocalId = {};
const APP_LOCAL_DEVICE_ID XUserNullDeviceId = {};

STDAPI XUserDuplicateHandle(
    _In_ XUserHandle handle,
    _Out_ XUserHandle* duplicatedHandle
    ) noexcept;

STDAPI_(void) XUserCloseHandle(
    _In_ XUserHandle user
    ) noexcept;

STDAPI_(int32_t) XUserCompare(
    _In_opt_ XUserHandle user1,
    _In_opt_ XUserHandle user2
    ) noexcept;

// MaxUsers

STDAPI XUserGetMaxUsers(
    _Out_ uint32_t* maxUsers
    ) noexcept;

// AddUser

enum class XUserAddOptions: uint32_t
{
    None                     = 0x00,
    AddDefaultUserSilently   = 0x01,
    AllowGuests              = 0x02,
    AddDefaultUserAllowingUI = 0x04,
};

DEFINE_ENUM_FLAG_OPERATORS(XUserAddOptions);

STDAPI XUserAddAsync(
    _In_ XUserAddOptions options,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserAddResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XUserHandle* newUser
    ) noexcept;

STDAPI XUserAddByIdWithUiAsync (
    _In_ uint64_t userId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserAddByIdWithUiResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XUserHandle* newUser
    ) noexcept;

// UserId

STDAPI XUserGetId(
    _In_ XUserHandle user,
    _Out_ uint64_t* userId
    ) noexcept;

STDAPI XUserFindUserById(
    _In_ uint64_t userId,
    _Out_ XUserHandle* handle
    ) noexcept;

// UserLocalId

STDAPI XUserGetLocalId(
    _In_ XUserHandle user,
    _Out_ XUserLocalId* userLocalId
    ) noexcept;

STDAPI XUserFindUserByLocalId(
    _In_ XUserLocalId userLocalId,
    _Out_ XUserHandle* handle
    ) noexcept;

// APP_LOCAL_DEVICE_ID

STDAPI XUserFindForDevice(
    _In_ const APP_LOCAL_DEVICE_ID* deviceId,
    _Out_ XUserHandle* handle
    ) noexcept;

// UserIsGuest

STDAPI XUserGetIsGuest(
    _In_ XUserHandle user,
    _Out_ bool* isGuest
    ) noexcept;

// UserState

enum class XUserState: uint32_t
{
    SignedIn    = 0,
    SigningOut  = 1,
    SignedOut   = 2,
};

STDAPI XUserGetState(
    _In_ XUserHandle user,
    _Out_ XUserState* state
    ) noexcept;

// UserGamertag

const size_t XUserGamertagComponentClassicMaxBytes = 16;
const size_t XUserGamertagComponentModernMaxBytes = 97;
const size_t XUserGamertagComponentModernSuffixMaxBytes = 15;
const size_t XUserGamertagComponentUniqueModernMaxBytes = 101;

enum class XUserGamertagComponent: uint32_t
{
    Classic         = 0, // Clasic Gamertag
    Modern          = 1, // Modern Gamertag without Suffix
    ModernSuffix    = 2, // Modern Gamertag Suffix if present (otherwise empty)
    UniqueModern    = 3, // Combined Modern Gamertag with Suffix
};

STDAPI XUserGetGamertag(
    _In_ XUserHandle user,
    _In_ XUserGamertagComponent gamertagComponent,
    _In_ size_t gamertagSize,
    _Out_writes_bytes_to_(gamertagSize, *gamertagUsed) char* gamertag,
    _Out_opt_ size_t* gamertagUsed
    ) noexcept;

// UserGamerPicture

enum class XUserGamerPictureSize: uint32_t
{
    Small           = 0, // 64x64
    Medium          = 1, // 208x208
    Large           = 2, // 424x424
    ExtraLarge      = 3, // 1080x1080
};

STDAPI XUserGetGamerPictureAsync(
    _In_ XUserHandle user,
    _In_ XUserGamerPictureSize pictureSize,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserGetGamerPictureResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
    ) noexcept;

STDAPI XUserGetGamerPictureResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Out_opt_ size_t* bufferUsed
    ) noexcept;

// UserAgeGroup

enum class XUserAgeGroup: uint32_t
{
    Unknown = 0,
    Child   = 1,
    Teen    = 2,
    Adult   = 3,
};

STDAPI XUserGetAgeGroup(
    _In_ XUserHandle user,
    _Out_ XUserAgeGroup* ageGroup
    ) noexcept;

// UserPrivilege

enum class XUserPrivilege: uint32_t
{
    CrossPlay              = 185, // The user can play with people outside of Xbox Live
    Clubs                  = 188, // Create/join/participate in Clubs
    Sessions               = 189, // Create/join non interactive multiplayer sessions
    Broadcast              = 190, // Broadcast live gameplay
    ManageProfilePrivacy   = 196, // Change settings to show real name
    GameDvr                = 198, // Upload GameDVR
    MultiplayerParties     = 203, // Join parties
    CloudManageSession     = 207, // Allocate cloud compute resources for their session
    CloudJoinSession       = 208, // Join cloud compute sessions
    CloudSavedGames        = 209, // Save games on the cloud
    SocialNetworkSharing   = 220, // Share progress to social networks
    UserGeneratedContent   = 247, // Access user generated content in game
    Communications         = 252, // Use real time voice and text communication
    Multiplayer            = 254, // Join multiplayer sessions
    AddFriends             = 255, // Add friends / people to follow
};

enum class XUserPrivilegeDenyReason: uint32_t
{
    None                = 0,
    PurchaseRequired    = 1,
    Restricted          = 2,
    Banned              = 3,

    Unknown             = 0xFFFFFFFF
};

enum class XUserPrivilegeOptions: uint32_t
{
    None            = 0x00,
    AllUsers        = 0x01,
};

DEFINE_ENUM_FLAG_OPERATORS(XUserPrivilegeOptions);

STDAPI XUserCheckPrivilege(
    _In_ XUserHandle user,
    _In_ XUserPrivilegeOptions options,
    _In_ XUserPrivilege privilege,
    _Out_ bool* hasPrivilege,
    _Out_opt_ XUserPrivilegeDenyReason* reason
    ) noexcept;

STDAPI XUserResolvePrivilegeWithUiAsync(
    _In_ XUserHandle user,
    _In_ XUserPrivilegeOptions options,
    _In_ XUserPrivilege privilege,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserResolvePrivilegeWithUiResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

// UserTokenAndSignature

enum class XUserGetTokenAndSignatureOptions: uint32_t
{
    None            = 0x00,
    ForceRefresh    = 0x01,
    AllUsers        = 0x02,
};

DEFINE_ENUM_FLAG_OPERATORS(XUserGetTokenAndSignatureOptions);

struct XUserGetTokenAndSignatureData
{
    size_t tokenSize;
    size_t signatureSize;
    _Field_size_bytes_opt_(tokenSize) _Null_terminated_ const char* token;
    _Field_size_bytes_opt_(signatureSize) _Null_terminated_ const char* signature;
};

struct XUserGetTokenAndSignatureHttpHeader
{
    _Field_z_ const char* name;
    _Field_z_ const char* value;
};

STDAPI XUserGetTokenAndSignatureAsync(
    _In_ XUserHandle user,
    _In_ XUserGetTokenAndSignatureOptions options,
    _In_z_ const char* method,
    _In_z_ const char* url,
    _In_ size_t headerCount,
    _In_reads_opt_(headerCount) const XUserGetTokenAndSignatureHttpHeader* headers,
    _In_ size_t bodySize,
    _In_reads_bytes_opt_(bodySize) const void* bodyBuffer,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserGetTokenAndSignatureResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
    ) noexcept;

STDAPI XUserGetTokenAndSignatureResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ XUserGetTokenAndSignatureData** ptrToBuffer,
    _Out_opt_ size_t* bufferUsed
    ) noexcept;

struct XUserGetTokenAndSignatureUtf16HttpHeader
{
    _Field_z_ const wchar_t* name;
    _Field_z_ const wchar_t* value;
};

struct XUserGetTokenAndSignatureUtf16Data
{
    size_t tokenCount;
    size_t signatureCount;
    _Field_size_opt_(tokenCount) _Null_terminated_ const wchar_t* token;
    _Field_size_opt_(signatureCount) _Null_terminated_ const wchar_t* signature;
};

STDAPI XUserGetTokenAndSignatureUtf16Async(
    _In_ XUserHandle user,
    _In_ XUserGetTokenAndSignatureOptions options,
    _In_z_ const wchar_t* method,
    _In_z_ const wchar_t* url,
    _In_ size_t headerCount,
    _In_reads_opt_(headerCount) const XUserGetTokenAndSignatureUtf16HttpHeader* headers,
    _In_ size_t bodySize,
    _In_reads_bytes_opt_(bodySize) const void* bodyBuffer,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserGetTokenAndSignatureUtf16ResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
    ) noexcept;

STDAPI XUserGetTokenAndSignatureUtf16Result(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ XUserGetTokenAndSignatureUtf16Data** ptrToBuffer,
    _Out_opt_ size_t* bufferUsed
    ) noexcept;

// ResolveUserIssue

STDAPI XUserResolveIssueWithUiAsync(
    _In_ XUserHandle user,
    _In_opt_z_ const char* url,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserResolveIssueWithUiResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserResolveIssueWithUiUtf16Async(
    _In_ XUserHandle user,
    _In_opt_z_ const wchar_t* url,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserResolveIssueWithUiUtf16Result(
    _Inout_ XAsyncBlock* async
    ) noexcept;

// UserEvent

enum class XUserChangeEvent: uint32_t
{
    SignedInAgain   = 0,
    SigningOut      = 1,
    SignedOut       = 2,
    Gamertag        = 3,
    GamerPicture    = 4,
    Privileges      = 5,
};

typedef void CALLBACK XUserChangeEventCallback(
    _In_opt_ void* context,
    _In_ XUserLocalId userLocalId,
    _In_ XUserChangeEvent event
    );

STDAPI XUserRegisterForChangeEvent(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XUserChangeEventCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XUserUnregisterForChangeEvent(
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

// UserSignOutDeferral

typedef struct XUserSignOutDeferral* XUserSignOutDeferralHandle;

STDAPI XUserGetSignOutDeferral(
    _Out_ XUserSignOutDeferralHandle* deferral
    ) noexcept;

STDAPI_(void) XUserCloseSignOutDeferralHandle(
    _In_ XUserSignOutDeferralHandle deferral
    ) noexcept;

// UserDeviceAssociation

struct XUserDeviceAssociationChange
{
    APP_LOCAL_DEVICE_ID deviceId;
    XUserLocalId oldUser;
    XUserLocalId newUser;
};

typedef void CALLBACK XUserDeviceAssociationChangedCallback(
    _In_opt_ void* context,
    _In_ const XUserDeviceAssociationChange* change
    );

// Will replay all the current associations upon addition
STDAPI XUserRegisterForDeviceAssociationChanged(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XUserDeviceAssociationChangedCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XUserUnregisterForDeviceAssociationChanged(
    XTaskQueueRegistrationToken token,
    bool wait
    ) noexcept;

STDAPI XUserFindControllerForUserWithUiAsync(
    _In_ XUserHandle user,
    _In_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserFindControllerForUserWithUiResult(
    _Inout_ XAsyncBlock* async,
    _Out_ APP_LOCAL_DEVICE_ID* deviceId
    ) noexcept;

// UserAudioAssociation

enum class XUserDefaultAudioEndpointKind : uint32_t
{
    CommunicationRender = 0,
    CommunicationCapture = 1
};

const size_t XUserAudioEndpointMaxUtf16Count = 56;

STDAPI XUserGetDefaultAudioEndpointUtf16(
    XUserLocalId user,
    XUserDefaultAudioEndpointKind defaultAudioEndpointKind,
    size_t endpointIdUtf16Count,
    _Out_writes_to_(endpointIdUtf16Count, *endpointIdUtf16Used) wchar_t* endpointIdUtf16,
    _Out_opt_ size_t* endpointIdUtf16Used
    ) noexcept;

typedef void CALLBACK XUserDefaultAudioEndpointUtf16ChangedCallback(
    _In_opt_ void* context,
    XUserLocalId user,
    XUserDefaultAudioEndpointKind defaultAudioEndpointKind,
    _In_opt_z_ const wchar_t* endpointIdUtf16
    );

STDAPI XUserRegisterForDefaultAudioEndpointUtf16Changed(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XUserDefaultAudioEndpointUtf16ChangedCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XUserUnregisterForDefaultAudioEndpointUtf16Changed(
    XTaskQueueRegistrationToken token,
    bool wait
    ) noexcept;

// MsaToken

enum class XUserGetMsaTokenSilentlyOptions : uint32_t
{
    None  = 0x00,
};

STDAPI XUserGetMsaTokenSilentlyAsync(
    _In_ XUserHandle user,
    _In_ XUserGetMsaTokenSilentlyOptions options,
    _In_z_ const char* scope,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XUserGetMsaTokenSilentlyResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t resultTokenSize,
    _Out_writes_to_(resultTokenSize, *resultTokenUsed) char* resultToken,
    _Out_opt_ size_t* resultTokenUsed
    ) noexcept;

STDAPI XUserGetMsaTokenSilentlyResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* tokenSize
    ) noexcept;

STDAPI_(bool) XUserIsStoreUser(
    _In_ XUserHandle user
    ) noexcept;

}
