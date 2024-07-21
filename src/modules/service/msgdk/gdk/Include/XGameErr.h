/*++

Copyright (c) Microsoft Corporation. All Rights Reserved.

Module Name:

    XGameErr.mc

Abstract:

    Constant definitions for GXDK error code values.

--*/

#pragma once


/*++

 MessageId's 0x0000 - 0x00ff (inclusive) are reserved for DStorage.

--*/
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_GAME                    2340
#define FACILITY_AUTHMAN                 2013
#define FACILITY_XBOXLIVE                21


//
// Define the severity codes
//


//
// MessageId: E_DSTORAGE_BEGIN
//
// MessageText:
//
// Start of DStorage error codes.
//
#define E_DSTORAGE_BEGIN                 ((HRESULT)0x89240000L)

//
// MessageId: E_DSTORAGE_END
//
// MessageText:
//
// End of DStorage error codes.
//
#define E_DSTORAGE_END                   ((HRESULT)0x892400FFL)

/*++

 MessageId's 0x0100 - 0x01ff (inclusive) are reserved for GAMERUNTIME.

--*/
// 0x89240100
//
// MessageId: E_GAMERUNTIME_NOT_INITIALIZED
//
// MessageText:
//
// The game runtime has not yet been initialized.
//
#define E_GAMERUNTIME_NOT_INITIALIZED    ((HRESULT)0x89240100L)

// 0x89240101
//
// MessageId: E_GAMERUNTIME_DLL_NOT_FOUND
//
// MessageText:
//
// The game runtime dll was not found.
//
#define E_GAMERUNTIME_DLL_NOT_FOUND      ((HRESULT)0x89240101L)

// 0x89240102
//
// MessageId: E_GAMERUNTIME_VERSION_MISMATCH
//
// MessageText:
//
// The game runtime dll does not support this version of XDK.
//
#define E_GAMERUNTIME_VERSION_MISMATCH   ((HRESULT)0x89240102L)

// 0x89240103
//
// MessageId: E_GAMERUNTIME_WINDOW_NOT_FOREGROUND
//
// MessageText:
//
// The operation requires an application window to be in the foreground.
//
#define E_GAMERUNTIME_WINDOW_NOT_FOREGROUND ((HRESULT)0x89240103L)

// 0x89240104
//
// MessageId: E_GAMERUNTIME_SUSPENDED
//
// MessageText:
//
// The game runtime is in a suspended state.
//
#define E_GAMERUNTIME_SUSPENDED          ((HRESULT)0x89240104L)

// 0x89240105
//
// MessageId: E_GAMERUNTIME_UNINITIALIZE_ACTIVEOBJECTS
//
// MessageText:
//
// The game runtime has active objects at XGameRuntimeUninitialize.
//
#define E_GAMERUNTIME_UNINITIALIZE_ACTIVEOBJECTS ((HRESULT)0x89240105L)

// 0x89240106
//
// MessageId: E_GAMERUNTIME_MULTIPLAYER_NOT_CONFIGURED
//
// MessageText:
//
// There is no multiplayer activity set on the service for this user on this title.
//
#define E_GAMERUNTIME_MULTIPLAYER_NOT_CONFIGURED ((HRESULT)0x89240106L)

// 0x89240107
//
// MessageId: E_GAMERUNTIME_MISSING_DEPENDENCY
//
// MessageText:
//
// The game runtime is missing a dependency required to run.
//
#define E_GAMERUNTIME_MISSING_DEPENDENCY ((HRESULT)0x89240107L)

// 0x89240108
//
// MessageId: E_GAMERUNTIME_SUSPEND_ACTIVEOBJECTS
//
// MessageText:
//
// The game runtime has active objects at suspend.
//
#define E_GAMERUNTIME_SUSPEND_ACTIVEOBJECTS ((HRESULT)0x89240108L)

// 0x89240109
//
// MessageId: E_GAMERUNTIME_OPTIONS_MISMATCH
//
// MessageText:
//
// The runtime has already been initialized with a different set of options.
//
#define E_GAMERUNTIME_OPTIONS_MISMATCH   ((HRESULT)0x89240109L)

