// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <XAsync.h>
#include <XTaskQueue.h>

extern "C"
{

STDAPI XNetworkingQueryPreferredLocalUdpMultiplayerPort(
    _Out_ uint16_t* preferredLocalUdpMultiplayerPort
    ) noexcept;

STDAPI XNetworkingQueryPreferredLocalUdpMultiplayerPortAsync(
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

STDAPI XNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _Out_ uint16_t* preferredLocalUdpMultiplayerPort
    ) noexcept;

typedef void XNetworkingPreferredLocalUdpMultiplayerPortChangedCallback(_In_opt_ void* context, _In_ uint16_t preferredLocalUdpMultiplayerPort);

STDAPI XNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XNetworkingPreferredLocalUdpMultiplayerPortChangedCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged(
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

enum class XNetworkingThumbprintType : uint32_t
{
    Leaf = 0,
    Issuer = 1,
    Root = 2,
};

struct XNetworkingThumbprint
{
    XNetworkingThumbprintType thumbprintType;
    size_t thumbprintBufferByteCount;
    _Field_size_(thumbprintBufferByteCount) uint8_t* thumbprintBuffer;
};

struct XNetworkingSecurityInformation
{
    uint32_t enabledHttpSecurityProtocolFlags;
    size_t thumbprintCount;
   _Field_size_(thumbprintCount) XNetworkingThumbprint* thumbprints;
};

STDAPI XNetworkingQuerySecurityInformationForUrlAsync(
    _In_z_ const char* url,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

STDAPI XNetworkingQuerySecurityInformationForUrlAsyncResultSize(
    _Inout_ XAsyncBlock* asyncBlock,
    _Out_ size_t* securityInformationBufferByteCount
    ) noexcept;

STDAPI XNetworkingQuerySecurityInformationForUrlAsyncResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _In_ size_t securityInformationBufferByteCount,
    _Out_opt_ size_t* securityInformationBufferByteCountUsed,
    _Out_writes_bytes_to_(securityInformationBufferByteCount, *securityInformationBufferByteCountUsed) uint8_t* securityInformationBuffer,
    _Outptr_ XNetworkingSecurityInformation** securityInformation
    ) noexcept;

STDAPI XNetworkingQuerySecurityInformationForUrlUtf16Async(
    _In_z_ const wchar_t* url,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

STDAPI XNetworkingQuerySecurityInformationForUrlUtf16AsyncResultSize(
    _Inout_ XAsyncBlock* asyncBlock,
    _Out_ size_t* securityInformationBufferByteCount
    ) noexcept;

STDAPI XNetworkingQuerySecurityInformationForUrlUtf16AsyncResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _In_ size_t securityInformationBufferByteCount,
    _Out_opt_ size_t* securityInformationBufferByteCountUsed,
    _Out_writes_bytes_to_(securityInformationBufferByteCount, *securityInformationBufferByteCountUsed) uint8_t* securityInformationBuffer,
    _Outptr_ XNetworkingSecurityInformation** securityInformation
    ) noexcept;

STDAPI XNetworkingVerifyServerCertificate(
    _In_ void* requestHandle,
    _In_ const XNetworkingSecurityInformation* securityInformation
    ) noexcept;

enum class XNetworkingConnectivityLevelHint : uint32_t
{
    Unknown = 0,
    None = 1,
    LocalAccess = 2,
    InternetAccess = 3,
    ConstrainedInternetAccess = 4,
};

enum class XNetworkingConnectivityCostHint : uint32_t
{
    Unknown = 0,
    Unrestricted = 1,
    Fixed = 2,
    Variable = 3,
};

struct XNetworkingConnectivityHint
{
    XNetworkingConnectivityLevelHint connectivityLevel;
    XNetworkingConnectivityCostHint connectivityCost;
    uint32_t ianaInterfaceType;
    bool networkInitialized;
    bool approachingDataLimit;
    bool overDataLimit;
    bool roaming;
};

typedef void XNetworkingConnectivityHintChangedCallback(_In_opt_ void* context, _In_ const XNetworkingConnectivityHint* connectivityHint);

STDAPI XNetworkingGetConnectivityHint(
    _Out_ XNetworkingConnectivityHint* connectivityHint
    ) noexcept;

STDAPI XNetworkingRegisterConnectivityHintChanged(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XNetworkingConnectivityHintChangedCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XNetworkingUnregisterConnectivityHintChanged(
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

enum class XNetworkingConfigurationSetting : uint32_t
{
    MaxTitleTcpQueuedReceiveBufferSize = 0,
    MaxSystemTcpQueuedReceiveBufferSize = 1,
    MaxToolsTcpQueuedReceiveBufferSize = 2,
};

STDAPI XNetworkingQueryConfigurationSetting(
    _In_ XNetworkingConfigurationSetting configurationSetting,
    _Out_ uint64_t* value
    ) noexcept;

STDAPI XNetworkingSetConfigurationSetting(
    _In_ XNetworkingConfigurationSetting configurationSetting,
    _In_ uint64_t value
    ) noexcept;

enum class XNetworkingStatisticsType : uint32_t
{
    TitleTcpQueuedReceivedBufferUsage = 0,
    SystemTcpQueuedReceivedBufferUsage = 1,
    ToolsTcpQueuedReceivedBufferUsage = 2,
};

struct XNetworkingTcpQueuedReceivedBufferUsageStatistics
{
    uint64_t numBytesCurrentlyQueued;
    uint64_t peakNumBytesEverQueued;
    uint64_t totalNumBytesQueued;
    uint64_t numBytesDroppedForExceedingConfiguredMax;
    uint64_t numBytesDroppedDueToAnyFailure;
};

union XNetworkingStatisticsBuffer
{
    XNetworkingTcpQueuedReceivedBufferUsageStatistics tcpQueuedReceiveBufferUsage;
};

STDAPI XNetworkingQueryStatistics(
    _In_ XNetworkingStatisticsType statisticsType,
    _Out_ XNetworkingStatisticsBuffer* statisticsBuffer
    ) noexcept;

}
