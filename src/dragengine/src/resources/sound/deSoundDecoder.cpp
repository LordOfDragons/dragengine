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