// 0x8924010A
//
// MessageId: E_GAMERUNTIME_OPTIONS_NOT_SUPPORTED
//
// MessageText:
//
// Custom initialization options cannot be used with packaged builds.
//
#define E_GAMERUNTIME_OPTIONS_NOT_SUPPORTED ((HRESULT)0x8924010AL)

// 0x8924010B
//
// MessageId: E_GAMERUNTIME_GAMECONFIG_BAD_FORMAT
//
// MessageText:
//
// There was as problem parsing the game config.
//
#define E_GAMERUNTIME_GAMECONFIG_BAD_FORMAT ((HRESULT)0x8924010BL)

// 0x8924010C
//
// MessageId: E_GAMERUNTIME_INVALID_HANDLE
//
// MessageText:
//
// This handle value is no longer valid.
//
#define E_GAMERUNTIME_INVALID_HANDLE     ((HRESULT)0x8924010CL)

/*++

 MessageId's 0x5100 - 0x51ff (inclusive) are reserved for GAMEUSER.

--*/
// 0x87DD0013
//
// MessageId: E_GAMEUSER_NO_AUTH_USER
//
// MessageText:
//
// The Authentication service can't identify the user.
//
#define E_GAMEUSER_NO_AUTH_USER          ((HRESULT)0x87DD0013L)

// 0x8015DC12
//
// MessageId: E_GAMEUSER_USER_NOT_IN_SANDBOX
//
// MessageText:
//
// Either the title is not allowed in the current sandbox, or a user authorized for the current sandbox has not been signed in.
//
#define E_GAMEUSER_USER_NOT_IN_SANDBOX   ((HRESULT)0x8015DC12L)

// 0x89245100
//
// MessageId: E_GAMEUSER_MAX_USERS_ADDED
//
// MessageText:
//
// Can't add this user because the max number of users has been added.
//
#define E_GAMEUSER_MAX_USERS_ADDED       ((HRESULT)0x89245100L)

// 0x89245101
//
// MessageId: E_GAMEUSER_SIGNED_OUT
//
// MessageText:
//
// Can't perform the operation because the user is signed out.
//
#define E_GAMEUSER_SIGNED_OUT            ((HRESULT)0x89245101L)

// 0x89245102
//
// MessageId: E_GAMEUSER_RESOLVE_USER_ISSUE_REQUIRED
//
// MessageText:
//
// Needs UI to resolve an issue with this user.
//
#define E_GAMEUSER_RESOLVE_USER_ISSUE_REQUIRED ((HRESULT)0x89245102L)

// 0x89245103
//
// MessageId: E_GAMEUSER_DEFERRAL_NOT_AVAILABLE
//
// MessageText:
//
// Not an appropriate time to request deferral.
//
#define E_GAMEUSER_DEFERRAL_NOT_AVAILABLE ((HRESULT)0x89245103L)

// 0x89245104
//
// MessageId: E_GAMEUSER_USER_NOT_FOUND
//
// MessageText:
//
// User matching the id was not found
//
#define E_GAMEUSER_USER_NOT_FOUND        ((HRESULT)0x89245104L)

// 0x89245105
//
// MessageId: E_GAMEUSER_NO_TOKEN_REQUIRED
//
// MessageText:
//
// No token is required for this call
//
#define E_GAMEUSER_NO_TOKEN_REQUIRED     ((HRESULT)0x89245105L)

// 0x89245106
//
// MessageId: E_GAMEUSER_NO_DEFAULT_USER
//
// MessageText:
//
// There is no current default user
//
#define E_GAMEUSER_NO_DEFAULT_USER       ((HRESULT)0x89245106L)

// 0x89245107
//
// MessageId: E_GAMEUSER_FAILED_TO_RESOLVE
//
// MessageText:
//
// Failed to resolve the given privilege
//
#define E_GAMEUSER_FAILED_TO_RESOLVE     ((HRESULT)0x89245107L)

