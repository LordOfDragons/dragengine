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
pAurealizationMode( eaFull ),

pSoundTraceRayCount( 64 ),
pSoundTraceMaxBounceCount( 20 ),
pSoundTraceMaxTransmitCount( 2 ),
pEstimateRoomRayCount( 128 ),

pEAXReverbReflectionGainFactor( 1.0f ),
pEAXReverbLateReverbGainFactor( 1.0f ),

pAsyncAudio( true ),
pFrameRateLimit( 0 ), // 0 means disabled
pAsyncAudioSkipSyncTimeRatio( 0.5 )
{
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
	if( threshold < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( threshold == pStreamBufSizeThreshold ){
		return;
	}
	
	pStreamBufSizeThreshold = threshold;
	pDirty = true;
}

void deoalConfiguration::SetAurealizationMode( eAurealizationModes mode ){
	if( mode == pAurealizationMode ){
		return;
	}
	
	pAurealizationMode = mode;
	pDirty = true;
}



void deoalConfiguration::SetSoundTraceRayCount( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pSoundTraceRayCount ){
		return;
	}
	
	pSoundTraceRayCount = count;
	pDirty = true;
}

void deoalConfiguration::SetSoundTraceMaxBounceCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pSoundTraceMaxBounceCount ){
		return;
	}
	
	pSoundTraceMaxBounceCount = count;
	pDirty = true;
}

void deoalConfiguration::SetSoundTraceMaxTransmitCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pSoundTraceMaxTransmitCount ){
		return;
	}
	
	pSoundTraceMaxTransmitCount = count;
	pDirty = true;
}

void deoalConfiguration::SetEstimateRoomRayCount( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
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
	if( frameRateLimit == pFrameRateLimit ){
		return;
	}
	if( frameRateLimit < 0 ){
		DETHROW( deeInvalidParam );
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



// Operators
//////////////

deoalConfiguration &deoalConfiguration::operator=( const deoalConfiguration &config ){
	pDeviceName = config.pDeviceName;
	pEnableEFX = config.pEnableEFX;
	pStreamBufSizeThreshold = config.pStreamBufSizeThreshold;
	pDisableExtensions = config.pDisableExtensions;
	pAurealizationMode = config.pAurealizationMode;
	
	pSoundTraceRayCount = config.pSoundTraceRayCount;
	pSoundTraceMaxBounceCount = config.pSoundTraceMaxBounceCount;
	pSoundTraceMaxTransmitCount = config.pSoundTraceMaxTransmitCount;
	pEstimateRoomRayCount = config.pEstimateRoomRayCount;
	
	pEAXReverbReflectionGainFactor = config.pEAXReverbReflectionGainFactor;
	pEAXReverbLateReverbGainFactor = config.pEAXReverbLateReverbGainFactor;
	
	return *this;
}



// Private Functions
//////////////////////

void deoalConfiguration::pCleanUp(){
}
