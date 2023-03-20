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
#include "dewmVideoDecoder.h"
#include "dewmWebmReader.h"
#include "dewmVPXTrackCallback.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/image/deImage.h>



// Class dewmVideoDecoder
///////////////////////////

// Constructor, destructor
////////////////////////////

dewmVideoDecoder::dewmVideoDecoder( deVideoWebm &module, decBaseFileReader *file ) :
deBaseVideoDecoder( file ),
pModule( module ),
pCallback( nullptr ),
pReader( nullptr ),
pParser( nullptr ),
pCurFrame( 0 )
{
	try{
		pCallback = new dewmVPXTrackCallback( module );
		pReader = new dewmWebmReader( *file );
		pParser = new webm::WebmParser;
		
	}catch( const deException &e ){
		pCleanUp();
		pModule.LogException( e );
		throw;
	}
}

dewmVideoDecoder::~dewmVideoDecoder(){
	pCleanUp();
}



// Management
///////////////

int dewmVideoDecoder::GetPosition(){
	return pCurFrame;
}

void dewmVideoDecoder::SetPosition( int position ){
	if( position == pCurFrame ){
		return;
	}
	
	pCallback->SetResBuffer( nullptr ); // process frames but do not output frame data
	
	if( position < pCurFrame ){
		pReader->SetPosition( 0 );
		pParser->DidSeek();
		pCurFrame = 0;
	}
	
	while( pCurFrame < position ){
		DEASSERT_TRUE( pParser->Feed( pCallback, pReader ).ok() )
		pCurFrame++;
	}
}

bool dewmVideoDecoder::DecodeFrame( void *buffer1, int, void*, int ){
	pCallback->SetResBuffer( buffer1 );
	if( ! pParser->Feed( pCallback, pReader ).ok() ){
		return false;
	}
	
	pCurFrame++;
	return true;
}



// Private Functions
//////////////////////

void dewmVideoDecoder::pCleanUp(){
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
