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

deConnection::deConnection(deConnectionManager *manager) :
deResource(manager),
pConnected(false),
pPeerNetwork(NULL),
pPeerScripting(NULL){
}

deConnection::~deConnection(){
	if(pPeerNetwork){
		delete pPeerNetwork;
	}
	if(pPeerScripting){
		delete pPeerScripting;
	}
}



// Management
///////////////

void deConnection::SetLocalAddress(const char *address){
	pLocalAddress = address;
}

void deConnection::SetRemoteAddress(const char *address){
	pRemoteAddress = address;
}

void deConnection::SetConnected(bool connected){
	pConnected = connected;
}

bool deConnection::ConnectTo(const char *address){
	if(!address){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerNetwork){
		return pPeerNetwork->ConnectTo(address);
	}
	
	return false;
}

void deConnection::Disconnect(){
	if(pPeerNetwork){
		pPeerNetwork->Disconnect();
	}
}
	
void deConnection::SendMessage(deNetworkMessage *message, int maxDelay){
	if(!message || message->GetDataLength() < 1 || maxDelay < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerNetwork){
		pPeerNetwork->SendMessage(message, maxDelay);
	}
}

void deConnection::SendReliableMessage(deNetworkMessage *message){
	if(!message || message->GetDataLength() < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerNetwork){
		pPeerNetwork->SendReliableMessage(message);
	}
}

void deConnection::LinkState(deNetworkMessage *message, deNetworkState *state, bool readOnly){
	if(!message || message->GetDataLength() < 1 || !state){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerNetwork){
		pPeerNetwork->LinkState(message, state, readOnly);
	}
}



// System Peers
/////////////////

void deConnection::SetPeerNetwork(deBaseNetworkConnection *peer){
	if(peer == pPeerNetwork){
		return;
	}
	
	if(pPeerNetwork){
		delete pPeerNetwork;
	}
	pPeerNetwork = peer;
}

void deConnection::SetPeerScripting(deBaseScriptingConnection *peer){
	if(peer == pPeerScripting){
		return;
	}
	
	if(pPeerScripting){
		delete pPeerScripting;
	}
	pPeerScripting = peer;
}
