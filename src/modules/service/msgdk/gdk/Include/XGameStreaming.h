// Copyright (c) Microsoft Corporation.  All rights reserved
#pragma once

#include <stdint.h>
#include <XGameRuntimeTypes.h>
#include <XTaskQueue.h>

#if !defined(__cplusplus)
#error C++11 required
#endif

extern "C"
{
    // Initalize Game Streaming APIs.
    STDAPI XGameStreamingInitialize() noexcept;

    // Uninitialize Game Streaming APIs.
    STDAPI_(void) XGameStreamingUninitialize() noexcept;

    // ID of a streaming client which is currently streaming the game
    typedef uint64_t XGameStreamingClientId;

    // Sentinel ID which represents the lack of a client.
    const XGameStreamingClientId XGameStreamingNullClientId = 0;
    
    // Sentinel ID which represents all connect clients
    const XGameStreamingClientId XGameStreamingAllClients = 0xFFFFFFFFFFFFFFFF;

    // Determine if the game is currently being streamed on xCloud.
    //
    // Returns true if the game is being streamed, false if it is being played locally.
    STDAPI_(bool) XGameStreamingIsStreaming() noexcept;

    // Get the number of clients which are currently streaming the game. This value may be zero if no clients
    // are currently streaming.
    STDAPI_(uint32_t) XGameStreamingGetClientCount() noexcept;

    // Get the ID of each currently connected client clients.
    STDAPI XGameStreamingGetClients(
        _In_ uint32_t clientCount,
        _Out_writes_to_(clientCount, *clientsUsed) XGameStreamingClientId* clients,
        _Out_ uint32_t* clientsUsed) noexcept;

    enum class XGameStreamingConnectionState : uint32_t
    {
        Disconnected = 0,
        Connected = 1
    };

    // Get the current connection state of a client
    STDAPI_(XGameStreamingConnectionState) XGameStreamingGetConnectionState(
        _In_ XGameStreamingClientId client
    ) noexcept;

    typedef void CALLBACK XGameStreamingConnectionStateChangedCallback(
        _In_opt_ void* context,
        _In_ XGameStreamingClientId client,
        _In_ XGameStreamingConnectionState state
    );

    // Registers a callback to be called when a client state changed.
    STDAPI XGameStreamingRegisterConnectionStateChanged(
        _In_opt_ XTaskQueueHandle queue,
        _In_opt_ void* context,
        _In_ XGameStreamingConnectionStateChangedCallback* callback,
        _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

    // Unregisters a previously registered client state changed callback.
    STDAPI_(bool) XGameStreamingUnregisterConnectionStateChanged(
        _In_ XTaskQueueRegistrationToken token,
        bool wait
    ) noexcept;

    // Hides Touch controls if they are visible.
    STDAPI_(void) XGameStreamingHideTouchControls() noexcept;
    STDAPI_(void) XGameStreamingHideTouchControlsOnClient(_In_ XGameStreamingClientId client) noexcept;

    // Shows the requested set of Touch controls.
    STDAPI_(void) XGameStreamingShowTouchControlLayout(_In_opt_z_ const char* layout) noexcept;
    STDAPI_(void) XGameStreamingShowTouchControlLayoutOnClient(
        _In_ XGameStreamingClientId client,
        _In_opt_z_ const char* layout) noexcept;

    enum class XGameStreamingClientProperty : uint32_t
    {
        None = 0,
        StreamPhysicalDimensions = 1,
        TouchInputEnabled = 2,
        TouchBundleVersion = 4,
        IPAddress = 5,
        SessionId = 6,
        DisplayDetails = 7,
    };

    typedef void CALLBACK XGameStreamingClientPropertiesChangedCallback(
        _In_ void* context,
        _In_ XGameStreamingClientId client,
        _In_ uint32_t updatedPropertiesCount,
        _In_reads_(updatedPropertiesCount) XGameStreamingClientProperty* updatedProperties);

    // Registers a callback to be called when a streaming client's properties change.
    STDAPI XGameStreamingRegisterClientPropertiesChanged(
        _In_ XGameStreamingClientId client,
        _In_opt_ XTaskQueueHandle queue, 
        _In_opt_ void* context,
        _In_ XGameStreamingClientPropertiesChangedCallback* callback,
        _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

    STDAPI_(bool) XGameStreamingUnregisterClientPropertiesChanged(
        _In_ XGameStreamingClientId client,
        _In_ XTaskQueueRegistrationToken token,
        _In_ bool wait
    ) noexcept;

    STDAPI XGameStreamingGetStreamPhysicalDimensions(
        _In_ XGameStreamingClientId client,
        _Out_ uint32_t* horizontalMm,
        _Out_ uint32_t* verticalMm) noexcept;

    //Gets the most recent latency measurements.
    STDAPI XGameStreamingGetStreamAddedLatency(
        _In_ XGameStreamingClientId client,
        _Out_ uint32_t* averageInputLatencyUs,
        _Out_ uint32_t* averageOutputLatencyUs,
        _Out_ uint32_t* standardDeviationUs) noexcept;

    // Gets the streaming server's location's size. Must be used to allocate space on a buffer for XGameStreamingGetServerLocationName.
    STDAPI_(size_t) XGameStreamingGetServerLocationNameSize() noexcept;

    // Gets the streaming server's location. Must have used XGameStreamingGetServerLocationNameSize in advance to get the size of the value to allocate a buffer.
    STDAPI XGameStreamingGetServerLocationName(_In_ size_t serverLocationNameSize, _Out_writes_z_(serverLocationNameSize) char* serverLocationName) noexcept;

    // Check if a client will send touch inputs to the game.
    STDAPI XGameStreamingIsTouchInputEnabled(
        _In_ XGameStreamingClientId client,
        _Out_ bool* touchInputEnabled
    ) noexcept;

    typedef UINT64 D3D12XBOX_FRAME_PIPELINE_TOKEN;

    // Gets the frame pipeline token for the frame that was most recently displayed on the streaming
    // client's screen
    STDAPI XGameStreamingGetLastFrameDisplayed(
        _In_ XGameStreamingClientId client,
        _Out_ D3D12XBOX_FRAME_PIPELINE_TOKEN* framePipelineToken
    ) noexcept;

    struct IGameInputReading;

    // Gets the frame pipeline token from a specific input reading
    STDAPI XGameStreamingGetAssociatedFrame(
        _In_ IGameInputReading* gamepadReading, _Out_ D3D12XBOX_FRAME_PIPELINE_TOKEN* framePipelineToken) noexcept;

    // Physicality mapping for every button, stick, and trigger on a standard Xbox Wireless
    // Controller. For any given button/stick/trigger, physicality can be unknown, physical,
    // virtual, or both.
    enum class XGameStreamingGamepadPhysicality : uint64_t
    {
        None = 0x0000000000000000,
        DPadUpPhysical = 0x0000000000000001,
        DPadDownPhysical = 0x0000000000000002,
        DPadLeftPhysical = 0x0000000000000004,
        DPadRightPhysical = 0x0000000000000008,
        MenuPhysical = 0x0000000000000010,
        ViewPhysical = 0x0000000000000020,
        LeftThumbstickPhysical = 0x0000000000000040,
        RightThumbstickPhysical = 0x0000000000000080,
        LeftShoulderPhysical = 0x0000000000000100,
        RightShoulderPhysical = 0x0000000000000200,
        APhysical = 0x0000000000001000,
        BPhysical = 0x0000000000002000,
        XPhysical = 0x0000000000004000,
        YPhysical = 0x0000000000008000,
        LeftTriggerPhysical = 0x0000000000010000,
        RightTriggerPhysical = 0x0000000000020000,
        LeftThumbstickXPhysical = 0x0000000000040000,
        LeftThumbstickYPhysical = 0x0000000000080000,
        RightThumbstickXPhysical = 0x0000000000100000,
        RightThumbstickYPhysical = 0x0000000000200000,
        ButtonsPhysical = 0x000000000000F3FF,
        AnalogsPhysical = 0x00000000003F0000,
        AllPhysical = 0x00000000003FF3FF,

        DPadUpVirtual = 0x0000000100000000,
        DPadDownVirtual = 0x0000000200000000,
        DPadLeftVirtual = 0x0000000400000000,
        DPadRightVirtual = 0x0000000800000000,
        MenuVirtual = 0x0000001000000000,
        ViewVirtual = 0x0000002000000000,
        LeftThumbstickVirtual = 0x0000004000000000,
        RightThumbstickVirtual = 0x0000008000000000,
        LeftShoulderVirtual = 0x0000010000000000,
        RightShoulderVirtual = 0x0000020000000000,
        AVirtual = 0x0000100000000000,
        BVirtual = 0x0000200000000000,
        XVirtual = 0x0000400000000000,
        YVirtual = 0x0000800000000000,
        LeftTriggerVirtual = 0x0001000000000000,
        RightTriggerVirtual = 0x0002000000000000,
        LeftThumbstickXVirtual = 0x0004000000000000,
        LeftThumbstickYVirtual = 0x0008000000000000,
        RightThumbstickXVirtual = 0x0010000000000000,
        RightThumbstickYVirtual = 0x0020000000000000,
        ButtonsVirtual = 0x0000F3FF00000000,
        AnalogsVirtual = 0x003F000000000000,
        AllVirtual = 0x003FF3FF00000000,
    };

    DEFINE_ENUM_FLAG_OPERATORS(XGameStreamingGamepadPhysicality);

    // Gets the input physicality mapping from a specific gamepad reading
    STDAPI XGameStreamingGetGamepadPhysicality(
        _In_ IGameInputReading* gamepadReading,
        _Out_ XGameStreamingGamepadPhysicality* gamepadPhysicality
    ) noexcept;

    enum class XGameStreamingTouchControlsStateOperationKind : uint32_t
    {
        Replace = 0
    };

    enum class XGameStreamingTouchControlsStateValueKind : uint32_t
    {
        Boolean = 0,
        Integer = 1,
        Double = 2,
        String = 3
    };

    struct XGameStreamingTouchControlsStateValue
    {
        XGameStreamingTouchControlsStateValueKind valueKind;
        union
        {
            bool booleanValue;
            int64_t integerValue;
            double doubleValue;
            const char* stringValue;
        };
    };

    struct XGameStreamingTouchControlsStateOperation
    {
        XGameStreamingTouchControlsStateOperationKind operationKind;
        const char* path;
        XGameStreamingTouchControlsStateValue value;
    };

    // Update the touch control state variables.
    STDAPI XGameStreamingUpdateTouchControlsState(
        _In_ size_t operationCount,
        _In_reads_opt_(operationCount) const XGameStreamingTouchControlsStateOperation* operations
    ) noexcept;

    // Update the touch control state variables for a specified connected client.
    STDAPI XGameStreamingUpdateTouchControlsStateOnClient(
        _In_ XGameStreamingClientId client,
        _In_ size_t operationCount,
        _In_reads_opt_(operationCount) const XGameStreamingTouchControlsStateOperation* operations
    ) noexcept;

    // Update the touch control layout and state variables.
    STDAPI XGameStreamingShowTouchControlsWithStateUpdate(
        _In_opt_z_ const char* layout,
        _In_ size_t operationCount,
        _In_reads_opt_(operationCount) const XGameStreamingTouchControlsStateOperation* operations
    ) noexcept;

    // Update the touch control layout and state variables for a specified connected client.
    STDAPI XGameStreamingShowTouchControlsWithStateUpdateOnClient(
        _In_ XGameStreamingClientId client,
        _In_opt_z_ const char* layout,
        _In_ size_t operationCount,
        _In_reads_opt_(operationCount) const XGameStreamingTouchControlsStateOperation* operations
    ) noexcept;

    // Get the number of bytes required to store the version name of the touch adaptation bundle
    // currently in use by the client app.
    STDAPI_(size_t) XGameStreamingGetTouchBundleVersionNameSize(
        _In_ XGameStreamingClientId client
    ) noexcept;

    // Get the version number and version name of the touch adaptation bundle currently in use by
    // the client app
    STDAPI XGameStreamingGetTouchBundleVersion(
        _In_ XGameStreamingClientId client,
        _Out_opt_ XVersion* version,
        _In_ size_t versionNameSize,
        _Out_writes_opt_z_(versionNameSize) char* versionName 
    ) noexcept;

    // The recommended size of the ipAddress parameter for the XGameStreamingGetClientIPAddress API.
    const size_t ClientIPAddressMaxBytes = 65;

    // Gets the specified client's IP address that the server is communicating with.
    STDAPI XGameStreamingGetClientIPAddress(
        _In_ XGameStreamingClientId client,
        _In_ size_t ipAddressSize,
        _Out_writes_z_(ipAddressSize) char* ipAddress
    ) noexcept;

    // The recommended size of the sessionId parameter for XGameStreamingGetSessionId API. 
    const size_t SessionIdMaxBytes = 256;

    // Gets the specified client's session id.
    STDAPI XGameStreamingGetSessionId(
        _In_ XGameStreamingClientId client,
        _In_ size_t sessionIdSize,
        _Out_writes_bytes_to_(sessionIdSize, *sessionIdUsed) char* sessionId,
        _Out_opt_ size_t* sessionIdUsed
    ) noexcept;

    // Video related features that are supported.
    enum class XGameStreamingVideoFlags : uint32_t
    {
        None = 0x0,
        SupportsCustomAspectRatio = 0x1,
        SupportsPresentScaling = 0x2,
        All = 0x3,
    };

    DEFINE_ENUM_FLAG_OPERATORS(XGameStreamingVideoFlags);

    // Data about the display being streamed to.
    struct XGameStreamingDisplayDetails
    {
        uint32_t preferredWidth;
        uint32_t preferredHeight;
        RECT safeArea;
        uint32_t maxPixels;
        uint32_t maxWidth;
        uint32_t maxHeight;
        XGameStreamingVideoFlags flags;
    };

    // Retrieves the display details for the give client.
    STDAPI XGameStreamingGetDisplayDetails(
        _In_ XGameStreamingClientId client,
        _In_ uint32_t maxSupportedPixels,
        _In_ float widestSupportedAspectRatio,
        _In_ float tallestSupportedAspectRatio,
        _Out_ XGameStreamingDisplayDetails* displayDetails
    ) noexcept;

    // Sets the stream resolution.
    STDAPI XGameStreamingSetResolution(_In_ uint32_t width, _In_ uint32_t height) noexcept;
}
