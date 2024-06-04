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

#include "deVideo.h"
#include "deVideoDecoder.h"
#include "deVideoManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/video/deBaseVideoDecoder.h"



// Class deVideoDecoder
/////////////////////////

// Constructor, destructor
////////////////////////////

deVideoDecoder::deVideoDecoder( deVideoManager &manager, deVideo *video ) :
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

deVideoDecoder::~deVideoDecoder(){
	SetPeerVideo( NULL );
	
	pVideoManager.RemoveDecoder( this );
}



// Management
///////////////

int deVideoDecoder::GetPosition(){
	DEASSERT_TRUE( pPeerVideo )
	return pPeerVideo->GetPosition();
}

void deVideoDecoder::SetPosition( int position ){
	DEASSERT_TRUE( pPeerVideo )
	pPeerVideo->SetPosition( position );
}

bool deVideoDecoder::DecodeFrame( void *buffer, int size ){
	DEASSERT_TRUE( pPeerVideo )
	return pPeerVideo->DecodeFrame( buffer, size );
}



// System Peers
/////////////////

void deVideoDecoder::SetPeerVideo( deBaseVideoDecoder *peer ){
	if( peer == pPeerVideo ){
		return;
	}
	
	if( pPeerVideo ){
		delete pPeerVideo;
	}
	pPeerVideo = peer;
}

void deVideoDecoder::SetLLManagerNext( deVideoDecoder *resource ){
	pLLManagerNext = resource;
}

void deVideoDecoder::SetLLManagerPrev( deVideoDecoder *resource ){
	pLLManagerPrev = resource;
}

void deVideoDecoder::MarkLeaking(){
	pLLManagerNext = NULL;
	pLLManagerPrev = NULL;
}
