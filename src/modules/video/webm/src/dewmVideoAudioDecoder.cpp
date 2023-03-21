/* 
 * Drag[en]gine WebM Video Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deVideoWebm.h"
#include "dewmVideoAudioDecoder.h"
#include "dewmWebmReader.h"
#include "dewmAudioTrackCallback.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>



// Class dewmVideoAudioDecoder
////////////////////////////////

// Constructor, destructor
////////////////////////////

dewmVideoAudioDecoder::dewmVideoAudioDecoder( deVideoWebm &module, decBaseFileReader *file ) :
deBaseVideoAudioDecoder( file ),
pModule( module ),
pCallback( nullptr ),
pReader( nullptr ),
pParser( nullptr ),
pCurSample( 0 )
{
	try{
		pCallback = new dewmAudioTrackCallback( module );
		pReader = new dewmWebmReader( *file );
		pParser = new webm::WebmParser;
		
	}catch( const deException &e ){
		pCleanUp();
		pModule.LogException( e );
		throw;
	}
}

dewmVideoAudioDecoder::~dewmVideoAudioDecoder(){
	pCleanUp();
}



// Management
///////////////

int dewmVideoAudioDecoder::GetPosition(){
	return pCurSample;
}

void dewmVideoAudioDecoder::SetPosition( int position ){
	pEnsureStreamOpen();
	
	if( position < pCurSample ){
		pReader->SetPosition( 0 );
		pCallback->Rewind();
		pParser->DidSeek();
		pCurSample = 0;
	}
	
	if( position == pCurSample ){
		return;
	}
	
	// read audio until the right time is found
	pCallback->SetResBuffer( nullptr, position - pCurSample );
	DEASSERT_TRUE( pParser->Feed( pCallback, pReader ).ok() )
	pCurSample = position;
}

int dewmVideoAudioDecoder::ReadSamples( void *buffer, int size ){
	pEnsureStreamOpen();
	
	pCallback->SetResBuffer( buffer, size / pCallback->GetSampleSize() );
	DEASSERT_TRUE( pParser->Feed( pCallback, pReader ).ok() )
	
	pCurSample += pCallback->GetResPosition();
	return pCallback->GetResPosition() * pCallback->GetSampleSize();
}



// Private Functions
//////////////////////

void dewmVideoAudioDecoder::pCleanUp(){
	if( pParser ){
		delete pParser;
	}
	if( pReader ){
		delete pReader;
	}
	if( pCallback ){
		delete pCallback;
	}
}

void dewmVideoAudioDecoder::pEnsureStreamOpen(){
	// this is unfortunately required since we do not know
	// sample size until the stream is open
	if( pCallback->IsStreamOpen() ){
		return;
	}
	
	// open is done by using a 0 read buffer. this will open
	// the stream and stop at the first sample
	pCallback->SetResBuffer( nullptr, 0 );
	DEASSERT_TRUE( pParser->Feed( pCallback, pReader ).ok() )
}
