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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalConfiguration.h"
#include "../deAudioOpenAL.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoalConfiguration
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalConfiguration::deoalConfiguration() :
pDirty( false ),

pEnableEFX( true ),
pStreamBufSizeThreshold( 700000 ), // see deoalSound.cpp
pAuralizationMode( eamFull ),
pAuralizationQuality( eaqMedium ),

pEstimateRoomRayCount( 128 ),

pEAXReverbReflectionGainFactor( 1.0f ),
pEAXReverbLateReverbGainFactor( 1.0f ),

pAsyncAudio( true ),
pFrameRateLimit( 0 ), // 0 means disabled
pAsyncAudioSkipSyncTimeRatio( 0.5 ),

pUseSharedEffectSlots( true ),
pShareEnvironmentThreshold( 0.05f ),
pSwitchSharedEnvironmentThreshold( 0.1f ),
pMaxSharedEffectSlots( 8 )
{
	pApplyAuralizationProfile();
}

deoalConfiguration::deoalConfiguration( const deoalConfiguration &config ){
	*this = config;
}

deoalConfiguration::~deoalConfiguration(){
}



// Management
///////////////

void deoalConfiguration::SetDirty( bool dirty ){
	pDirty = dirty;
}



void deoalConfiguration::SetDeviceName( const char *deviceName ){
	if( pDeviceName == deviceName ){
		return;
	}
	
	pDeviceName = deviceName;
	pDirty = true;
}

void deoalConfiguration::SetEnableEFX( bool enable ){
	if( enable == pEnableEFX ){
		return;
	}
	
	pEnableEFX = enable;
	pDirty = true;
}

void deoalConfiguration::SetStreamBufSizeThreshold( int threshold ){
	DEASSERT_TRUE( threshold > 0 )
	if( threshold == pStreamBufSizeThreshold ){
		return;
	}
	
	pStreamBufSizeThreshold = threshold;
	pDirty = true;
}

void deoalConfiguration::SetAuralizationMode( eAuralizationModes mode ){
	if( mode == pAuralizationMode ){
		return;
	}
	
	pAuralizationMode = mode;
	pDirty = true;
}

void deoalConfiguration::SetAuralizationQuality( eAuralizationQuality quality ){
	if( quality == pAuralizationQuality ){
		return;
	}
	
	pAuralizationQuality = quality;
	pDirty = true;
	pApplyAuralizationProfile();
}



void deoalConfiguration::SetSoundTraceRayCount( int count ){
	DEASSERT_TRUE( count > 0 )
	if( count == pSoundTraceRayCount ){
		return;
	}
	
	pSoundTraceRayCount = count;
	pDirty = true;
}

void deoalConfiguration::SetSoundTraceMaxBounceCount( int count ){
	DEASSERT_TRUE( count > 0 )
	if( count == pSoundTraceMaxBounceCount ){
		return;
	}
	
	pSoundTraceMaxBounceCount = count;
	pDirty = true;
}

void deoalConfiguration::SetSoundTraceMaxTransmitCount( int count ){
	DEASSERT_TRUE( count > 0 )
	if( count == pSoundTraceMaxTransmitCount ){
		return;
	}
	
	pSoundTraceMaxTransmitCount = count;
	pDirty = true;
}

void deoalConfiguration::SetEstimateRoomRayCount( int count ){
	DEASSERT_TRUE( count > 0 )
	if( count == pEstimateRoomRayCount ){
		return;
	}
	
	pEstimateRoomRayCount = count;
	pDirty = true;
}



