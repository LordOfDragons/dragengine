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

#include <stdio.h>
#include <stdlib.h>

#include "deVideo.h"
#include "deVideoAudioDecoder.h"
#include "deVideoManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/video/deBaseVideoAudioDecoder.h"



// Class deVideoAudioDecoder
/////////////////////////

// Constructor, destructor
////////////////////////////

deVideoAudioDecoder::deVideoAudioDecoder( deVideoManager &manager, deVideo *video ) :
pVideoManager( manager ),
pVideo( video ),
pPeerVideo( NULL ),
pLLManagerPrev( NULL ),
pLLManagerNext( NULL )
{
	if( ! video ){
		DETHROW( deeInvalidParam );
	}
}

deVideoAudioDecoder::~deVideoAudioDecoder(){
	SetPeerVideo( NULL );
	
	pVideoManager.RemoveAudioDecoder( this );
}



// Management
///////////////

int deVideoAudioDecoder::GetPosition(){
	if( ! pPeerVideo ){
		DETHROW( deeInvalidParam );
	}
	return pPeerVideo->GetPosition();
}

void deVideoAudioDecoder::SetPosition( int position ){
	if( ! pPeerVideo ){
		DETHROW( deeInvalidParam );
	}
	pPeerVideo->SetPosition( position );
}

int deVideoAudioDecoder::ReadSamples( void *buffer, int size ){
	if( ! pPeerVideo ){
		DETHROW( deeInvalidParam );
	}
	return pPeerVideo->ReadSamples( buffer, size );
}



// System Peers
/////////////////

void deVideoAudioDecoder::SetPeerVideo( deBaseVideoAudioDecoder *peer ){
	if( peer == pPeerVideo ){
		return;
	}
	
	if( pPeerVideo ){
		delete pPeerVideo;
	}
	pPeerVideo = peer;
}

void deVideoAudioDecoder::SetLLManagerNext( deVideoAudioDecoder *resource ){
	pLLManagerNext = resource;
}

void deVideoAudioDecoder::SetLLManagerPrev( deVideoAudioDecoder *resource ){
	pLLManagerPrev = resource;
}

void deVideoAudioDecoder::MarkLeaking(){
	pLLManagerNext = NULL;
	pLLManagerPrev = NULL;
}
