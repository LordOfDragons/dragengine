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

#include "debnSocket.h"
#include "debnServer.h"
#include "debnAddress.h"
#include "debnConnection.h"
#include "deNetworkBasic.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntSet.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/network/deServer.h>
#include <dragengine/resources/network/deConnection.h>
#include <dragengine/resources/network/deConnectionManager.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingServer.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingConnection.h>



// Class debnServer
/////////////////////

// Constructor, destructor
////////////////////////////

debnServer::debnServer(deNetworkBasic *netBasic, deServer *server){
	if(!netBasic || !server){
		DETHROW(deeInvalidParam);
	}
	
	pNetBasic = netBasic;
	pServer = server;
	
	pSocket = NULL;
	pListening = false;
	
	pPreviousServer = NULL;
	pNextServer = NULL;
	pIsRegistered = false;
}

debnServer::~debnServer(){
	pCleanUp();
}



// Management
///////////////

void debnServer::ProcessConnectionRequest(debnAddress &address, decBaseFileReader &reader){
	// reject connection if not listening or there is no script module peer
	deBaseScriptingServer * const scrSvr = pServer->GetPeerScripting();
	if(!pListening || !scrSvr){
		decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
		sendWriter.SetPosition(0);
		pNetBasic->GetSharedSendDatagram()->Clear();
		sendWriter.WriteByte((uint8_t)eccConnectionAck);
		sendWriter.WriteByte((uint8_t)ecaRejected);
		pSocket->SendDatagram(*pNetBasic->GetSharedSendDatagram(), address);
		return;
	}
	
	// find best protocol to speak
	decIntSet clientProtocols;
	const int clientProtocolCount = reader.ReadUShort();
	int i;
	for(i=0; i<clientProtocolCount; i++){
		clientProtocols.Add(reader.ReadUShort());
	}
	
	if(!clientProtocols.Has(epDENetworkProtocol)){
		decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
		sendWriter.SetPosition(0);
		pNetBasic->GetSharedSendDatagram()->Clear();
		sendWriter.WriteByte((uint8_t)eccConnectionAck);
		sendWriter.WriteByte((uint8_t)ecaNoCommonProtocol);
		pSocket->SendDatagram(*pNetBasic->GetSharedSendDatagram(), address);
		return;
	}
	
	eProtocols protocol = epDENetworkProtocol;
	
	// create connection 
	deConnection::Ref connection(deConnection::Ref::New(
		 pNetBasic->GetGameEngine()->GetConnectionManager()->CreateConnection()));
	((debnConnection*)connection->GetPeerNetwork())->AcceptConnection(pSocket, address, protocol);
	
	// send back result
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition(0);
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte((uint8_t)eccConnectionAck);
	sendWriter.WriteByte((uint8_t)ecaAccepted);
	sendWriter.WriteUShort(protocol);
	pSocket->SendDatagram(*pNetBasic->GetSharedSendDatagram(), address);
	
	// notify script about client connected
	scrSvr->ClientConnected(connection);
}

bool debnServer::ListenOn(const char *address){
	if(pListening){
		pNetBasic->LogWarn("Already listening.");
		return false;
	}
	
	decString useAddress(address);
	
	if(useAddress == "*"){
		decStringList publicAddresses;
		pNetBasic->FindPublicAddresses(publicAddresses);
		
		if(publicAddresses.GetCount() > 0){
			useAddress = publicAddresses.GetAt(0);
			
		}else{
			pNetBasic->LogWarn("debnServer.ListenOn: No public address found. Using localhost");
			useAddress = "localhost";
		}
	}
	
	pNetBasic->LogInfoFormat("debnServer.ListenOn: Listening on '%s'", useAddress.GetString());
	
	try{
		pSocket.TakeOver(new debnSocket(*pNetBasic));
		
		pSocket->GetAddress().SetFromString(useAddress);
		pSocket->Bind();
		
		pListening = true;
		
		pNetBasic->RegisterServer(this);
		
	}catch(const deException &e){
			pSocket = nullptr;
		}
		pNetBasic->LogException(e);
		return false;
	}
	
	return true;
}

void debnServer::StopListening(){
	if(!pListening){
		return;
	}
	
	if(pNetBasic){
		if(pSocket){
			pNetBasic->CloseConnections(pSocket);
		}
		
		pNetBasic->UnregisterServer(this);
	}
	
	pSocket = NULL;
	
	pListening = false;
}



// Linked List
////////////////

void debnServer::SetPreviousServer(debnServer *server){
	pPreviousServer = server;
}

void debnServer::SetNextServer(debnServer *server){
	pNextServer = server;
}

void debnServer::SetIsRegistered(bool isRegistered){
	pIsRegistered = isRegistered;
}



// Private Functions
//////////////////////

void debnServer::pCleanUp(){
	StopListening();
}