void deoalConfiguration::SetEAXReverbReflectionGainFactor( float factor ){
	if( fabsf( factor - pEAXReverbReflectionGainFactor ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pEAXReverbReflectionGainFactor = factor;
	pDirty = true;
}

void deoalConfiguration::SetEAXReverbLateReverbGainFactor( float factor ){
	if( fabsf( factor - pEAXReverbLateReverbGainFactor ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pEAXReverbLateReverbGainFactor = factor;
	pDirty = true;
}



void deoalConfiguration::SetFrameRateLimit( int frameRateLimit ){
	DEASSERT_TRUE( frameRateLimit >= 0 )
	if( frameRateLimit == pFrameRateLimit ){
		return;
	}
	
	pFrameRateLimit = frameRateLimit;
	pDirty = true;
}

void deoalConfiguration::SetAsyncAudioSkipSyncTimeRatio( float ratio ){
	ratio = decMath::clamp( ratio, 0.0f, 1.0f );
	if( fabsf( ratio - pAsyncAudioSkipSyncTimeRatio ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pAsyncAudioSkipSyncTimeRatio = ratio;
	pDirty = true;
}

void deoalConfiguration::SetAsyncAudio( bool asyncAudio ){
	if( asyncAudio == pAsyncAudio ){
		return;
	}
	
	pAsyncAudio = asyncAudio;
	pDirty = true;
}

void deoalConfiguration::SetUseSharedEffectSlots( bool useUseSharedEffectSlots ){
	if( useUseSharedEffectSlots == pUseSharedEffectSlots ){
		return;
	}
	
	pUseSharedEffectSlots = useUseSharedEffectSlots;
	pDirty = true;
}

void deoalConfiguration::SetShareEnvironmentThreshold( float threshold ){
	threshold = decMath::max( threshold, 0.01f );
	if( fabsf( threshold - pShareEnvironmentThreshold ) < 0.001f ){
		return;
	}
	
	pShareEnvironmentThreshold = threshold;
	pDirty = true;
}

void deoalConfiguration::SetSwitchSharedEnvironmentThreshold( float threshold ){
	threshold = decMath::max( threshold, 0.01f );
	if( fabsf( threshold - pSwitchSharedEnvironmentThreshold ) < 0.001f ){
		return;
	}
	
	pSwitchSharedEnvironmentThreshold = threshold;
	pDirty = true;
}

void deoalConfiguration::SetMaxSharedEffectSlots( int count ){
	DEASSERT_TRUE( count >= 2 )
	DEASSERT_TRUE( count <= 8 )
	if( count == pMaxSharedEffectSlots ){
		return;
	}
	
	pMaxSharedEffectSlots = count;
	pDirty = true;
}



// Operators
//////////////

deoalConfiguration &deoalConfiguration::operator=( const deoalConfiguration &config ){
	pDeviceName = config.pDeviceName;
	pEnableEFX = config.pEnableEFX;
	pStreamBufSizeThreshold = config.pStreamBufSizeThreshold;
	pDisableExtensions = config.pDisableExtensions;
	pAuralizationMode = config.pAuralizationMode;
	pAuralizationQuality = config.pAuralizationQuality;
	
	pSoundTraceRayCount = config.pSoundTraceRayCount;
	pSoundTraceMaxBounceCount = config.pSoundTraceMaxBounceCount;
	pSoundTraceMaxTransmitCount = config.pSoundTraceMaxTransmitCount;
	pEstimateRoomRayCount = config.pEstimateRoomRayCount;
	
	pEAXReverbReflectionGainFactor = config.pEAXReverbReflectionGainFactor;
	pEAXReverbLateReverbGainFactor = config.pEAXReverbLateReverbGainFactor;
	
	pUseSharedEffectSlots = config.pUseSharedEffectSlots;
	pShareEnvironmentThreshold = config.pShareEnvironmentThreshold;
	pSwitchSharedEnvironmentThreshold = config.pSwitchSharedEnvironmentThreshold;
	pMaxSharedEffectSlots = config.pMaxSharedEffectSlots;
	return *this;
}



// Private Functions
//////////////////////

void deoalConfiguration::pCleanUp(){
}

void deoalConfiguration::pApplyAuralizationProfile(){
	// switch profile.
	// 
	// bounces up to 100 go farther but if walls are absorbing enough this is not often reached.
	// 
	// less than 10 bounces has negative effect on reverberation time calculation stability
	// and in general sounds worse if moving
	switch( pAuralizationQuality ){
	case eaqVeryHigh:
		// best working version for moving-sound scenario but creates 8x the ray segments.
		// not usable from a performance point of view unless HW-acceleration can be used.
		pSoundTraceRayCount = 256;
		pSoundTraceMaxBounceCount = 40;
		pSoundTraceMaxTransmitCount = 2;
		break;
		
	case eaqHigh:
		// works better for moving-sound scenario creating roughly 1.5x times the ray segments.
		// this would be usable from a performance point of view depending on the scene.
		pSoundTraceRayCount = 128;
		pSoundTraceMaxBounceCount = 30;
		pSoundTraceMaxTransmitCount = 2;
		break;
		
	case eaqMedium:
		// this version is working similar well for the moving-sound scenario creating also
		// roughly 1.5x times the ray segments. maybe slightly less good sounding.
		// this version is stable for reverberation time calculation.
		pSoundTraceRayCount = 64;
		pSoundTraceMaxBounceCount = 20;
		pSoundTraceMaxTransmitCount = 2;
		break;
		
	case eaqLow:
		pSoundTraceRayCount = 48;
		pSoundTraceMaxBounceCount = 18;
		pSoundTraceMaxTransmitCount = 2;
		break;
		
	case eaqVeryLow:
		// 32 is a possible value for lower performance paltforms. the gains are slightly
		// higher than on 64 but performance is roughly 2x faster
		pSoundTraceRayCount = 32;
		pSoundTraceMaxBounceCount = 16;
		pSoundTraceMaxTransmitCount = 2;
		break;
	}
}