// 0x89245108
//
// MessageId: E_GAMEUSER_NO_TITLE_ID
//
// MessageText:
//
// An xbox live title id must be configured
//
#define E_GAMEUSER_NO_TITLE_ID           ((HRESULT)0x89245108L)

// 0x89245109
//
// MessageId: E_GAMEUSER_UNKNOWN_GAME_IDENTITY
//
// MessageText:
//
// The game identity is not recognized
//
#define E_GAMEUSER_UNKNOWN_GAME_IDENTITY ((HRESULT)0x89245109L)

// 0x89245110
//
// MessageId: E_GAMEUSER_NO_PACKAGE_IDENTITY
//
// MessageText:
//
// A package identity must be configured
//
#define E_GAMEUSER_NO_PACKAGE_IDENTITY   ((HRESULT)0x89245110L)

// 0x89245111
//
// MessageId: E_GAMEUSER_FAILED_TO_GET_TOKEN
//
// MessageText:
//
// The token request failed
//
#define E_GAMEUSER_FAILED_TO_GET_TOKEN   ((HRESULT)0x89245111L)

// 0x89245112
//
// MessageId: E_GAMEUSER_INVALID_APP_CONFIGURATION
//
// MessageText:
//
// The MSAAppId in the game config is invalid or is not configured for the authentication stack
//
#define E_GAMEUSER_INVALID_APP_CONFIGURATION ((HRESULT)0x89245112L)

// 0x89245113
//
// MessageId: E_GAMEUSER_MALFORMED_MSAAPPID
//
// MessageText:
//
// The MSAAppId in the game config doesn't match an expected format, either a 16 character string or GUID
//
#define E_GAMEUSER_MALFORMED_MSAAPPID    ((HRESULT)0x89245113L)

// 0x89245114
//
// MessageId: E_GAMEUSER_INCONSISTENT_MSAAPPID_AND_TITLEID
//
// MessageText:
//
// The MSAAppId and TitleId in the game config don't match the ones associated to the game
//
#define E_GAMEUSER_INCONSISTENT_MSAAPPID_AND_TITLEID ((HRESULT)0x89245114L)

// 0x89245115
//
// MessageId: E_GAMEUSER_NO_MSAAPPID
//
// MessageText:
//
// The 'MSAAppId' in the MicrosoftGame.config is required for authentication.
// Please use the MicrosoftGame.config Editor "Associate with Store" feature to automatically retrieve Id values from Partner Center.
//
#define E_GAMEUSER_NO_MSAAPPID           ((HRESULT)0x89245115L)

/*++

 MessageId's 0x5200 - 0x52ff (inclusive) are reserved for GAMEPACKAGE.

--*/
// 0x89245200
//
// MessageId: E_GAMEPACKAGE_APP_NOT_PACKAGED
//
// MessageText:
//
// The game is not packaged in a container
//
#define E_GAMEPACKAGE_APP_NOT_PACKAGED   ((HRESULT)0x89245200L)

// 0x89245201
//
// MessageId: E_GAMEPACKAGE_NO_INSTALLED_LANGUAGES
//
// MessageText:
//
// The game uses Intelligent Delivery to selectively install languages, but none of the languages are installed.
//
#define E_GAMEPACKAGE_NO_INSTALLED_LANGUAGES ((HRESULT)0x89245201L)

// 0x89245202
//
// MessageId: E_GAMEPACKAGE_NO_STORE_ID
//
// MessageText:
//
// This API requires that the StoreId attribute be set in the application's game config.
//
#define E_GAMEPACKAGE_NO_STORE_ID        ((HRESULT)0x89245202L)

// 0x89245203
//
// MessageId: E_GAMEPACKAGE_INVALID_SELECTOR
//
// MessageText:
//
// A chunk selector was provided that does not resolve to a chunk in the package.
//
#define E_GAMEPACKAGE_INVALID_SELECTOR   ((HRESULT)0x89245203L)

// 0x89245204
//
// MessageId: E_GAMEPACKAGE_DOWNLOAD_REQUIRED
//
// MessageText:
//
// One or more required chunks must be downloaded to mount the package on this device.
//
#define E_GAMEPACKAGE_DOWNLOAD_REQUIRED  ((HRESULT)0x89245204L)

