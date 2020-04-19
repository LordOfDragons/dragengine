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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deConnection.h"
#include "deConnectionManager.h"
#include "deNetworkMessage.h"
#include "deNetworkState.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/network/deBaseNetworkConnection.h"
#include "../../systems/modules/scripting/deBaseScriptingConnection.h"



// Class deConnection
///////////////////////

// Constructor, destructor
////////////////////////////

deConnection::deConnection( deConnectionManager *manager ) :
deResource( manager ),
pConnected( false ),
pPeerNetwork( NULL ),
pPeerScripting( NULL ){
}

deConnection::~deConnection(){
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
	if( pPeerScripting ){
		delete pPeerScripting;
	}
}



// Management
///////////////

void deConnection::SetLocalAddress( const char *address ){
	pLocalAddress = address;
}

void deConnection::SetRemoteAddress( const char *address ){
	pRemoteAddress = address;
}

void deConnection::SetConnected( bool connected ){
	pConnected = connected;
}

bool deConnection::ConnectTo( const char *address ){
	if( ! address ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerNetwork ){
		return pPeerNetwork->ConnectTo( address );
	}
	
	return false;
}

void deConnection::Disconnect(){
	if( pPeerNetwork ){
		pPeerNetwork->Disconnect();
	}
}
	
void deConnection::SendMessage( deNetworkMessage *message, int maxDelay ){
	if( ! message || message->GetDataLength() < 1 || maxDelay < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerNetwork ){
		pPeerNetwork->SendMessage( message, maxDelay );
	}
}

void deConnection::SendReliableMessage( deNetworkMessage *message ){
	if( ! message || message->GetDataLength() < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerNetwork ){
		pPeerNetwork->SendReliableMessage( message );
	}
}

void deConnection::LinkState( deNetworkMessage *message, deNetworkState *state, bool readOnly ){
	if( ! message || message->GetDataLength() < 1 || ! state ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerNetwork ){
		pPeerNetwork->LinkState( message, state, readOnly );
	}
}



// System Peers
/////////////////

void deConnection::SetPeerNetwork( deBaseNetworkConnection *peer ){
	if( peer == pPeerNetwork ){
		return;
	}
	
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
	pPeerNetwork = peer;
}

void deConnection::SetPeerScripting( deBaseScriptingConnection *peer ){
	if( peer == pPeerScripting ){
		return;
	}
	
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	pPeerScripting = peer;
}
