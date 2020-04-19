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
#include <stdint.h>

#include "deoalAVideoPlayer.h"
#include "deoalVideoPlayer.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoAudioDecoder.h>
#include <dragengine/resources/video/deVideoManager.h>


// Class deoalAVideoPlayer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAVideoPlayer::deoalAVideoPlayer( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pPlayState( deVideoPlayer::epsStopped ),
pPlayPosition( 0.0f ),
pPlayFrom( 0.0f ),
pPlayTo( 0.0f ),
pPlaySpeed( 1.0f ),
pLooping( false ),
pBytesPerSample( 1 ),
pChannelCount( 1 ),
pSampleRate( 11025 ),
pSampleCount( 0 ),
pBufferSampleSize( pBytesPerSample * pSampleRate ),
pSeekNextRead( false ),
pUpdateTracker( 1 )
{
	LEAK_CHECK_CREATE( audioThread, VideoPlayer );
}

deoalAVideoPlayer::~deoalAVideoPlayer(){
	LEAK_CHECK_FREE( pAudioThread, VideoPlayer );
	
	pCleanUp();
}



// Management
///////////////

void deoalAVideoPlayer::SetVideo( deVideo *video ){
	// WARNING Called during synchronization time by main thread
	
	pDecoder = NULL;
	pBytesPerSample = 1;
	pChannelCount = 1;
	pSampleRate = 11025;
	pSampleCount = 0;
	pBufferSampleSize = pBytesPerSample * pSampleRate;
	
	if( ! video ){
		return;
	}
	
	switch( video->GetBytesPerSample() ){
	case 1:
	case 2:
		break;
		
	default:
		return;
	}
	
	pDecoder.TakeOver( pAudioThread.GetOal().GetGameEngine()->GetVideoManager()->CreateAudioDecoder( video ) );
	pBytesPerSample = video->GetBytesPerSample();
	pChannelCount = video->GetChannelCount();
	pSampleRate = video->GetSampleRate();
	pSampleCount = video->GetSampleCount();
	pBufferSampleSize = pBytesPerSample * pChannelCount;
	
	pAdvanceUpdateTracker();
}

void deoalAVideoPlayer::SetPlayState( deVideoPlayer::ePlayState playState ){
	pPlayState = playState;
}

void deoalAVideoPlayer::SetPlayPosition( float position, bool seeking ){
	pPlayPosition = position;
	pSeekNextRead |= seeking;
}

void deoalAVideoPlayer::SetPlayRange( float from, float to ){
	pPlayFrom = from;
	pPlayTo = to;
}

void deoalAVideoPlayer::SetPlaySpeed( float playSpeed ){
	pPlaySpeed = playSpeed;
}

void deoalAVideoPlayer::SetLooping( bool looping ){
	pLooping = looping;
}



void deoalAVideoPlayer::ReadSamples( void *buffer, int bufferSize, int offset, int samples ){
	char *dataBuffer = ( char* )buffer;
	
	if( pSeekNextRead ){
		pSeekNextRead = false;
		pDecoder->SetPosition( ( int )( pPlayPosition * pSampleRate ) );
	}
	
	while( true ){
		const int writtenCount = pDecoder->ReadSamples( dataBuffer, samples * pBufferSampleSize ) / pBufferSampleSize;
		if( writtenCount == samples ){
			return;
		}
		
		if( pLooping ){
			dataBuffer += pBufferSampleSize * writtenCount;
			samples -= writtenCount;
			
		}else{
			int i;
			if( pBytesPerSample == 1 ){
				int8_t * const silenceData = ( int8_t* )dataBuffer + writtenCount;
				for( i=writtenCount; i<samples; i++ ){
					silenceData[ i ] = 0;
				}
				
			}else{
				int16_t * const silenceData = ( int16_t* )dataBuffer + writtenCount * 2;
				for( i=writtenCount; i<samples; i++ ){
					silenceData[ i ] = 0;
				}
			}
			return;
		}
	}
}



// Private Functions
//////////////////////

void deoalAVideoPlayer::pCleanUp(){
}

void deoalAVideoPlayer::pAdvanceUpdateTracker(){
	pUpdateTracker++;
	if( pUpdateTracker == 0 ){
		pUpdateTracker = 1;
	}
}