// 0x89245205
//
// MessageId: E_GAMEPACKAGE_NO_TAG_CHANGE
//
// MessageText:
//
// Change installed chunks in this package using Features, not tags.
//
#define E_GAMEPACKAGE_NO_TAG_CHANGE      ((HRESULT)0x89245205L)

// 0x89245206
//
// MessageId: E_GAMEPACKAGE_DLC_NOT_SUPPORTED
//
// MessageText:
//
// The DLC being mounted is not supported by this game.  Ensure the correct DLC compatibility is set in the game config.
//
#define E_GAMEPACKAGE_DLC_NOT_SUPPORTED  ((HRESULT)0x89245206L)

// 0x89245207
//
// MessageId: E_GAMEPACKAGE_DUPLICATE_ID_VALUES
//
// MessageText:
//
// The package contains more than one executable mapping to the same 'Id' attribute. Please check the game config for duplicate 'Id' attributes in the 'Executable' elements.
//
#define E_GAMEPACKAGE_DUPLICATE_ID_VALUES ((HRESULT)0x89245207L)

// 0x89245208
//
// MessageId: E_GAMEPACKAGE_NO_PACKAGE_IDENTIFIER
//
// MessageText:
//
// Could not resolve an identifier for the package.  The package may not be installed.
//
#define E_GAMEPACKAGE_NO_PACKAGE_IDENTIFIER ((HRESULT)0x89245208L)

// 0x89245209
//
// MessageId: E_GAMEPACKAGE_CONFIG_NO_ROOT_NODE
//
// MessageText:
//
// Could not find a root <Game> node in the MicrosoftGame.config file. Please check the MicrosoftGame.config file to ensure it is properly formatted.
//
#define E_GAMEPACKAGE_CONFIG_NO_ROOT_NODE ((HRESULT)0x89245209L)

// 0x8924520A
//
// MessageId: E_GAMEPACKAGE_CONFIG_ZERO_VERSION
//
// MessageText:
//
// The 'Version' attribute in the 'Identity' node in the MicrosoftGame.config cannot be 0.0.0.0.
// Please increment or remove this attribute. If no version is specified, a default value of 1.0.0.0 will be used.
//
#define E_GAMEPACKAGE_CONFIG_ZERO_VERSION ((HRESULT)0x8924520AL)

// 0x8924520B
//
// MessageId: E_GAMEPACKAGE_CONFIG_NO_MSAAPPID_OR_TITLEID
//
// MessageText:
//
// The 'MSAAppId' or 'TitleId' element in the MicrosoftGame.config is missing. If one of these values is specified, then they must both be specified when using configVersion >= 1.
// Please use the MicrosoftGame.config Editor "Associate with Store" feature to automatically retrieve Id values from Partner Center.
//
#define E_GAMEPACKAGE_CONFIG_NO_MSAAPPID_OR_TITLEID ((HRESULT)0x8924520BL)

// 0x8924520C
//
// MessageId: E_GAMEPACKAGE_CONFIG_DEPRECATED_PC_ENTRIES
//
// MessageText:
//
// The DesktopRegistration entries "modFolder", "enableWritesToPackageRoot", "disableRegistryWriteVirtualization" and "disableFilesystemWriteVirtualization" are deprecated in configVersion >= 1.
//
#define E_GAMEPACKAGE_CONFIG_DEPRECATED_PC_ENTRIES ((HRESULT)0x8924520CL)

// 0x8924520D
//
// MessageId: E_GAMEPACKAGE_CONFIG_SUM_REQUIRES_MSAAPPID
//
// MessageText:
//
// The 'MSAAppId' and 'TitleId' elements in the MicrosoftGame.config are required when using the Simplified User Model (AdvancedUserModel element) and configVersion >= 1.
// Please use the MicrosoftGame.config Editor "Associate with Store" feature to automatically retrieve Id values from Partner Center.
//
#define E_GAMEPACKAGE_CONFIG_SUM_REQUIRES_MSAAPPID ((HRESULT)0x8924520DL)

