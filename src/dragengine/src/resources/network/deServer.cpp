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

#include "deServer.h"
#include "deServerManager.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/network/deBaseNetworkServer.h"
#include "../../systems/modules/scripting/deBaseScriptingServer.h"



// Class deServer
///////////////////

// Constructor, destructor
////////////////////////////

deServer::deServer( deServerManager *manager ) :
deResource( manager ),
pPeerNetwork( NULL ),
pPeerScripting( NULL ){
}

deServer::~deServer(){
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
	if( pPeerScripting ){
		delete pPeerScripting;
	}
}



// Management
///////////////

void deServer::SetAddress( const char *address ){
	pAddress = address;
}

bool deServer::ListenOn( const char *address ){
	if( ! address ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerNetwork ){
		return pPeerNetwork->ListenOn( address );
	}
	
	return false;
}

void deServer::StopListening(){
	if( pPeerNetwork ){
		pPeerNetwork->StopListening();
	}
}



// System Peers
/////////////////

void deServer::SetPeerNetwork( deBaseNetworkServer *peer ){
	if( peer == pPeerNetwork ){
		return;
	}
	
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
	pPeerNetwork = peer;
}

void deServer::SetPeerScripting( deBaseScriptingServer *peer ){
	if( peer == pPeerScripting ){
		return;
	}
	
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	pPeerScripting = peer;
}
