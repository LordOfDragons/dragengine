/* 
 * Drag[en]gine Basic Network Module
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debnMessage.h"
#include "dragengine/resources/network/deNetworkMessage.h"
#include "dragengine/common/exceptions.h"



// Class debnMessage
//////////////////////

// Constructor, destructor
////////////////////////////

debnMessage::debnMessage(){
	pMessage = NULL;
	pNumber = -1;
	pState = emsPending;
	pType = 0;
	pSecSinceSend = 0.0f;
	
	try{
		pMessage = new deNetworkMessage;
		if( ! pMessage ) DETHROW( deeOutOfMemory );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debnMessage::debnMessage( deNetworkMessage *message ){
	if( ! message ) DETHROW( deeInvalidParam );
	
	pMessage = NULL;
	pNumber = -1;
	pState = emsPending;
	pType = 0;
	pSecSinceSend = 0.0f;
	
	pMessage = message;
	message->AddReference();
}

debnMessage::~debnMessage(){
	pCleanUp();
}



// Management
///////////////

void debnMessage::SetNumber( int number ){
	pNumber = number;
}

void debnMessage::SetState( int state ){
	if( state < emsPending || state > emsDone ) DETHROW( deeInvalidParam );
	
	pState = state;
}

void debnMessage::SetType( int type ){
	pType = type;
}

void debnMessage::SetSecondsSinceSend( float seconds ){
	pSecSinceSend = seconds;
}

void debnMessage::IncreaseSecondsSinceSend( float seconds ){
	pSecSinceSend += seconds;
}



// Private Functions
//////////////////////

void debnMessage::pCleanUp(){
	if( pMessage ) pMessage->FreeReference();
}
