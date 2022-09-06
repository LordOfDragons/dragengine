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
#include <string.h>

#include "deNetworkMessage.h"
#include "deNetworkMessageReader.h"
#include "../../common/exceptions.h"



// Class deNetworkMessageReader
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deNetworkMessageReader::deNetworkMessageReader( deNetworkMessage *message ) :
pMessage( message ),
pPosition( 0 )
{
	if( ! message ){
		DETHROW( deeInvalidParam );
	}
}

deNetworkMessageReader::~deNetworkMessageReader(){
}



// Management
///////////////

const char *deNetworkMessageReader::GetFilename(){
	return "<NetworkMessage>";
}

int deNetworkMessageReader::GetLength(){
	return pMessage->GetDataLength();
}

TIME_SYSTEM deNetworkMessageReader::GetModificationTime(){
	return pMessage->GetTimeStamp();
}



// Seeking
////////////

int deNetworkMessageReader::GetPosition(){
	return pPosition;
}

void deNetworkMessageReader::SetPosition( int position ){
	if( position < 0 || position > pMessage->GetDataLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = position;
}

void deNetworkMessageReader::MovePosition( int offset ){
	const int newPos = pPosition + offset;
	if( newPos < 0 || newPos > pMessage->GetDataLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = newPos;
}

void deNetworkMessageReader::SetPositionEnd( int position ){
	if( position < 0 || position > pMessage->GetDataLength() ){
		DETHROW( deeOutOfBoundary );
	}
	pPosition = pMessage->GetDataLength() - position;
}



// Reading
////////////

void deNetworkMessageReader::Read( void *buffer, int size ){
	if( ! buffer ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPosition + size > pMessage->GetDataLength() ){
		DETHROW( deeInvalidParam );
	}
	
	memcpy( buffer, pMessage->GetBuffer() + pPosition, size );
	pPosition += size;
}

decBaseFileReader::Ref deNetworkMessageReader::Duplicate(){
	return decBaseFileReader::Ref::New( new deNetworkMessageReader( pMessage ) );
}