// 0x8924520E
//
// MessageId: E_GAMEPACKAGE_CONFIG_NO_CODE_CLOUD_SAVES_REQUIRES_MSAAPPID
//
// MessageText:
//
// The 'MSAAppId' and 'TitleId' elements in the MicrosoftGame.config are required when using No Code Cloud Saves (NoCodePCRoot element) and configVersion >= 1.
// Please use the MicrosoftGame.config Editor "Associate with Store" feature to automatically retrieve Id values from Partner Center.
//
#define E_GAMEPACKAGE_CONFIG_NO_CODE_CLOUD_SAVES_REQUIRES_MSAAPPID ((HRESULT)0x8924520EL)

// 0x8924520F
//
// MessageId: E_GAMEPACKAGE_CONFIG_MSAAPPID_OR_TITLEID_IS_DEFAULT
//
// MessageText:
//
// The 'MSAAppId' or 'TitleId' element in the MicrosoftGame.config is a default value. If NoCodePCRoot or AdvancedUserModel is specified, then the TitleId and MSAAppId must both be valid Ids from Partner Center when using configVersion >= 1.
// Please use the MicrosoftGame.config Editor "Associate with Store" feature to automatically retrieve Id values from Partner Center.
//
#define E_GAMEPACKAGE_CONFIG_MSAAPPID_OR_TITLEID_IS_DEFAULT ((HRESULT)0x8924520FL)

// 0x89245210
//
// MessageId: E_GAMEPACKAGE_CONFIG_INVALID_CONTROL_CHARACTERS
//
// MessageText:
//
// One or more attributes of the 'ShellVisuals' element in the MicrosoftGame.config file contains control characters such as newlines or tabs.
// Please ensure all attributes in the 'ShellVisuals' element are free of control characters.
//
#define E_GAMEPACKAGE_CONFIG_INVALID_CONTROL_CHARACTERS ((HRESULT)0x89245210L)

// 0x89245211
//
// MessageId: E_GAMEPACKAGE_CONFIG_PROTOCOL_REQUIRES_EXECUTABLE
//
// MessageText:
//
// There is a 'Protocol' element in the MicrosoftGame.config without an executable name.  This is only permitted if there is a single executable in the file.
// Please ensure all 'Protocol' elements have a corresponding executable name.
//
#define E_GAMEPACKAGE_CONFIG_PROTOCOL_REQUIRES_EXECUTABLE ((HRESULT)0x89245211L)

/*++

 MessageId's 0x5300 - 0x53ff (inclusive) are reserved for GAMESTORE.

--*/
// 0x89245300
//
// MessageId: E_GAMESTORE_LICENSE_ACTION_NOT_APPLICABLE_TO_PRODUCT
//
// MessageText:
//
// The game requested a license for a product that cannot be licensed
//
#define E_GAMESTORE_LICENSE_ACTION_NOT_APPLICABLE_TO_PRODUCT ((HRESULT)0x89245300L)

// 0x89245301
//
// MessageId: E_GAMESTORE_NETWORK_ERROR
//
// MessageText:
//
// The game failed to communicate with the store network
//
#define E_GAMESTORE_NETWORK_ERROR        ((HRESULT)0x89245301L)

// 0x89245302
//
// MessageId: E_GAMESTORE_SERVER_ERROR
//
// MessageText:
//
// The game received a bad response from the store server
//
#define E_GAMESTORE_SERVER_ERROR         ((HRESULT)0x89245302L)

// 0x89245303
//
// MessageId: E_GAMESTORE_INSUFFICIENT_QUANTITY
//
// MessageText:
//
// The user does not have enough of this consumable to use the requested amount
//
#define E_GAMESTORE_INSUFFICIENT_QUANTITY ((HRESULT)0x89245303L)

// 0x89245304
//
// MessageId: E_GAMESTORE_ALREADY_PURCHASED
//
// MessageText:
//
// The user already owns this product
//
#define E_GAMESTORE_ALREADY_PURCHASED    ((HRESULT)0x89245304L)

