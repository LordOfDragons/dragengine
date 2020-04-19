/* 
 * Drag[en]gine Game Engine
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deVideo.h"
#include "deVideoManager.h"
#include "deVideoPlayer.h"
#include "deVideoPlayerManager.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioVideoPlayer.h"
#include "../../systems/modules/graphic/deBaseGraphicVideoPlayer.h"



// Class deVideoPlayer
////////////////////////

// Constructor, destructor
////////////////////////////

deVideoPlayer::deVideoPlayer( deVideoPlayerManager *manager ) :
deResource( manager ),

pPlayState( epsStopped ),
pLooping( false ),
pPlayFrom( 0.0f ),
pPlayTo( 0.0f ),
pPlaySpeed( 1.0f ),
pPlayPosition( 0.0f ),

pPeerGraphic( NULL ),
pPeerAudio( NULL ){
}

deVideoPlayer::~deVideoPlayer(){
	pCleanUp();
}



// Management
///////////////

void deVideoPlayer::SetVideo( deVideo *video ){
	if( video == pVideo ){
		return;
	}
	
	pVideo = video;
	
	pPlayFrom = 0.0f;
	
	if( video ){
		pPlayTo = video->GetPlayTime();
		
	}else{
		pPlayTo = 0.0f;
	}
	
	pPlayPosition = pPlayFrom;
	
	if( pPeerGraphic ){
		pPeerGraphic->SourceChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->SourceChanged();
	}
}



void deVideoPlayer::SetLooping( bool looping ){
	if( looping == pLooping ){
		return;
	}
	
	pLooping = looping;
	
	if( pPeerGraphic ){
		pPeerGraphic->LoopingChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->LoopingChanged();
	}
}

void deVideoPlayer::SetPlayRange( float playFrom, float playTo ){
	playFrom = decMath::max( playFrom, 0.0f );
	playTo = decMath::min( playTo, playFrom );
	
	if( fabsf( playFrom - pPlayFrom ) <= FLOAT_SAFE_EPSILON && fabsf( playTo - pPlayTo ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPlayFrom = playFrom;
	pPlayTo = playTo;
	
	if( pPlayPosition < pPlayFrom ){
		pPlayPosition = pPlayFrom;
	}
	if( pPlayPosition > pPlayTo ){
		pPlayPosition = pPlayTo;
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->PlayRangeChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->PlayRangeChanged();
	}
}

void deVideoPlayer::SetPlaySpeed( float playSpeed ){
	playSpeed = decMath::max( playSpeed, 0.0f );
	
	if( fabsf( playSpeed - pPlaySpeed ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPlaySpeed = playSpeed;
	
	if( pPeerGraphic ){
		pPeerGraphic->PlaySpeedChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->PlaySpeedChanged();
	}
}

void deVideoPlayer::SetPlayPosition( float position ){
	pSetPlayPosition( position, true );
}



void deVideoPlayer::SetPlayState( ePlayState playState ){
	if( playState == pPlayState ){
		return;
	}
	
	pPlayState = playState;
	
	if( pPeerGraphic ){
		pPeerGraphic->PlayStateChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->PlayStateChanged();
	}
}

void deVideoPlayer::Play(){
	SetPlayState( epsPlaying );
}

void deVideoPlayer::Stop(){
	SetPlayState( epsStopped );
}

void deVideoPlayer::Pause(){
	SetPlayState( epsPaused );
}



void deVideoPlayer::Update( float elapsed ){
	if( pPlayState != epsPlaying ){
		return;
	}
	
	const float position = pPlayPosition + elapsed * pPlaySpeed;
	
	if( pLooping ){
		pSetPlayPosition( decMath::normalize( position, pPlayFrom, pPlayTo ), false );
		
	}else{
		pSetPlayPosition( position, false );
		
		if( position >= pPlayTo ){
			Stop();
		}
	}
}



// System Peers
/////////////////

void deVideoPlayer::SetPeerGraphic( deBaseGraphicVideoPlayer *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}

void deVideoPlayer::SetPeerAudio( deBaseAudioVideoPlayer *peer ){
	if( peer == pPeerAudio ){
		return;
	}
	
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}



// Private Functions
//////////////////////

void deVideoPlayer::pCleanUp(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	if( pPeerAudio ){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
}

void deVideoPlayer::pSetPlayPosition( float position, bool seeking ){
	position = decMath::clamp( position, pPlayFrom, pPlayTo );
	if( fabsf( position - pPlayPosition ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPlayPosition = position;
	
	if( pPeerGraphic ){
		pPeerGraphic->PlayPositionChanged( seeking );
	}
	if( pPeerAudio ){
		pPeerAudio->PlayPositionChanged( seeking );
	}
}
