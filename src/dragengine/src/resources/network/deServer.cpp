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
