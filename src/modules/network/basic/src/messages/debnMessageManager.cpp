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
#include "debnMessageManager.h"
#include "dragengine/resources/network/deNetworkMessage.h"
#include "dragengine/common/exceptions.h"



// Class debnMessageManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

debnMessageManager::debnMessageManager(){
	pMessages = NULL;
	pMessageCount = 0;
	pMessageSize = 0;
}

debnMessageManager::~debnMessageManager(){
	RemoveAllMessages();
	if( pMessages ) delete [] pMessages;
}



// Management
///////////////

debnMessage *debnMessageManager::GetMessageAt( int index ) const{
	if( index < 0 || index >= pMessageCount ) DETHROW( deeInvalidParam );
	
	return pMessages[ index ];
}

int debnMessageManager::IndexOfMessageWithNumber( int number ) const{
	int i;
	
	for( i=0; i<pMessageCount; i++ ){
		if( number == pMessages[ i ]->GetNumber() ) return i;
	}
	
	return -1;
}

int debnMessageManager::IndexOfMessage( debnMessage *message ) const{
	if( ! message ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pMessageCount; i++ ){
		if( message == pMessages[ i ] ) return i;
	}
	
	return -1;
}

bool debnMessageManager::HasMessage( debnMessage *message ) const{
	if( ! message ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pMessageCount; i++ ){
		if( message == pMessages[ i ] ) return true;
	}
	
	return false;
}

void debnMessageManager::AddMessage( debnMessage *message ){
	if( ! message ) DETHROW( deeInvalidParam );
	
	if( pMessageCount == pMessageSize ){
		int newSize = pMessageSize * 3 / 2 + 1;
		debnMessage **newArray = new debnMessage*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pMessages ){
			memcpy( newArray, pMessages, sizeof( debnMessage* ) * pMessageSize );
			delete [] pMessages;
		}
		pMessages = newArray;
		pMessageSize = newSize;
	}
	
	pMessages[ pMessageCount ] = message;
	pMessageCount++;
}

void debnMessageManager::RemoveMessage( debnMessage *message ){
	RemoveMessageAt( IndexOfMessage( message ) );
}

void debnMessageManager::RemoveMessageAt( int index ){
	if( index < 0 || index >= pMessageCount ) DETHROW( deeInvalidParam );
	debnMessage *message = pMessages[ index ];
	int i;
	
	for( i=index+1; i<pMessageCount; i++ ){
		pMessages[ i - 1 ] = pMessages[ i ];
	}
	pMessageCount--;
	
	delete message;
}

void debnMessageManager::RemoveAllMessages(){
	while( pMessageCount > 0 ){
		pMessageCount--;
		delete pMessages[ pMessageCount ];
	}
}
