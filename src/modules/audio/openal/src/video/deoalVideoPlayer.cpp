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

#include "deoalVideoPlayer.h"
#include "deoalAVideoPlayer.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"
#include "../speaker/deoalSpeaker.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoalVideoPlayer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalVideoPlayer::deoalVideoPlayer( deAudioOpenAL &oal, deVideoPlayer &videoPlayer ) :
pOal( oal ),
pVideoPlayer( videoPlayer ),
pAVideoPlayer( new deoalAVideoPlayer( oal.GetAudioThread() ) ),
pDirtyVideo( true ),
pDirtyParameters( true ),
pDirtyPlayPosition( true ),
pRequiresSeeking( false ){
}

deoalVideoPlayer::~deoalVideoPlayer(){
	if( pAVideoPlayer ){
		pAVideoPlayer->FreeReference();
	}
}



// Management
///////////////

void deoalVideoPlayer::AddSpeaker( deoalSpeaker *speaker ){
	pSpeakers.AddIfAbsent( speaker );
}

void deoalVideoPlayer::RemoveSpeaker( deoalSpeaker *speaker ){
	pSpeakers.RemoveIfPresent( speaker );
}

void deoalVideoPlayer::Synchronize(){
	// WARNING Called during synchronization time from main thread.
	
	if( pDirtyVideo ){
		pAVideoPlayer->SetVideo( pVideoPlayer.GetVideo() );
		pDirtyVideo = false;
	}
	
	if( pDirtyParameters ){
		pAVideoPlayer->SetLooping( pVideoPlayer.GetLooping() );
		pAVideoPlayer->SetPlaySpeed( pVideoPlayer.GetPlaySpeed() );
		pAVideoPlayer->SetPlayState( pVideoPlayer.GetPlayState() );
		pAVideoPlayer->SetPlayRange( pVideoPlayer.GetPlayFrom(), pVideoPlayer.GetPlayTo() );
		pDirtyParameters = false;
	}
	
	if( pDirtyPlayPosition ){
		pAVideoPlayer->SetPlayPosition( pVideoPlayer.GetPlayPosition(), pRequiresSeeking );
		pRequiresSeeking = false;
		pDirtyPlayPosition = false;
	}
}



// Notifications
//////////////////

void deoalVideoPlayer::SourceChanged(){
	PlayRangeChanged(); // also sends VideoPlayerRequiresSync notification
	
	pDirtyVideo = true;
}

void deoalVideoPlayer::LoopingChanged(){
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalSpeaker &speaker = *( ( deoalSpeaker* )pSpeakers.GetAt( i ) );
		speaker.GetSpeaker().SetLooping( pVideoPlayer.GetLooping() );
		speaker.VideoPlayerRequiresSync();
	}
	
	pDirtyParameters = true;
}

void deoalVideoPlayer::PlayRangeChanged(){
	const int count = pSpeakers.GetCount();
	if( count > 0 ){
		int from = 0, to = 0;
		if( pVideoPlayer.GetVideo() ){
			const int sampleRate = pVideoPlayer.GetVideo()->GetSampleRate();
			from = ( int )( pVideoPlayer.GetPlayFrom() * sampleRate );
			to = ( int )( pVideoPlayer.GetPlayTo() * sampleRate );
		}
		
		int i;
		for( i=0; i<count; i++ ){
			deoalSpeaker &speaker = *( ( deoalSpeaker* )pSpeakers.GetAt( i ) );
			speaker.GetSpeaker().SetPlayPosition( from, to );
			speaker.VideoPlayerRequiresSync();
			speaker.DirtyResetStreaming();
		}
	}
	
	pDirtyParameters = true;
}

void deoalVideoPlayer::PlaySpeedChanged(){
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalSpeaker &speaker = *( ( deoalSpeaker* )pSpeakers.GetAt( i ) );
		speaker.GetSpeaker().SetPlaySpeed( pVideoPlayer.GetPlaySpeed() );
		speaker.VideoPlayerRequiresSync();
	}
	
	pDirtyParameters = true;
}

void deoalVideoPlayer::PlayPositionChanged( bool seeking ){
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalSpeaker &speaker = *( ( deoalSpeaker* )pSpeakers.GetAt( i ) );
		speaker.VideoPlayerRequiresSync();
		if( seeking ){
			speaker.DirtyResetStreaming();
		}
	}
	
	pDirtyPlayPosition = true;
	pRequiresSeeking |= seeking;
}

void deoalVideoPlayer::PlayStateChanged(){
	const int count = pSpeakers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalSpeaker &speaker = *( ( deoalSpeaker* )pSpeakers.GetAt( i ) );
		
		switch( pVideoPlayer.GetPlayState() ){
		case deVideoPlayer::epsPlaying:
			speaker.GetSpeaker().SetPlayState( deSpeaker::epsPlaying );
			break;
			
		case deVideoPlayer::epsPaused:
			speaker.GetSpeaker().SetPlayState( deSpeaker::epsPaused );
			break;
			
		case deVideoPlayer::epsStopped:
			speaker.GetSpeaker().SetPlayState( deSpeaker::epsStopped );
			break;
		}
		
		speaker.VideoPlayerRequiresSync();
	}
	
	pDirtyParameters = true;
}
