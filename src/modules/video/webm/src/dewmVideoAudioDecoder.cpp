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
