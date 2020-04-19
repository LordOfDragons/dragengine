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

#include "deSound.h"
#include "deSoundDecoder.h"
#include "deSoundManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/sound/deBaseSoundDecoder.h"



// Class deSoundDecoder
/////////////////////////

// Constructor, destructor
////////////////////////////

deSoundDecoder::deSoundDecoder( deSoundManager &manager, deSound *sound ) :
pSoundManager( manager ),
pSound( sound ),
pPeerSound( NULL ),
pLLManagerPrev( NULL ),
pLLManagerNext( NULL )
{
	if( ! sound ){
		DETHROW( deeInvalidParam );
	}
}

deSoundDecoder::~deSoundDecoder(){
	SetPeerSound( NULL );
	
	pSoundManager.RemoveDecoder( this );
}



// Management
///////////////

int deSoundDecoder::GetPosition(){
	if( ! pPeerSound ){
		DETHROW( deeInvalidParam );
	}
	
	return pPeerSound->GetPosition();
}

void deSoundDecoder::SetPosition( int position ){
	if( ! pPeerSound ){
		DETHROW( deeInvalidParam );
	}
	
	pPeerSound->SetPosition( position );
}

int deSoundDecoder::ReadSamples( void *buffer, int size ){
	if( ! pPeerSound ){
		DETHROW( deeInvalidParam );
	}
	
	return pPeerSound->ReadSamples( buffer, size );
}



// System Peers
/////////////////

void deSoundDecoder::SetPeerSound( deBaseSoundDecoder *peer ){
	if( peer == pPeerSound ){
		return;
	}
	
	if( pPeerSound ){
		delete pPeerSound;
	}
	pPeerSound = peer;
}

void deSoundDecoder::SetLLManagerNext( deSoundDecoder *resource ){
	pLLManagerNext = resource;
}

void deSoundDecoder::SetLLManagerPrev( deSoundDecoder *resource ){
	pLLManagerPrev = resource;
}

void deSoundDecoder::MarkLeaking(){
	pLLManagerNext = NULL;
	pLLManagerPrev = NULL;
}
