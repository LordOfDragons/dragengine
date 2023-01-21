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
#include <dragengine/resources/network/deConnectionReference.h>
#include <dragengine/resources/network/deConnectionManager.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingServer.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingConnection.h>



// Class debnServer
/////////////////////

// Constructor, destructor
////////////////////////////

debnServer::debnServer( deNetworkBasic *netBasic, deServer *server ){
	if( ! netBasic || ! server ){
		DETHROW( deeInvalidParam );
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

void debnServer::ProcessConnectionRequest( debnAddress &address, decBaseFileReader &reader ){
	// reject connection if not listening or there is no script module peer
	deBaseScriptingServer * const scrSvr = pServer->GetPeerScripting();
	if( ! pListening || ! scrSvr ){
		decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
		sendWriter.SetPosition( 0 );
		pNetBasic->GetSharedSendDatagram()->Clear();
		sendWriter.WriteByte( ( uint8_t )eccConnectionAck );
		sendWriter.WriteByte( ( uint8_t )ecaRejected );
		pSocket->SendDatagram( *pNetBasic->GetSharedSendDatagram(), address );
		return;
	}
	
	// find best protocol to speak
	decIntSet clientProtocols;
	const int clientProtocolCount = reader.ReadUShort();
	int i;
	for( i=0; i<clientProtocolCount; i++ ){
		clientProtocols.Add( reader.ReadUShort() );
	}
	
	if( ! clientProtocols.Has( epDENetworkProtocol ) ){
		decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
		sendWriter.SetPosition( 0 );
		pNetBasic->GetSharedSendDatagram()->Clear();
		sendWriter.WriteByte( ( uint8_t )eccConnectionAck );
		sendWriter.WriteByte( ( uint8_t )ecaNoCommonProtocol );
		pSocket->SendDatagram( *pNetBasic->GetSharedSendDatagram(), address );
		return;
	}
	
	eProtocols protocol = epDENetworkProtocol;
	
	// create connection 
	deConnectionReference connection;
	connection.TakeOver( pNetBasic->GetGameEngine()->GetConnectionManager()->CreateConnection() );
	( ( debnConnection* )connection->GetPeerNetwork() )->AcceptConnection( pSocket, address, protocol );
	
	// send back result
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( ( uint8_t )eccConnectionAck );
	sendWriter.WriteByte( ( uint8_t )ecaAccepted );
	sendWriter.WriteUShort( protocol );
	pSocket->SendDatagram( *pNetBasic->GetSharedSendDatagram(), address );
	
	// notify script about client connected
	scrSvr->ClientConnected( connection );
}

bool debnServer::ListenOn( const char *address ){
	if( pListening ){
		pNetBasic->LogWarn( "Already listening." );
		return false;
	}
	
	decString useAddress( address );
	
	if( useAddress == "*" ){
		decStringList publicAddresses;
		pNetBasic->FindPublicAddresses( publicAddresses );
		
		if( publicAddresses.GetCount() > 0 ){
			useAddress = publicAddresses.GetAt( 0 );
			
		}else{
			pNetBasic->LogWarn( "debnServer.ListenOn: No public address found. Using localhost" );
			useAddress = "localhost";
		}
	}
	
	pNetBasic->LogInfoFormat( "debnServer.ListenOn: Listening on '%s'", useAddress.GetString() );
	
	try{
		pSocket = new debnSocket( *pNetBasic );
		
		pSocket->GetAddress().SetFromString( useAddress );
		pSocket->Bind();
		
		pListening = true;
		
		pNetBasic->RegisterServer( this );
		
	}catch( const deException &e ){
		if( pSocket ){
			pSocket->FreeReference();
			pSocket = nullptr;
		}
		pNetBasic->LogException( e );
		return false;
	}
	
	return true;
}

void debnServer::StopListening(){
	if( ! pListening ){
		return;
	}
	
	if( pNetBasic ){
		if( pSocket ){
			pNetBasic->CloseConnections( pSocket );
		}
		
		pNetBasic->UnregisterServer( this );
	}
	
	pSocket->FreeReference();
	pSocket = NULL;
	
	pListening = false;
}



// Linked List
////////////////

void debnServer::SetPreviousServer( debnServer *server ){
	pPreviousServer = server;
}

void debnServer::SetNextServer( debnServer *server ){
	pNextServer = server;
}

void debnServer::SetIsRegistered( bool isRegistered ){
	pIsRegistered = isRegistered;
}



// Private Functions
//////////////////////

void debnServer::pCleanUp(){
	StopListening();
}
