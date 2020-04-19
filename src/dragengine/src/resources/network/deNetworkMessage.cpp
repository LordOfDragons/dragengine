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
#include "../../common/exceptions.h"



// Class deNetworkMessage
///////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkMessage::deNetworkMessage() :
pBuffer( NULL ),
pBufferSize( 0 ),
pDataLength( 0 ),
pTimeStamp( decDateTime::GetSystemTime() ){
}

deNetworkMessage::~deNetworkMessage(){
	if( pBuffer ){
		delete [] pBuffer;
	}
}



// Management
///////////////

void deNetworkMessage::SetDataLength( int dataLength ){
	if( dataLength < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( dataLength == pDataLength ){
		return;
	}
	
	if( dataLength > pBufferSize ){
		uint8_t * const newBuffer = new uint8_t[ dataLength ];
		if( pBuffer ){
			memcpy( newBuffer, pBuffer, pBufferSize );
			delete [] pBuffer;
		}
		pBuffer = newBuffer;
		pBufferSize = dataLength;
	}
	
	pDataLength = dataLength;
}

void deNetworkMessage::Clear(){
	pDataLength = 0;
}

void deNetworkMessage::SetTimeStamp( TIME_SYSTEM timeStamp ){
	pTimeStamp = timeStamp;
}
