/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOALCONFIGURATION_H_
#define _DEOALCONFIGURATION_H_

#include <dragengine/common/string/decStringSet.h>



/**
 * \brief OpenAL Configuration.
 */
class deoalConfiguration{
public:
	/** \brief Aurealization mode. */
	enum eAurealizationModes{
		/**
		 * \brief Aurealization disabled.
		 * 
		 * Direct sound without any material based effects. This mode is enforced if APU has
		 * no EFX support or EFX support has been disabled.
		 */
		eaDisabled,
		
		/**
		 * \brief Direct sound aurealization only.
		 * 
		 * Direct sound effect based on materials are used. This includes muffling of sound
		 * over different frquency bands. Uses single collision test along sound direction
		 * from source to listener to calculate the result.
		 */
		eaDirectSound,
		
		/**
		 * \brief Full aurealization.
		 * 
		 * Enables all aurealization effects. Uses ray-tracing to calculate various parameters.
		 * This is expensive depending on the hardware used.
		 */
		eaFull
	};
	
	
	
private:
	bool pDirty;
	
	decString pDeviceName;
	bool pEnableEFX;
	int pStreamBufSizeThreshold;
	decStringSet pDisableExtensions;
	eAurealizationModes pAurealizationMode;
	
	int pSoundTraceRayCount;
	int pSoundTraceMaxBounceCount;
	int pSoundTraceMaxTransmitCount;
	int pEstimateRoomRayCount;
	
	float pEAXReverbReflectionGainFactor;
	float pEAXReverbLateReverbGainFactor;
	
	bool pAsyncAudio;
	int pFrameRateLimit;
	float pAsyncAudioSkipSyncTimeRatio;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates configuration. */
	deoalConfiguration();
	
	/** \brief Creates copy of configuration. */
	deoalConfiguration( const deoalConfiguration &config );
	
	/** \brief Clean up openal configuration. */
	~deoalConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Configuration changed. */
	inline bool GetDirty() const{ return pDirty; }
	
	/** \brief Set configuration changed. */
	void SetDirty( bool dirty );
	
	
	
	/** \brief Device name or an empty string to use the default one. */
	inline const decString &GetDeviceName() const{ return pDeviceName; }
	
	/** \brief Set device name or an empty string to use the default one. */
	void SetDeviceName( const char *deviceName );
	
	/** \brief Effects are enabled. */
	inline bool GetEnableEFX() const{ return pEnableEFX; }
	
	/** \brief Set if effects are enabled. */
	void SetEnableEFX( bool enable );
	
	/** \brief Buffer size threshold to stream sound samples. */
	inline int GetStreamBufSizeThreshold() const{ return pStreamBufSizeThreshold; }
	
	/** \brief Set buffer size threshold to stream sound samples. */
	void SetStreamBufSizeThreshold( int threshold );
	
	/** \brief Disable extensions. */
	inline decStringSet &GetDisableExtensions(){ return pDisableExtensions; }
	inline const decStringSet &GetDisableExtensions() const{ return pDisableExtensions; }
	
	/** \brief Aurealization mode */
	inline eAurealizationModes GetAurealizationMode() const{ return pAurealizationMode; }
	
	/** \brief Set aurealization mode. */
	void SetAurealizationMode( eAurealizationModes mode );
	
	
	
	/** \brief Number of sound tracing rays. */
	inline int GetSoundTraceRayCount() const{ return pSoundTraceRayCount; }
	
	/** \brief Set number of sound tracing rays. */
	void SetSoundTraceRayCount( int count );
	
	/** \brief Maximum number of bounces to use for sound tracing. */
	inline int GetSoundTraceMaxBounceCount() const{ return pSoundTraceMaxBounceCount; }
	
	/** \brief Set maximum number of bounces to use for sound tracing. */
	void SetSoundTraceMaxBounceCount( int count );
	
	/** \brief Maximum number of transmission to use for sound tracing. */
	inline int GetSoundTraceMaxTransmitCount() const{ return pSoundTraceMaxTransmitCount; }
	
	/** \brief Set maximum number of transmission to use for sound tracing. */
	void SetSoundTraceMaxTransmitCount( int count );
	
	/** \brief Number of estimate room rays. */
	inline int GetEstimateRoomRayCount() const{ return pEstimateRoomRayCount; }
	
	/** \brief Set number of estimate room rays. */
	void SetEstimateRoomRayCount( int count );
	
	
	
	/** \brief EAX reverb filter reflection gain factor. */
	inline float GetEAXReverbReflectionGainFactor() const{ return pEAXReverbReflectionGainFactor; }
	
	/** \brief Set EAX reverb filter reflection gain factor. */
	void SetEAXReverbReflectionGainFactor( float factor );
	
	/** \brief EAX reverb filter late reverb gain factor. */
	inline float GetEAXReverbLateReverbGainFactor() const{ return pEAXReverbLateReverbGainFactor; }
	
	/** \brief Set EAX reverb filter late reverb gain factor. */
	void SetEAXReverbLateReverbGainFactor( float factor );
	
	
	
	/** \brief Frame rate limit for audio thread. */
	inline int GetFrameRateLimit() const{ return pFrameRateLimit; }
	
	/** \brief Set frame rate limit for audio thread. */
	void SetFrameRateLimit( int frameRateLimit );
	
	/** \brief Ratio between game time and audio time required to skip synchronization. */
	inline float GetAsyncAudioSkipSyncTimeRatio() const{ return pAsyncAudioSkipSyncTimeRatio; }
	
	/** \brief Set ratio between game time and audio time required to skip synchronization. */
	void SetAsyncAudioSkipSyncTimeRatio( float ratio );
	
	/** \brief Asynchronous audio. */
	inline bool GetAsyncAudio() const{ return pAsyncAudio; }
	
	/** \brief Set asynchronous audio. */
	void SetAsyncAudio( bool asyncAudio );
	/*@}*/
	
	
	
	/** \name Operator */
	/*@{*/
	/** \brief Copy configuration. */
	deoalConfiguration &operator=( const deoalConfiguration &config );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
