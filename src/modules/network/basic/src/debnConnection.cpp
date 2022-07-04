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
#include "debnAddress.h"
#include "debnConnection.h"
#include "deNetworkBasic.h"
#include "states/debnState.h"
#include "states/debnStateLink.h"
#include "states/debnStateLinkList.h"
#include "states/debnStateLinkManager.h"
#include "messages/debnMessage.h"
#include "messages/debnMessageManager.h"
#include "visitors/debnVisitorValueDebug.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/network/deConnection.h>
#include <dragengine/resources/network/deNetworkState.h>
#include <dragengine/resources/network/deNetworkStateReference.h>
#include <dragengine/resources/network/deNetworkStateManager.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageReference.h>
#include <dragengine/resources/network/deNetworkMessageReader.h>
#include <dragengine/resources/network/deNetworkMessageWriter.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingConnection.h>



// Class debnConnection
/////////////////////////

// Constructor, destructor
////////////////////////////

debnConnection::debnConnection( deNetworkBasic *netBasic, deConnection *connection ){
	if( ! netBasic || ! connection ) DETHROW( deeInvalidParam );
	
	pNetBasic = netBasic;
	pConnection = connection;
	
	pSocket = NULL;
	pRemoteAddress = NULL;
	pConnectionState = ecsDisconnected;
	pIdentifier = -1;
	
	pStateLinks = NULL;
	pModifiedStateLinks = NULL;
	
	pReliableMessagesSend = NULL;
	pReliableMessagesRecv = NULL;
	pReliableNumberSend = 0;
	pReliableNumberRecv = 0;
	pReliableWindowSize = 10;
	
	pPreviousConnection = NULL;
	pNextConnection = NULL;
	pIsRegistered = false;
	
	try{
		pRemoteAddress = new debnAddress;
		pStateLinks = new debnStateLinkManager;
		pModifiedStateLinks = new debnStateLinkList;
		pReliableMessagesSend = new debnMessageManager;
		pReliableMessagesRecv = new debnMessageManager;
		
		// register us
		netBasic->RegisterConnection( this );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debnConnection::~debnConnection(){
	pCleanUp();
}



// Management
///////////////

void debnConnection::SetIdentifier( int identifier ){
	if( identifier < -1 ) DETHROW( deeInvalidParam );
	
	pIdentifier = identifier;
}

void debnConnection::Process( float elapsedTime ){
	if( pConnectionState == ecsConnected ){
		pUpdateTimeouts( elapsedTime );
		pUpdateStates();
	}
}

void debnConnection::InvalidateState( debnState *state ){
	debnStateLink *stateLink, *nextLink;
	
	stateLink = pStateLinks->GetHeadLink();
	while( stateLink ){
		nextLink = stateLink->GetNextLink();
		
		if( state == stateLink->GetState() ){
			if( state->GetLinks()->HasLink( stateLink ) ){
				state->GetLinks()->RemoveLink( stateLink );
			}
			
			pStateLinks->RemoveLink( stateLink );
		}
		
		stateLink = nextLink;
	}
}

bool debnConnection::Matches( const debnSocket *bnSocket, const debnAddress *address ) const{
	return bnSocket == pSocket && *address == *pRemoteAddress;
}



void debnConnection::AcceptConnection( debnSocket *bnSocket, debnAddress *address, eProtocols protocol ){
	if( ! bnSocket || ! address ) DETHROW( deeInvalidParam );
	
	pSocket = bnSocket;
	bnSocket->AddReference();
	
	*pRemoteAddress = *address;
	pConnection->SetRemoteAddress( address->ToString() );
	
	pConnectionState = ecsConnected;
	pProtocol = protocol;
}

void debnConnection::ProcessConnectionAck( decBaseFileReader &reader ){
	deBaseScriptingConnection *scrCon = pConnection->GetPeerScripting();
	
	if( pConnectionState == ecsConnecting ){
		const eConnectionAck code = ( eConnectionAck )reader.ReadByte();
		
		if( code == ecaAccepted ){
// 			pNetBasic->LogInfoFormat( "Connection accepted." );
			
			pProtocol = ( eProtocols )reader.ReadUShort();
			
			pConnectionState = ecsConnected;
			pConnection->SetConnected( true );
			
		}else{
// 			pNetBasic->LogInfo( "Connection rejected." );
			
			pConnectionState = ecsDisconnected;
			pConnection->SetConnected( false );
			
			if( scrCon ){
				scrCon->ConnectionClosed();
			}
		}
		
	}else{
		pNetBasic->LogInfo( "Invalid connection ack received." );
	}
}

void debnConnection::ProcessConnectionClose( decBaseFileReader &reader ){
	deBaseScriptingConnection *scrCon = pConnection->GetPeerScripting();
	
	if( pConnectionState == ecsConnected ){
// 		pNetBasic->LogInfo( "Closing connection upon request from remote side." );
		
		pDisconnect();
		
		// WARNING!
		// it is possible ( and very likely ) that the connection object
		// is freed during the callback. to avoid running into a segfault
		// the callback is placed after the cleanup. a better solution
		// would be to safeguard a pointer during this time.
		if( scrCon ) scrCon->ConnectionClosed();
	}
}

void debnConnection::ProcessMessage( decBaseFileReader &reader ){
	deBaseScriptingConnection *scrCon = pConnection->GetPeerScripting();
	
	const int length = reader.GetLength() - reader.GetPosition();
	
	deNetworkMessageReference message;
	message.TakeOver( new deNetworkMessage );
	message->SetDataLength( length );
	reader.Read( message->GetBuffer(), length );
	
	//pNetBasic->LogInfoFormat( "Message received with length %i.", length );
	
	if( scrCon ){
		scrCon->MessageReceived( message );
	}
}

void debnConnection::ProcessReliableMessage( decBaseFileReader &reader ){
	// we process nothing if not connected
	if( pConnectionState != ecsConnected ){
		pNetBasic->LogInfo( "Reliable message received although not connected." );
		return;
	}
	
	// read the infos about the message
	const int number = reader.ReadUShort();
	bool validNumber;
	
	// verify number
	if( number < pReliableNumberRecv ){
		validNumber = number < ( pReliableNumberRecv + pReliableWindowSize ) % 65535;
		
	}else{
		validNumber = number < pReliableNumberRecv + pReliableWindowSize;
	}
	if( ! validNumber ){
		pNetBasic->LogInfo( "Reliable message: invalid sequence number." );
		return;
	}
	
	// send ack
// 	pNetBasic->LogInfoFormat( "ProcessReliableMessage: send ack for %i", number );
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( ( uint8_t )eccReliableAck );
	sendWriter.WriteUShort( ( uint16_t )number );
	sendWriter.WriteByte( ( uint8_t )eraSuccess );
	
	pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
	
	// prepare
	//length = reader.GetDataLength() - reader.GetPosition();
	//pNetBasic->LogInfoFormat( "Reliable message: number %i with length %i.", number, length );
	
	// if the number is the next one expected send directly to the script
	if( number == pReliableNumberRecv ){
		// process message
		pProcessReliableMessage( number, reader );
		
		// bump up the number
		pReliableNumberRecv = ( pReliableNumberRecv + 1 ) % 65535;
		
		// check if the next message happens to be already in the queue
		pProcessQueuedMessages();
		
	// otherwise add the message to the queue
	}else{
		pAddReliableReceive( eccReliableMessage, number, reader );
	}
}

void debnConnection::ProcessReliableLinkState( decBaseFileReader &reader ){
	// we process nothing if not connected
	if( pConnectionState != ecsConnected ){
		pNetBasic->LogInfoFormat( "Link state: not connected." );
		return;
	}
	
	// read the infos about the message
	const int number = reader.ReadUShort();
	bool validNumber;
	
	// verify number
	if( number < pReliableNumberRecv ){
		validNumber = number < ( pReliableNumberRecv + pReliableWindowSize ) % 65535;
		
	}else{
		validNumber = number < pReliableNumberRecv + pReliableWindowSize;
	}
	if( ! validNumber ){
		pNetBasic->LogInfo( "Link state: invalid sequence number." );
		return;
	}
	
	// send ack
// 	pNetBasic->LogInfoFormat( "ProcessReliableLinkState: send ack for %i", number );
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( ( uint8_t )eccReliableAck );
	sendWriter.WriteUShort( ( uint16_t )number );
	sendWriter.WriteByte( ( uint8_t )eraSuccess );
	
	pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
	
	// prepare
	//length = reader.GetDataLength() - reader.GetPosition();
	
	// if the number is the next one expected send directly to the script
	if( number == pReliableNumberRecv ){
		// process the link
		pProcessLinkState( number, reader );
		
		// bump up the number
		pReliableNumberRecv = ( pReliableNumberRecv + 1 ) % 65535;
		
		// check if the next message happens to be already in the queue
		pProcessQueuedMessages();
		
	// otherwise add the message to the queue
	}else{
		pAddReliableReceive( eccReliableLinkState, number, reader );
	}
}

void debnConnection::ProcessReliableAck( decBaseFileReader &reader ){
	// we process nothing if not connected
	if( pConnectionState != ecsConnected ){
		pNetBasic->LogInfo( "Reliable ack: not connected." );
		return;
	}
	
	// read the infos about the message
	const int number = reader.ReadUShort();
	const int code = reader.ReadByte();
// 	pNetBasic->LogInfoFormat( "ProcessReliableAck: received ack for %i", number );
	
	// verify
	const int index = pReliableMessagesSend->IndexOfMessageWithNumber( number );
	if( index == -1 ){
		pNetBasic->LogInfo( "Reliable ack: no reliable transmission with this number waiting for an ack!" );
		return;
	}
	debnMessage * const bnMessage = pReliableMessagesSend->GetMessageAt( index );
	
	// if reliable transmission arrived succefull done the message
	if( code == eraSuccess ){
		// mark the message done
		bnMessage->SetState( debnMessage::emsDone );
		
		// remove all done messages up to the first pending one
		pRemoveSendReliablesDone();
		
	// otherwise resend
	}else{
		// reset timeout
		bnMessage->SetSecondsSinceSend( 0.0f );
		
		// resend message
		pSocket->SendDatagram( bnMessage->GetMessage(), pRemoteAddress );
	}
}

void debnConnection::ProcessLinkUp( decBaseFileReader &reader ){
	// we process nothing if not connected
	if( pConnectionState != ecsConnected ){
		pNetBasic->LogInfo( "Link up: not connected." );
		return;
	}
	
	// read the infos about the message
	const int identifier = reader.ReadUShort();
	
	// check if a link exists with this identifier
	debnStateLink * const stateLink = pStateLinks->GetLinkWithIdentifier( identifier );
	if( ! stateLink || stateLink->GetLinkState() != debnStateLink::elsListening ){
		pNetBasic->LogInfo( "Link up: link with this identifier does not exist or is not listening." );
		return;
	}
	
	// mark the link established
	stateLink->SetLinkState( debnStateLink::elsUp );
	
	// infos
// 	pNetBasic->LogInfoFormat( "Link up for link %i.", identifier );
}

void debnConnection::ProcessLinkDown( decBaseFileReader &reader ){
	// we process nothing if not connected
	if( pConnectionState != ecsConnected ){
		pNetBasic->LogInfo( "Link down: not connected!" );
		return;
	}
	
	// read the infos about the message
	const int identifier = reader.ReadUShort();
	
	// check if a link exists with this identifier
	debnStateLink * const stateLink = pStateLinks->GetLinkWithIdentifier( identifier );
	if( ! stateLink || stateLink->GetLinkState() != debnStateLink::elsListening ){
		pNetBasic->LogInfo( "Link down: link with this identifier does not exist or is not listening." );
		return;
	}
	
	// mark the link down ( TODO )
	stateLink->SetLinkState( debnStateLink::elsDown );
	
	// infos
// 	pNetBasic->LogInfoFormat( "Link down for link %i.", identifier );
}

void debnConnection::ProcessLinkUpdate( decBaseFileReader &reader ){
	if( pConnectionState != ecsConnected ){
		pNetBasic->LogInfo( "Link update: not connected." );
		return;
	}
	
	try{
		const int count = reader.ReadByte();
		int i;
		for( i=0; i<count; i++ ){
			const int identifier = reader.ReadUShort();
			
			debnStateLink * const stateLink = pStateLinks->GetLinkWithIdentifier( identifier );
			if( ! stateLink || stateLink->GetLinkState() != debnStateLink::elsUp ){
				pNetBasic->LogInfo( "Invalid link identifier!" );
				return;
			}
			
			if( stateLink->GetState() ){
				stateLink->GetState()->LinkReadValues( reader, *stateLink );
			}
		}
		
	}catch( const deException & ){
		pNetBasic->LogInfo( "Invalid data in the link update message!" );
		return;
	}
}



bool debnConnection::ConnectTo( const char *address ){
	// if we are already connected stop right here
	if( pSocket ) return false;
	
	pNetBasic->LogInfoFormat( "debnConnection.ConnectTo %s", address );
	
	// create connect socket
	pSocket = new debnSocket( pNetBasic );
	
	pSocket->GetAddress()->SetIPv4Any();
	pSocket->Bind();
	
	pConnection->SetLocalAddress( pSocket->GetAddress()->ToString() );
	
	// send connect request
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( eccConnectionRequest );
	
	sendWriter.WriteUShort( 1 );
	sendWriter.WriteUShort( epDENetworkProtocol );
	
	pRemoteAddress->SetIPv4FromString( address );
	pConnection->SetRemoteAddress( address );
	
	pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
	
	// switch to connecting state
	pConnectionState = ecsConnecting;
	
	// finished
	return true;
}

void debnConnection::Disconnect(){
	pNetBasic->LogInfo( "debnConnection.Disconnect." );
	
	if( pSocket ){
		// send close if connected
		if( pConnectionState == ecsConnected ){
			decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
			sendWriter.SetPosition( 0 );
			pNetBasic->GetSharedSendDatagram()->Clear();
			sendWriter.WriteByte( eccConnectionClose );
			
			pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
		}
		
		// clean up
		pDisconnect();
	}
}

void debnConnection::SendMessage( deNetworkMessage *message, int maxDelay ){
	if( ! message || message->GetDataLength() < 1 ) DETHROW( deeInvalidParam );
	
	// only if connected
	if( pConnectionState != ecsConnected ) return;
	
	// send message
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( eccMessage ); // command
	sendWriter.Write( message->GetBuffer(), message->GetDataLength() );
	
	pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
}

void debnConnection::SendReliableMessage( deNetworkMessage *message ){
	if( ! message || message->GetDataLength() < 1 ) DETHROW( deeInvalidParam );
	debnMessage *bnMessage = NULL;
	
	// only if connected
	if( pConnectionState != ecsConnected ) return;
	
	// add message
	try{
		// create message
		bnMessage = new debnMessage;
		bnMessage->SetType( eccReliableMessage );
		bnMessage->SetNumber( ( pReliableNumberSend + pReliableMessagesSend->GetMessageCount() ) % 65535 );
		bnMessage->SetState( debnMessage::emsPending );
		
		// build message
		decBaseFileWriterReference writer;
		writer.TakeOver( new deNetworkMessageWriter( bnMessage->GetMessage(), false ) );
		writer->WriteByte( eccReliableMessage );
		writer->WriteUShort( ( uint16_t )bnMessage->GetNumber() );
		writer->Write( message->GetBuffer(), message->GetDataLength() );
		
		// add
		pReliableMessagesSend->AddMessage( bnMessage );
		
	}catch( const deException & ){
		if( bnMessage ) delete bnMessage;
		throw;
	}
	
	// if the message fits into the window send it right now
	if( pReliableMessagesSend->GetMessageCount() <= pReliableWindowSize ){
		// send
		pSocket->SendDatagram( bnMessage->GetMessage(), pRemoteAddress );
		
		// mark the message send
		bnMessage->SetState( debnMessage::emsSend );
		bnMessage->SetSecondsSinceSend( 0.0f );
	}
}

void debnConnection::LinkState( deNetworkMessage *message, deNetworkState *state, bool readOnly ){
	if( ! message || message->GetDataLength() < 1 || ! state ) DETHROW( deeInvalidParam );
	debnState *bnState = ( debnState* )state->GetPeerNetwork();
	debnMessage *bnMessage = NULL;
	
	// only if connected
	if( pConnectionState != ecsConnected ) return;
	
	// state is now bound to us no matter what happens
	//bnState->SetConnection( this );
	
	// check if a link exists with this state already that is not broken
	debnStateLink *stateLink = pStateLinks->GetLinkWithNetworkState( bnState );
	if( stateLink && stateLink->GetLinkState() != debnStateLink::elsDown ){
		pNetBasic->LogInfo( "There exists already a link to this state." );
		return;
	}
	
	// create the link if not existing, assign it a new identifier and add it
	if( ! stateLink ){
		try{
			stateLink = new debnStateLink( bnState, *this );
			
			stateLink->SetIdentifier( pStateLinks->GetFreeIdentifier() );
			if( stateLink->GetIdentifier() == -1 ) DETHROW( deeInvalidParam );
			
			pStateLinks->AddLink( stateLink );
			
		}catch( const deException & ){
			if( stateLink ) delete stateLink;
			throw;
		}
		
		bnState->GetLinks()->AddLink( stateLink );
	}
	
	//pNetBasic->LogInfoFormat( "Linking state %p using link %i", state, stateLink->GetIdentifier() );
	
	// add message
	try{
		// create message
		bnMessage = new debnMessage;
		bnMessage->SetType( eccReliableLinkState );
		bnMessage->SetNumber( ( pReliableNumberSend + pReliableMessagesSend->GetMessageCount() ) % 65535 );
		bnMessage->SetState( debnMessage::emsPending );
		
		// build message
		decBaseFileWriterReference writer;
		writer.TakeOver( new deNetworkMessageWriter( bnMessage->GetMessage(), false ) );
		writer->WriteByte( eccReliableLinkState );
		writer->WriteUShort( ( uint16_t )bnMessage->GetNumber() );
		writer->WriteUShort( ( uint16_t )stateLink->GetIdentifier() );
		writer->WriteByte( readOnly ? 1 : 0 ); // flags: readOnly=0x1
		
		writer->WriteUShort( message->GetDataLength() );
		writer->Write( message->GetBuffer(), message->GetDataLength() );
		
		bnState->LinkWriteValuesWithVerify( writer );
		
		// add
		pReliableMessagesSend->AddMessage( bnMessage );
		
	}catch( const deException & ){
		if( bnMessage ) delete bnMessage;
		throw;
	}
	
	// if the message fits into the window send it right now
	if( pReliableMessagesSend->GetMessageCount() <= pReliableWindowSize ){
		// send
		pSocket->SendDatagram( bnMessage->GetMessage(), pRemoteAddress );
		
		// mark the message send
		bnMessage->SetState( debnMessage::emsSend );
		bnMessage->SetSecondsSinceSend( 0.0f );
	}
	
	// switch the link to the listening state
	stateLink->SetLinkState( debnStateLink::elsListening );
}



// Linked List
////////////////

void debnConnection::SetPreviousConnection( debnConnection *connection ){
	pPreviousConnection = connection;
}

void debnConnection::SetNextConnection( debnConnection *connection ){
	pNextConnection = connection;
}

void debnConnection::SetIsRegistered( bool isRegistered ){
	pIsRegistered = isRegistered;
}



// Private Functions
//////////////////////

void debnConnection::pCleanUp(){
	if( pNetBasic ) pNetBasic->UnregisterConnection( this );
	
	if( pStateLinks ){
		delete pStateLinks;
	}
	if( pModifiedStateLinks ){
		delete pModifiedStateLinks;
	}
	if( pSocket ){
		pSocket->FreeReference();
	}
	if( pRemoteAddress ){
		delete pRemoteAddress;
	}
	
	if( pReliableMessagesRecv ){
		delete pReliableMessagesRecv;
	}
	if( pReliableMessagesSend ){
		delete pReliableMessagesSend;
	}
}

void debnConnection::pDisconnect(){
	debnStateLink *stateLink;
	
	// clean up linked states
	pModifiedStateLinks->RemoveAllLinks();
	
	stateLink = pStateLinks->GetHeadLink();
	while( stateLink ){
		if( stateLink->GetState() ){
			stateLink->GetState()->GetLinks()->RemoveLinkIfExisting( stateLink );
		}
		stateLink = stateLink->GetNextLink();
	}
	pStateLinks->RemoveAllLinks();
	
	// clean up reliables
	pReliableMessagesRecv->RemoveAllMessages();
	pReliableMessagesSend->RemoveAllMessages();
	
	// free the socket
	pConnectionState = ecsDisconnected;
	if( pSocket ){
		pSocket->FreeReference();
		pSocket = NULL;
	}
	
	// switch to disconnected state
	pConnectionState = ecsDisconnected;
	pConnection->SetConnected( false );
}

void debnConnection::pUpdateStates(){
	int linkCount = pModifiedStateLinks->GetLinkCount();
	if( linkCount == 0 ){
		return;
	}
	
	int i, changedCount = 0;
	for( i=0; i<linkCount; i++ ){
		const debnStateLink &stateLink = *pModifiedStateLinks->GetLinkAt( i );
		if( stateLink.GetLinkState() == debnStateLink::elsUp && stateLink.GetChanged() ){
			changedCount++;
		}
	}
	if( changedCount == 0 ){
		return;
	}
	if( changedCount > 255 ){
		changedCount = 255;
	}
	
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( ( uint8_t )eccLinkUpdate );
	sendWriter.WriteByte( ( uint8_t )changedCount );
	
	for( i=0; i<linkCount; i++ ){
		debnStateLink &stateLink = *pModifiedStateLinks->GetLinkAt( i );
		if( stateLink.GetLinkState() != debnStateLink::elsUp || ! stateLink.GetChanged() ){
			continue;
		}
		
		sendWriter.WriteUShort( ( uint16_t )stateLink.GetIdentifier() );
		
		if( stateLink.GetState() ){
			stateLink.GetState()->LinkWriteValues( sendWriter, stateLink );
		}
		
		pModifiedStateLinks->RemoveLink( &stateLink );
		linkCount--;
		i--;
		
		changedCount--;
		if( changedCount == 0 ){
			break;
		}
	}
	
	pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
}

void debnConnection::pUpdateTimeouts( float elapsedTime ){
	int i, count = pReliableMessagesSend->GetMessageCount();
	debnMessage *bnMessage;
	float timeout = 3.0f;
	
	// increase the timeouts on all send packages
	for( i=0; i<count; i++ ){
		bnMessage = pReliableMessagesSend->GetMessageAt( i );
		
		// we are only interested in send packages
		if( bnMessage->GetState() == debnMessage::emsSend ){
			// increase the timeout if the package is send
			bnMessage->IncreaseSecondsSinceSend( elapsedTime );
			
			// if the elapsed time reaches the timeout send the message again
			if( bnMessage->GetSecondsSinceSend() > timeout ){
				// send the message
				pNetBasic->LogInfoFormat( "pUpdateTimeouts: resend message %i", bnMessage->GetNumber() );
				pSocket->SendDatagram( bnMessage->GetMessage(), pRemoteAddress );
				
				// reset the timeout
				bnMessage->SetSecondsSinceSend( 0.0f );
				
				// TODO: retry limit
			}
		}
	}
}

void debnConnection::pProcessQueuedMessages(){
	debnMessage *bnMessage;
	int index, type;
	
	// check if the next message happens to be already in the queue
	index = pReliableMessagesRecv->IndexOfMessageWithNumber( pReliableNumberRecv );
	while( index != -1 ){
		bnMessage = pReliableMessagesRecv->GetMessageAt( index );
		
		// process the message
		type = bnMessage->GetType();
		if( type == eccReliableMessage ){
			decBaseFileReaderReference reader;
			reader.TakeOver( new deNetworkMessageReader( bnMessage->GetMessage() ) );
			pProcessReliableMessage( pReliableNumberRecv, reader );
			
		}else if( type == eccReliableLinkState ){
			decBaseFileReaderReference reader;
			reader.TakeOver( new deNetworkMessageReader( bnMessage->GetMessage() ) );
			pProcessLinkState( pReliableNumberRecv, reader );
		}
		
		// remove the message from the queue
		pReliableMessagesRecv->RemoveMessageAt( index );
		
		// bump up the number
		pReliableNumberRecv = ( pReliableNumberRecv + 1 ) % 65535;
		
		// see if the next message happens to be in the queue
		index = pReliableMessagesRecv->IndexOfMessageWithNumber( pReliableNumberRecv );
	}
}

void debnConnection::pProcessReliableMessage( int number, decBaseFileReader &reader ){
	// send message to the script
	const int position = reader.GetPosition();
	const int length = reader.GetLength() - position;
	
	deNetworkMessageReference message;
	message.TakeOver( new deNetworkMessage );
	message->SetDataLength( length );
	reader.Read( message->GetBuffer(), length );
	
	deBaseScriptingConnection * const scrCon = pConnection->GetPeerScripting();
	if( scrCon ){
		scrCon->MessageReceived( message );
	}
}

void debnConnection::pProcessLinkState( int number, decBaseFileReader &reader ){
	deBaseScriptingConnection *scrCon = pConnection->GetPeerScripting();
	// read the infos about the message
	const int identifier = reader.ReadUShort();
	const bool readOnly = reader.ReadByte() == 1; // flags: 0x1=readOnly
	//pNetBasic->LogInfoFormat( "Link state for link %i readOnly=%d.", identifier, readOnly );
	
	// check if a link exists with this identifier already that is not broken
	debnStateLink *stateLink = pStateLinks->GetLinkWithIdentifier( identifier );
	if( stateLink && stateLink->GetLinkState() != debnStateLink::elsDown ){
		pNetBasic->LogInfo( "Link state: link with this identifier already exists." );
		return;
	}
	
	// create linked network state
	deNetworkMessageReference message;
	message.TakeOver( new deNetworkMessage );
	message->SetDataLength( reader.ReadUShort() );
	reader.Read( message->GetBuffer(), message->GetDataLength() );
	
	deNetworkStateReference state;
	if( scrCon ){
		state.TakeOver( pNetBasic->GetGameEngine()->GetNetworkStateManager()->CreateState( readOnly ) );
		if( ! scrCon->LinkState( state, message ) ){
			state = NULL;
		}
	}
	
	// if there is a state try to link
	debnState *bnState = NULL;
	if( state ){
		bnState = ( debnState* )state->GetPeerNetwork();
	}
	
	eCommandCodes code = eccLinkDown;
	if( bnState ){
		if( bnState->LinkReadAndVerifyAllValues( reader ) ){
			// create the link if not existing, assign it a new identifier and add it
			if( ! stateLink ){
				try{
					stateLink = new debnStateLink( bnState, *this );
					stateLink->SetIdentifier( identifier );
					pStateLinks->AddLink( stateLink );
					
				}catch( const deException & ){
					if( stateLink ){
						delete stateLink;
					}
					throw;
				}
				
				bnState->GetLinks()->AddLink( stateLink );
			}
			
			// mark the link as established
			stateLink->SetLinkState( debnStateLink::elsUp );
			
			// done with it
			//pNetBasic->LogInfo( "Link state succeeded." );
			code = eccLinkUp;
			
		}else{
			pNetBasic->LogInfo( "Link state does not match the state provided." );
			code = eccLinkDown;
		}
		
	}else{
		pNetBasic->LogInfo( "No link state provided." );
		code = eccLinkDown;
	}
	
	// send link up 
	decBaseFileWriter &sendWriter = pNetBasic->GetSharedSendDatagramWriter();
	sendWriter.SetPosition( 0 );
	pNetBasic->GetSharedSendDatagram()->Clear();
	sendWriter.WriteByte( ( uint8_t )code );
	sendWriter.WriteUShort( ( uint16_t )identifier );
	
	pSocket->SendDatagram( pNetBasic->GetSharedSendDatagram(), pRemoteAddress );
}

void debnConnection::pAddReliableReceive( int type, int number, decBaseFileReader &reader ){
	debnMessage *bnMessage = NULL;
	
	// length of the message
	const int length = reader.GetLength() - reader.GetPosition();
	
	// add message
	try{
		bnMessage = new debnMessage;
		
		deNetworkMessage * const localMessage = bnMessage->GetMessage();
		localMessage->SetDataLength( length );
		reader.Read( localMessage->GetBuffer(), length );
		
		bnMessage->SetType( type );
		bnMessage->SetNumber( number );
		bnMessage->SetState( debnMessage::emsDone );
		
		pReliableMessagesRecv->AddMessage( bnMessage );
		
	}catch( const deException & ){
		if( bnMessage ) delete bnMessage;
		throw;
	}
}

void debnConnection::pRemoveSendReliablesDone(){
	bool removedSome = false;
	
	// remove send messages if done
	while( pReliableMessagesSend->GetMessageCount() > 0 ){
		if( pReliableMessagesSend->GetMessageAt( 0 )->GetState() == debnMessage::emsDone ){
			// remove the message
			pReliableMessagesSend->RemoveMessageAt( 0 );
			
			// bump up the next number
			pReliableNumberSend = ( pReliableNumberSend + 1 ) % 65535;
			
			// we removed something
			removedSome = true;
			
		}else{
			break;
		}
	}
	
	// if we removed something there is the chance for new messages to
	// move inside the window
	if( removedSome ) pSendPendingReliables();
}

void debnConnection::pSendPendingReliables(){
	int i, count = pReliableMessagesSend->GetMessageCount();
	debnMessage *bnMessage;
	
	// check if there are messages inside the window that can be send
	for( i=0; i<count; i++ ){
		// if we hit the window size exit
		if( i == pReliableWindowSize ) break;
		
		// if the message is pending send it
		bnMessage = pReliableMessagesSend->GetMessageAt( i );
		if( bnMessage->GetState() == debnMessage::emsPending ){
			// send
			pSocket->SendDatagram( bnMessage->GetMessage(), pRemoteAddress );
			
			// mark the message send
			bnMessage->SetState( debnMessage::emsSend );
			bnMessage->SetSecondsSinceSend( 0.0f );
		}
	}
}
