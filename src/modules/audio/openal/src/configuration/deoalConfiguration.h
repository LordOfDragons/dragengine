/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOALCONFIGURATION_H_
#define _DEOALCONFIGURATION_H_

#include <dragengine/common/string/decStringSet.h>



/**
 * OpenAL Configuration.
 */
class deoalConfiguration{
public:
	/** Auralization mode. */
	enum eAuralizationModes{
		/**
		 * Auralization disabled.
		 * 
		 * Direct sound without any material based effects. This mode is enforced if APU has
		 * no EFX support or EFX support has been disabled.
		 */
		eamDisabled,
		
		/**
		 * Direct sound auralization only.
		 * 
		 * Direct sound effect based on materials are used. This includes muffling of sound
		 * over different frquency bands. Uses single collision test along sound direction
		 * from source to listener to calculate the result.
		 */
		eamDirectSound,
		
		/**
		 * Full auralization.
		 * 
		 * Enables all auralization effects. Uses ray-tracing to calculate various parameters.
		 * This is expensive depending on the hardware used.
		 */
		eamFull
	};
	
	/** Auralization quality. */
	enum eAuralizationQuality{
		eaqVeryLow,
		eaqLow,
		eaqMedium,
		eaqHigh,
		eaqVeryHigh
	};
	
	
	
private:
	bool pDirty;
	
	decString pDeviceName;
	bool pEnableEFX;
	int pStreamBufSizeThreshold;
	decStringSet pDisableExtensions;
	eAuralizationModes pAuralizationMode;
	eAuralizationQuality pAuralizationQuality;
	
	int pSoundTraceRayCount;
	int pSoundTraceMaxBounceCount;
	int pSoundTraceMaxTransmitCount;
	int pEstimateRoomRayCount;
	
	float pEAXReverbReflectionGainFactor;
	float pEAXReverbLateReverbGainFactor;
	
	bool pAsyncAudio;
	int pFrameRateLimit;
	float pAsyncAudioSkipSyncTimeRatio;
	
	bool pUseSharedEffectSlots;
	float pShareEnvironmentThreshold;
	float pSwitchSharedEnvironmentThreshold;
	int pMaxSharedEffectSlots;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates configuration. */
	deoalConfiguration();
	
	/** Creates copy of configuration. */
	deoalConfiguration( const deoalConfiguration &config );
	
	/** Clean up openal configuration. */
	~deoalConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Configuration changed. */
	inline bool GetDirty() const{ return pDirty; }
	
	/** Set configuration changed. */
	void SetDirty( bool dirty );
	
	
	
	/** Device name or an empty string to use the default one. */
	inline const decString &GetDeviceName() const{ return pDeviceName; }
	
	/** Set device name or an empty string to use the default one. */
	void SetDeviceName( const char *deviceName );
	
	/** Effects are enabled. */
	inline bool GetEnableEFX() const{ return pEnableEFX; }
	
	/** Set if effects are enabled. */
	void SetEnableEFX( bool enable );
	
	/** Buffer size threshold to stream sound samples. */
	inline int GetStreamBufSizeThreshold() const{ return pStreamBufSizeThreshold; }
	
	/** Set buffer size threshold to stream sound samples. */
	void SetStreamBufSizeThreshold( int threshold );
	
	/** Disable extensions. */
	inline decStringSet &GetDisableExtensions(){ return pDisableExtensions; }
	inline const decStringSet &GetDisableExtensions() const{ return pDisableExtensions; }
	
	/** Auralization mode */
	inline eAuralizationModes GetAuralizationMode() const{ return pAuralizationMode; }
	
	/** Set Auralization mode. */
	void SetAuralizationMode( eAuralizationModes mode );
	
	/** Auralization quality */
	inline eAuralizationQuality GetAuralizationQuality() const{ return pAuralizationQuality; }
	
	/** Set Auralization quality. */
	void SetAuralizationQuality( eAuralizationQuality quality );
	
	
	
	/** Number of sound tracing rays. */
	inline int GetSoundTraceRayCount() const{ return pSoundTraceRayCount; }
	
	/** Set number of sound tracing rays. */
	void SetSoundTraceRayCount( int count );
	
	/** Maximum number of bounces to use for sound tracing. */
	inline int GetSoundTraceMaxBounceCount() const{ return pSoundTraceMaxBounceCount; }
	
	/** Set maximum number of bounces to use for sound tracing. */
	void SetSoundTraceMaxBounceCount( int count );
	
	/** Maximum number of transmission to use for sound tracing. */
	inline int GetSoundTraceMaxTransmitCount() const{ return pSoundTraceMaxTransmitCount; }
	
	/** Set maximum number of transmission to use for sound tracing. */
	void SetSoundTraceMaxTransmitCount( int count );
	
	/** Number of estimate room rays. */
	inline int GetEstimateRoomRayCount() const{ return pEstimateRoomRayCount; }
	
	/** Set number of estimate room rays. */
	void SetEstimateRoomRayCount( int count );
	
	
	
	/** EAX reverb filter reflection gain factor. */
	inline float GetEAXReverbReflectionGainFactor() const{ return pEAXReverbReflectionGainFactor; }
	
	/** Set EAX reverb filter reflection gain factor. */
	void SetEAXReverbReflectionGainFactor( float factor );
	
	/** EAX reverb filter late reverb gain factor. */
	inline float GetEAXReverbLateReverbGainFactor() const{ return pEAXReverbLateReverbGainFactor; }
	
	/** Set EAX reverb filter late reverb gain factor. */
	void SetEAXReverbLateReverbGainFactor( float factor );
	
	
	
	/** Frame rate limit for audio thread. */
	inline int GetFrameRateLimit() const{ return pFrameRateLimit; }
	
	/** Set frame rate limit for audio thread. */
	void SetFrameRateLimit( int frameRateLimit );
	
	/** Ratio between game time and audio time required to skip synchronization. */
	inline float GetAsyncAudioSkipSyncTimeRatio() const{ return pAsyncAudioSkipSyncTimeRatio; }
	
	/** Set ratio between game time and audio time required to skip synchronization. */
	void SetAsyncAudioSkipSyncTimeRatio( float ratio );
	
	/** Asynchronous audio. */
	inline bool GetAsyncAudio() const{ return pAsyncAudio; }
	
	/** Set asynchronous audio. */
	void SetAsyncAudio( bool asyncAudio );
	
	/** Use shared effect slots. */
	inline bool GetUseSharedEffectSlots() const{ return pUseSharedEffectSlots; }
	
	/** Set use shared effect slots. */
	void SetUseSharedEffectSlots( bool useUseSharedEffectSlots );
	
	/** Share environment threshold. */
	inline float GetShareEnvironmentThreshold() const{ return pShareEnvironmentThreshold; }
	
	/** Set share environment threshold. */
	void SetShareEnvironmentThreshold( float threshold );
	
	/** Switch shared environment threshold. */
	inline float GetSwitchSharedEnvironmentThreshold() const{ return pSwitchSharedEnvironmentThreshold; }
	
	/** Set switch shared environment threshold. */
	void SetSwitchSharedEnvironmentThreshold( float threshold );
	
	/** Maximum shared effect slot count. */
	inline int GetMaxSharedEffectSlots() const{ return pMaxSharedEffectSlots; }
	
	/** Set maximum shared effect slot count. */
	void SetMaxSharedEffectSlots( int count );
	/*@}*/
	
	
	
	/** \name Operator */
	/*@{*/
	/** Copy configuration. */
	deoalConfiguration &operator=( const deoalConfiguration &config );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pApplyAuralizationProfile();
};

#endif
