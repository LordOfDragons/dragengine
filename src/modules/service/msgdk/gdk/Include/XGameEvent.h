// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

extern "C"
{

/// <summary> 
/// Write an in-game event that optionally includes "dimensions" and "measurement" data fields. 
/// 
/// Dimensions include event fields with a finite number of defined numeric or string values. 
/// Examples of dimensions: map id, difficulty level, character or weapon class, game mode, boolean settings, etc. 
///  
/// Measurements include event fields that represent scalar numeric metrics. 
/// Examples of measurements: score, time, counters, position, etc. 
/// 
/// Example: for an in-game event that tracks the highest match score for a difficulty level:  
/// The difficulty level should be included in dimensions, and the score should be included in measurements. 
/// 
/// The name of the event, and the names of the event fields in both dimensions and measurements, must match 
/// the names declared in the title's service configuration. The names are case insensitive. 
/// If the API writes an event with a name that does not match a name in the service configuration, the 
/// service drops the event with no notification. 
/// </summary> 
/// <param name="user">The User handle to send the event on behalf of</param> 
/// <param name="serviceConfigId">The Xbox Live service config ID (SCID) for this title</param> 
/// <param name="playSessionId">Play Session ID.  This is a unique GUID created by the game at the start 
/// of each game session to track the set of events sent in a single play session</param> 
/// <param name="eventName">Event name</param> 
/// <param name="dimensionsJson">Dimensions data fields in JSON format</param> 
/// <param name="measurementsJson">Measurement data fields in JSON format</param> 
STDAPI XGameEventWrite(
    _In_ XUserHandle user,
    _In_z_ const char *serviceConfigId,
    _In_z_ const char *playSessionId,
    _In_z_ const char* eventName,
    _In_opt_z_ const char* dimensionsJson,
    _In_opt_z_ const char* measurementsJson
    ) noexcept;

}