// 0x89245305
//
// MessageId: E_GAMESTORE_LICENSE_ACTION_THROTTLED
//
// MessageText:
//
// The game has excessively requested a license for a product and is now being throttled.
//
#define E_GAMESTORE_LICENSE_ACTION_THROTTLED ((HRESULT)0x89245305L)

/*++

 MessageIds 0x5400 - 0x54ff (inclusive) are reserved for XGameStreaming.

--*/
// 0x89245400
//
// MessageId: E_GAMESTREAMING_NOT_INITIALIZED
//
// MessageText:
//
// The XGameStreaming runtime has not been initialized. Call XGameStreamingInitialize before calling other APIs.
//
#define E_GAMESTREAMING_NOT_INITIALIZED  ((HRESULT)0x89245400L)

// 0x89245401
//
// MessageId: E_GAMESTREAMING_CLIENT_NOT_CONNECTED
//
// MessageText:
//
// The specified client is not connected.
//
#define E_GAMESTREAMING_CLIENT_NOT_CONNECTED ((HRESULT)0x89245401L)

// 0x89245402
//
// MessageId: E_GAMESTREAMING_NO_DATA
//
// MessageText:
//
// The requested data is not available. The data may be available later.
//
#define E_GAMESTREAMING_NO_DATA          ((HRESULT)0x89245402L)

// 0x89245403
//
// MessageId: E_GAMESTREAMING_NO_DATACENTER
//
// MessageText:
//
// The current machine is not running in a datacenter.
//
#define E_GAMESTREAMING_NO_DATACENTER    ((HRESULT)0x89245403L)

// 0x89245404
//
// MessageId: E_GAMESTREAMING_NOT_STREAMING_CONTROLLER
//
// MessageText:
//
// The current GamepadReading did not come from a streaming controller.
//
#define E_GAMESTREAMING_NOT_STREAMING_CONTROLLER ((HRESULT)0x89245404L)

// 0x89245405
//
// MessageId: E_GAMESTREAMING_NO_MATCH
//
// MessageText:
//
// Unable to find a match with the given scenario.
//
#define E_GAMESTREAMING_NO_MATCH         ((HRESULT)0x89245405L)

// 0x89245406
//
// MessageId: E_GAMESTREAMING_TOO_MANY_CALLS
//
// MessageText:
//
// The API was called too frequently.
//
#define E_GAMESTREAMING_TOO_MANY_CALLS   ((HRESULT)0x89245406L)

// 0x89245407
//
// MessageId: E_GAMESTREAMING_CUSTOM_RESOLUTION_NOT_SUPPORTED
//
// MessageText:
//
// This console does not support custom resolution.
//
#define E_GAMESTREAMING_CUSTOM_RESOLUTION_NOT_SUPPORTED ((HRESULT)0x89245407L)

// 0x89245408
//
// MessageId: E_GAMESTREAMING_CUSTOM_RESOLUTION_TOO_SMALL
//
// MessageText:
//
// The resolution is below the minimum width or height.
//
#define E_GAMESTREAMING_CUSTOM_RESOLUTION_TOO_SMALL ((HRESULT)0x89245408L)

// 0x89245409
//
// MessageId: E_GAMESTREAMING_CUSTOM_RESOLUTION_TOO_LARGE
//
// MessageText:
//
// The resolution is above the maximum width or height.
//
#define E_GAMESTREAMING_CUSTOM_RESOLUTION_TOO_LARGE ((HRESULT)0x89245409L)

// 0x8924540A
//
// MessageId: E_GAMESTREAMING_CUSTOM_RESOLUTION_TOO_MANY_PIXELS
//
// MessageText:
//
// The resolution is above the maximum number of pixels.
//
#define E_GAMESTREAMING_CUSTOM_RESOLUTION_TOO_MANY_PIXELS ((HRESULT)0x8924540AL)

// 0x8924540B
//
// MessageId: E_GAMESTREAMING_INVALID_CUSTOM_RESOLUTION
//
// MessageText:
//
// The resolution is invalid.
//
#define E_GAMESTREAMING_INVALID_CUSTOM_RESOLUTION ((HRESULT)0x8924540BL)


