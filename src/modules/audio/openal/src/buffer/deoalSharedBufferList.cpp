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

#include "deoalSharedBuffer.h"
#include "deoalSharedBufferList.h"

#include <dragengine/common/exceptions.h>



// Class deoalSharedBufferList
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSharedBufferList::deoalSharedBufferList(){
}

deoalSharedBufferList::~deoalSharedBufferList(){
	const int count = pBuffers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoalSharedBuffer* )pBuffers.GetAt( i );
	}
	pBuffers.RemoveAll();
}



// Management
///////////////

deoalSharedBuffer *deoalSharedBufferList::ClaimBuffer( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pBuffers.GetCount();
	deoalSharedBuffer *buffer = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		buffer = ( deoalSharedBuffer* )pBuffers.GetAt( i );
		if( ! buffer->GetInUse() ){
			if( size > buffer->GetSize() ){
				buffer->SetSize( size );
			}
			buffer->SetInUse( true );
			return buffer;
		}
	}
	
	buffer = NULL;
	try{
		buffer = new deoalSharedBuffer;
		buffer->SetSize( size );
		buffer->SetInUse( true );
		pBuffers.Add( buffer );
		return buffer;
		
	}catch( const deException & ){
		if( buffer ){
			delete buffer;
		}
		throw;
	}
}

void deoalSharedBufferList::ReleaseBuffer( deoalSharedBuffer *buffer ){
	if( ! buffer ){
		DETHROW( deeInvalidParam );
	}
	
	buffer->SetInUse( false );
}
