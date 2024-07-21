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
    // Broadcast
    struct XAppBroadcastStatus
    {
        bool canStartBroadcast;
        bool isAnyAppBroadcasting;
        bool isCaptureResourceUnavailable;
        bool isGameStreamInProgress;
        bool isGpuConstrained;
        bool isAppInactive;
        bool isBlockedForApp;
        bool isDisabledByUser;
        bool isDisabledBySystem;
    };

    typedef void CALLBACK XAppBroadcastMonitorCallback(_In_ void* context);

    STDAPI XAppBroadcastShowUI(_In_ XUserHandle requestingUser) noexcept;
    STDAPI XAppBroadcastGetStatus(_In_ XUserHandle requestingUser, _Out_ XAppBroadcastStatus* appBroadcastStatus) noexcept;

    STDAPI_(bool) XAppBroadcastIsAppBroadcasting() noexcept;

    STDAPI XAppBroadcastRegisterIsAppBroadcastingChanged(
                    _In_opt_ XTaskQueueHandle queue,
                    _In_opt_ void* context,
                    _In_ XAppBroadcastMonitorCallback* appBroadcastMonitorCallback,
                    _Out_ XTaskQueueRegistrationToken* token) noexcept;

    STDAPI_(bool) XAppBroadcastUnregisterIsAppBroadcastingChanged(
        _In_ XTaskQueueRegistrationToken token,
        _In_ bool wait) noexcept;

    // Metadata
    enum class XAppCaptureMetadataPriority : uint8_t
    {
        Informational = 0,
        Important,
    };

    typedef void CALLBACK XAppCaptureMetadataPurgedCallback(_In_ void* context);

    STDAPI XAppCaptureMetadataAddStringEvent(_In_z_ const char* name,
                                            _In_z_ const char* value,
                                            _In_ XAppCaptureMetadataPriority priority) noexcept; 

    STDAPI XAppCaptureMetadataAddInt32Event(_In_z_ const char* name,
                                        _In_ int32_t value,
                                        _In_ XAppCaptureMetadataPriority priority) noexcept; 

    STDAPI XAppCaptureMetadataAddDoubleEvent(_In_z_ const char* name,
                                            _In_ double value,
                                            _In_ XAppCaptureMetadataPriority priority) noexcept; 

    STDAPI XAppCaptureMetadataStartStringState( _In_z_ const char* name,
                                            _In_z_ const char* value,
                                            _In_ XAppCaptureMetadataPriority priority) noexcept; 

    STDAPI XAppCaptureMetadataStartInt32State(_In_z_ const char* name,
                                            _In_ int32_t value,
                                            _In_ XAppCaptureMetadataPriority priority) noexcept; 

    STDAPI XAppCaptureMetadataStartDoubleState(_In_z_ const char* name,
                                            _In_ double value,
                                            _In_ XAppCaptureMetadataPriority priority) noexcept; 
    
    STDAPI XAppCaptureMetadataStopState(_In_z_ const char* name) noexcept; 
    
    STDAPI XAppCaptureMetadataStopAllStates() noexcept; 
    
    STDAPI XAppCaptureMetadataRemainingStorageBytesAvailable(_Out_ uint64_t* value) noexcept; 

    STDAPI XAppCaptureRegisterMetadataPurged(_In_opt_ XTaskQueueHandle queue,
                                        _In_ void* context,
                                        _In_ XAppCaptureMetadataPurgedCallback* callback,
                                        _Out_ XTaskQueueRegistrationToken* token) noexcept; 
    
    STDAPI_(bool) XAppCaptureUnRegisterMetadataPurged(_In_ XTaskQueueRegistrationToken token, _In_ bool wait) noexcept; 

    // Diagnostic APIs for developers

    const uint8_t APPCAPTURE_MAX_CAPTURE_FILES = 10;

    enum class XAppCaptureVideoEncoding : uint8_t
    {
        H264 = 0,
        HEVC = 1
    };

    struct XAppCaptureScreenshotFile
    {
        _Field_z_ char path[MAX_PATH];
        size_t fileSize;
        uint32_t width;
        uint32_t height;
    };

    enum class XAppCaptureScreenshotFormatFlag : uint16_t
    {
        SDR = 1,
        HDR = 2
    };
    DEFINE_ENUM_FLAG_OPERATORS(XAppCaptureScreenshotFormatFlag);

    struct XAppCaptureDiagnosticScreenshotResult
    {
        size_t fileCount;
        XAppCaptureScreenshotFile files[APPCAPTURE_MAX_CAPTURE_FILES];
    };

    struct XAppCaptureRecordClipResult
    {
        _Field_z_ char path[MAX_PATH];
        size_t fileSize;
        time_t startTime;
        uint32_t durationInMs;
        uint32_t width;
        uint32_t height;
        XAppCaptureVideoEncoding encoding;
        uint32_t startTimePreciseOffsetHns;
    };

    STDAPI XAppCaptureTakeDiagnosticScreenshot(
        _In_ bool gamescreenOnly,
        _In_ XAppCaptureScreenshotFormatFlag captureFlags,
        _In_opt_ const char* filenamePrefix,
        _Out_ XAppCaptureDiagnosticScreenshotResult* result
    ) noexcept;

    STDAPI XAppCaptureRecordDiagnosticClip(
        _In_ time_t startTime,
        _In_ uint32_t durationInMs,
        _In_opt_ const char* filenamePrefix,
        _Out_ XAppCaptureRecordClipResult* result
    ) noexcept;

    // Local capture APIs

    const int APPCAPTURE_MAX_LOCALID_LENGTH = 250;

    enum class XAppCaptureVideoColorFormat : uint8_t
    {
        SDR = 0,
        HDR = 1
    };

    typedef struct XAppCaptureLocalStream* XAppCaptureLocalStreamHandle;

    struct XAppCaptureVideoCaptureSettings
    {
        uint32_t width;
        uint32_t height;
        uint64_t maxRecordTimespanDurationInMs;
        XAppCaptureVideoEncoding encoding;
        XAppCaptureVideoColorFormat colorFormat;
        bool isCaptureByGamesAllowed;
    };

    struct XAppCaptureLocalResult
    {
        XAppCaptureLocalStreamHandle clipHandle;
        size_t fileSizeInBytes;
        SYSTEMTIME clipStartTimestamp;
        uint64_t durationInMilliseconds;
        uint32_t width;
        uint32_t height;
        XAppCaptureVideoEncoding encoding;
        XAppCaptureVideoColorFormat colorFormat;
    };

    STDAPI XAppCaptureGetVideoCaptureSettings(
        _Out_ XAppCaptureVideoCaptureSettings* userCaptureSettings
    ) noexcept;

    STDAPI XAppCaptureRecordTimespan(
        _In_opt_ const SYSTEMTIME* startTimestamp,
        _In_ uint64_t durationInMilliseconds,
        _Out_ XAppCaptureLocalResult* result
    ) noexcept;

    STDAPI XAppCaptureReadLocalStream(
        _In_ XAppCaptureLocalStreamHandle handle,
        _In_ size_t startPosition,
        _In_ uint32_t bytesToRead,
        _Out_writes_to_(bytesToRead, *bytesWritten) uint8_t* buffer,
        _Out_ uint32_t* bytesWritten
    ) noexcept;

    STDAPI XAppCaptureCloseLocalStream(
        _In_ XAppCaptureLocalStreamHandle handle
    ) noexcept;

    struct XAppCaptureUserRecordingResult
    {
        size_t fileSizeInBytes;
        SYSTEMTIME clipStartTimestamp;
        uint64_t durationInMilliseconds;
        uint32_t width;
        uint32_t height;
        XAppCaptureVideoEncoding encoding;
        XAppCaptureVideoColorFormat colorFormat;
    };

    STDAPI XAppCaptureStartUserRecord(
        _In_ XUserHandle requestingUser,
        _In_ uint32_t localIdBufferLength,
        _Out_writes_z_(localIdBufferLength) char* localIdBuffer
    ) noexcept;

    STDAPI XAppCaptureStopUserRecord(
        _In_ const char* localId,
        _Out_ XAppCaptureUserRecordingResult* result
    ) noexcept;


    // Screenshot APIs

    typedef struct XAppCaptureScreenshotStream* XAppCaptureScreenshotStreamHandle;

    struct XAppCaptureTakeScreenshotResult
    {
        _Field_z_ char localId[APPCAPTURE_MAX_LOCALID_LENGTH];
        XAppCaptureScreenshotFormatFlag availableScreenshotFormats;
    };

    STDAPI XAppCaptureTakeScreenshot(
        _In_ XUserHandle requestingUser,
        _Out_ XAppCaptureTakeScreenshotResult* result
    ) noexcept;


    STDAPI XAppCaptureOpenScreenshotStream(
        _In_ const char* localId,
        _In_ XAppCaptureScreenshotFormatFlag screenshotFormat,
        _Out_ XAppCaptureScreenshotStreamHandle* handle,
        _Out_opt_ uint64_t* totalBytes) noexcept;

    STDAPI XAppCaptureReadScreenshotStream(
        _In_ XAppCaptureScreenshotStreamHandle handle,
        _In_ uint64_t startPosition,
        _In_ uint32_t bytesToRead,
        _Out_writes_to_(bytesToRead, *bytesWritten) uint8_t* buffer,
        _Out_ uint32_t* bytesWritten) noexcept;

    STDAPI XAppCaptureCloseScreenshotStream(_In_ XAppCaptureScreenshotStreamHandle handle) noexcept;

    // Settings APIs

    STDAPI XAppCaptureEnableRecord() noexcept; // enable recording and screenshots for user
    STDAPI XAppCaptureDisableRecord() noexcept; // disable recording and screenshots for user
} // extern "C"
