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
#include <sys/time.h>

#ifdef OS_UNIX
#include <sys/select.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#ifdef OS_BEOS
#include <sys/sockio.h>
#endif

#ifdef OS_W32
#	include <dragengine/app/include_windows.h>
#	include <iphlpapi.h>
#endif

#include "debnServer.h"
#include "debnSocket.h"
#include "debnAddress.h"
#include "debnConnection.h"
#include "debnWorld.h"
#include "deNetworkBasic.h"
#include "states/debnState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageReader.h>
#include <dragengine/resources/network/deNetworkMessageWriter.h>
#include <dragengine/systems/modules/deModuleParameter.h>



// Export Definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *BasicNetworkCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Function
///////////////////

deBaseModule *BasicNetworkCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deNetworkBasic( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deNetworkBasic
/////////////////////////

//#define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS					timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(nb,what)			(nb).LogInfoFormat( "NetworkBasic Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#define DEBUG_PRINT_TIMER_TOTAL(nb,what)	(nb).LogInfoFormat( "NetworkBasic Timer-Total %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(nb,what)
#define DEBUG_PRINT_TIMER_TOTAL(nb,what)
#endif

// Constructor, destructor
////////////////////////////

deNetworkBasic::deNetworkBasic( deLoadableModule &loadableModule ) :
deBaseNetworkModule( loadableModule ){
	pHeadConnection = NULL;
	pTailConnection = NULL;
	pHeadServer = NULL;
	pTailServer = NULL;
	pHeadSocket = NULL;
	pTailSocket = NULL;
	
	pDatagram = NULL;
	pAddressReceive = NULL;
	
	//pMessagesSend = NULL;
	//pMessagesReceive = NULL;
	
	//pMessage = NULL;
}

deNetworkBasic::~deNetworkBasic(){
	CleanUp();
}



// Management
///////////////

bool deNetworkBasic::Init(){
	try{
		// create shared datagram
		pDatagram = new deNetworkMessage;
		if( ! pDatagram ) DETHROW( deeOutOfMemory );
		pDatagram->SetDataLength( 1024 );
		
		// create receive address
		pAddressReceive = new debnAddress;
		if( ! pAddressReceive ) DETHROW( deeOutOfMemory );
		
		pSharedSendDatagram.TakeOver( new deNetworkMessage );
		pSharedSendDatagram->SetDataLength( 50 );
		pSharedSendDatagramWriter.TakeOver( new deNetworkMessageWriter( pSharedSendDatagram, false ) );
		
		// create send and receive message queues
		//pMessagesSend = new debnMessageQueue;
		//if( ! pMessagesSend ) DETHROW( deeOutOfMemory );
		
		//pMessagesReceive = new debnMessageQueue;
		//if( ! pMessagesReceive ) DETHROW( deeOutOfMemory );
		
		//pMessage = new deNetworkMessage;
		//if( ! pMessage ) DETHROW( deeOutOfMemory );
		
	}catch( const deException & ){
		CleanUp();
		return false;
	}
	
	return true;
}

void deNetworkBasic::CleanUp(){
	/*if( pMessage ){
		delete pMessage;
		pMessage = NULL;
	}
	if( pMessagesSend ){
		delete pMessagesSend;
		pMessagesSend = NULL;
	}
	if( pMessagesReceive ){
		delete pMessagesReceive;
		pMessagesReceive = NULL;
	}*/
	
	if( pAddressReceive ){
		delete pAddressReceive;
		pAddressReceive = NULL;
	}
	
	if( pDatagram ){
		pDatagram->FreeReference();
		pDatagram = NULL;
	}
	
	// ensure all linked lists are NULL
	/*
	pHeadConnection = NULL;
	pTailConnection = NULL;
	pHeadServer = NULL;
	pTailServer = NULL;
	pHeadSocket = NULL;
	pTailSocket = NULL;
	*/
}

void deNetworkBasic::ProcessNetwork(){
DEBUG_RESET_TIMERS;
	const float elapsedTime = GetGameEngine()->GetElapsedTime();
	
	// process all messages destined to go out
	pProcessConnections( elapsedTime );
DEBUG_PRINT_TIMER( *this, "Process Connections" );
	
	// sleep a little bit to avoid hogging the cpu
	/*
	struct timeval timout;
	timout.tv_sec = 0;
	timout.tv_usec = 1000; // 1ms
	select( 0, NULL, NULL, NULL, &timout );
	*/
DEBUG_PRINT_TIMER( *this, "Sleep" );
	
	// check on incoming messages
	pReceiveDatagrams();
DEBUG_PRINT_TIMER( *this, "Receive Datagrams" );
DEBUG_PRINT_TIMER_TOTAL( *this, "Process Network" );
}



// Peer Management
////////////////////

void deNetworkBasic::RegisterConnection( debnConnection *connection ){
	if( ! connection || connection->GetIsRegistered() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTailConnection ){
		pTailConnection->SetNextConnection( connection );
		connection->SetPreviousConnection( pTailConnection );
		pTailConnection = connection;
		
	}else{
		pHeadConnection = connection;
		pTailConnection = connection;
	}
	
	connection->SetIsRegistered( true );
}

void deNetworkBasic::UnregisterConnection( debnConnection *connection ){
	if( ! connection || ! connection->GetIsRegistered() ){
		DETHROW( deeInvalidParam );
	}
	
	debnConnection * const previous = connection->GetPreviousConnection();
	debnConnection * const next = connection->GetNextConnection();
	
	if( previous ){
		previous->SetNextConnection( next );
		
	}else{
		pHeadConnection = next;
	}
	
	if( next ){
		next->SetPreviousConnection( previous );
		
	}else{
		pTailConnection = previous;
	}
	
	connection->SetIsRegistered( false );
	connection->SetNextConnection( NULL );
	connection->SetPreviousConnection( NULL );
}

void deNetworkBasic::RegisterServer( debnServer *server ){
	if( ! server || server->GetIsRegistered() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTailServer ){
		pTailServer->SetNextServer( server );
		server->SetPreviousServer( pTailServer );
		pTailServer = server;
		
	}else{
		pHeadServer = server;
		pTailServer = server;
	}
	
	server->SetIsRegistered( true );
}

void deNetworkBasic::UnregisterServer( debnServer *server ){
	if( ! server || ! server->GetIsRegistered() ){
		DETHROW( deeInvalidParam );
	}
	
	debnServer * const previous = server->GetPreviousServer();
	debnServer * const next = server->GetNextServer();
	
	if( previous ){
		previous->SetNextServer( next );
		
	}else{
		pHeadServer = next;
	}
	
	if( next ){
		next->SetPreviousServer( previous );
		
	}else{
		pTailServer = previous;
	}
	
	server->SetIsRegistered( false );
	server->SetNextServer( NULL );
	server->SetPreviousServer( NULL );
}

void deNetworkBasic::RegisterSocket( debnSocket *bnSocket ){
	if( ! bnSocket || bnSocket->GetIsRegistered() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTailSocket ){
		pTailSocket->SetNextSocket( bnSocket );
		bnSocket->SetPreviousSocket( pTailSocket );
		pTailSocket = bnSocket;
		
	}else{
		pHeadSocket = bnSocket;
		pTailSocket = bnSocket;
	}
	
	bnSocket->SetIsRegistered( true );
}

void deNetworkBasic::UnregisterSocket( debnSocket *bnSocket ){
	if( ! bnSocket || ! bnSocket->GetIsRegistered() ){
		DETHROW( deeInvalidParam );
	}
	
	debnSocket * const previous = bnSocket->GetPreviousSocket();
	debnSocket * const next = bnSocket->GetNextSocket();
	
	if( previous ){
		previous->SetNextSocket( next );
		
	}else{
		pHeadSocket = next;
	}
	
	if( next ){
		next->SetPreviousSocket( previous );
		
	}else{
		pTailSocket = previous;
	}
	
	bnSocket->SetIsRegistered( false );
	bnSocket->SetNextSocket( NULL );
	bnSocket->SetPreviousSocket( NULL );
}



void deNetworkBasic::FindPublicAddresses( decStringList &list ){
	// unix version
	#ifdef OS_UNIX
	const int sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		struct ifreq ifr;
		int ifindex = 1;
		memset( &ifr, 0, sizeof( ifr ) );
		char bufferIP[ 17 ];
		
		while( true ){
			#ifdef OS_BEOS
			ifr.ifr_index = ifindex++;
			#else
			ifr.ifr_ifindex = ifindex++;
			#endif
			if( ioctl( sock, SIOCGIFNAME, &ifr ) ){
				break;
			}
			
			if( ioctl( sock, SIOCGIFADDR, &ifr ) ){
				continue; // something failed, ignore the interface
			}
			const struct in_addr saddr = ( ( struct sockaddr_in & )ifr.ifr_addr ).sin_addr;
			
			/*
			if( ioctl( sock, SIOCGIFNETMASK, &ifr ) ){
				continue; // something failed, ignore the interface
			}
			const struct in_addr &saddrNetMask = ( ( struct sockaddr_in & )ifr.ifr_netmask ).sin_addr;
			*/
			
			if( ! inet_ntop( AF_INET, &saddr, bufferIP, 16 ) ){
				continue;
			}
			
			if( strcmp( bufferIP, "127.0.0.1" ) == 0 ){
				continue; // ignore localhost
			}
			
			LogInfoFormat( "Found public address: %s", bufferIP );
			list.Add( bufferIP );
			// ifr.ifr_name  => device name
		}
		close( sock );
		
	}catch( const deException & ){
		close( sock );
		throw;
	}
	#endif
	
	// beos version
	#ifdef OS_BEOS
	(void)list;
	DETHROW_INFO( deeInvalidParam, "TODO Implement this using BeOS means" );
	#endif
	
	// windows version
	#ifdef OS_W32
	// get size and allocate buffer
	PIP_ADAPTER_INFO pAdapterInfo = ( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap(), 0, sizeof( IP_ADAPTER_INFO ) );
	if( ! pAdapterInfo ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		ULONG ulOutBufLen = sizeof( IP_ADAPTER_INFO );
		if( GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW ){
			HeapFree( GetProcessHeap(), 0, pAdapterInfo );
			pAdapterInfo = ( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap(), 0, ulOutBufLen );
			if( ! pAdapterInfo ){
				DETHROW( deeInvalidParam );
			}
		}
		
		if( GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) != NO_ERROR ){
			DETHROW( deeInvalidParam );
		}
		
		// evaluate
		PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
		while( pAdapter ){
			// NOTE: IP_ADDR_STRING is a linked list and can potentially contain more than one address
			
			const unsigned long ulAddr = inet_addr( pAdapter->IpAddressList.IpAddress.String );
			if( ulAddr == INADDR_NONE || ulAddr == 0 /*0.0.0.0*/ ){
				pAdapter = pAdapter->Next;
				continue;
			}
			
			const unsigned long ulNetMask = inet_addr( pAdapter->IpAddressList.IpMask.String );
			if(ulNetMask == INADDR_NONE || ulNetMask == 0 /*0.0.0.0*/){
				pAdapter = pAdapter->Next;
				continue;
			}
			
			if( strcmp( pAdapter->IpAddressList.IpAddress.String, "127.0.0.1" ) == 0 ){
				pAdapter = pAdapter->Next;
				continue; // ignore localhost
			}
			
			LogInfoFormat( "Found public address: %s", pAdapter->IpAddressList.IpAddress.String );
			
			list.Add( pAdapter->IpAddressList.IpAddress.String );
			// pAdapter->AdapterName  => device name
			// (uint32_t)ulAddr   => address in in_addr format
			// (uint32_t)ulNetMask   => netmask in in_addr format
			
			pAdapter = pAdapter->Next;
		}
		
		HeapFree( GetProcessHeap(), 0, pAdapterInfo );
		
	}catch( const deException & ){
		if( pAdapterInfo ){
			HeapFree( GetProcessHeap(), 0, pAdapterInfo );
		}
		throw;
	}
	#endif
}

void deNetworkBasic::CloseConnections( debnSocket *bnSocket ){
	debnConnection *connection = pHeadConnection;
	
	while( connection ){
		debnConnection * const checkConnection = connection;
		connection = connection->GetNextConnection();
		
		if( checkConnection->GetSocket() == bnSocket ){
			checkConnection->Disconnect();
		}
	}
}



deBaseNetworkWorld *deNetworkBasic::CreateWorld( deWorld *world ){
	return new debnWorld( this, world );
}

deBaseNetworkServer *deNetworkBasic::CreateServer( deServer *server ){
	return new debnServer( this, server );
}

deBaseNetworkConnection *deNetworkBasic::CreateConnection( deConnection *connection ){
	return new debnConnection( this, connection );
}

deBaseNetworkState *deNetworkBasic::CreateState( deNetworkState *state ){
	return new debnState( *state );
}




// Private Functions
//////////////////////

debnConnection *deNetworkBasic::pFindConnection( const debnSocket *bnSocket, const debnAddress *address ) const{
	debnConnection *connection = pHeadConnection;
	
	while( connection ){
		if( connection->Matches( bnSocket, address ) ) return connection;
		
		connection = connection->GetNextConnection();
	}
	
	return NULL;
}

debnServer *deNetworkBasic::pFindServer( const debnSocket *bnSocket ) const{
	debnServer *server = pHeadServer;
	
	while( server ){
		if( bnSocket == server->GetSocket() ) return server;
		
		server = server->GetNextServer();
	}
	
	return NULL;
}



void deNetworkBasic::pReceiveDatagrams(){
	debnSocket *bnSocket = pHeadSocket;
	
	while( bnSocket ){
		while( bnSocket->ReceiveDatagram( pDatagram, pAddressReceive ) ){
			decBaseFileReaderReference reader;
			reader.TakeOver( new deNetworkMessageReader( pDatagram ) );
			
			const eCommandCodes command = ( eCommandCodes )reader->ReadByte();
			
			if( command == eccConnectionRequest ){
				debnServer * const server = pFindServer( bnSocket );
				if( server ){
					server->ProcessConnectionRequest( pAddressReceive, reader );
					{
					LogWarnFormat("ConnectRequest: Incoming address: %s", pAddressReceive->ToString().GetString());
					debnConnection *c = pHeadConnection;
					while(c){
						LogWarnFormat("- %s", c->GetRemoteAddress() ? c->GetRemoteAddress()->ToString().GetString() : "<null>");
						c = c->GetNextConnection();
					}
					}
				}else{
					LogError( "Connection request for a non existing socket? how in gods name is this possible?!\n" );
				}
				continue;
				
			}
			
			debnConnection * const connection = pFindConnection( bnSocket, pAddressReceive );
			if( connection ){
				switch( command ){
				case eccConnectionAck:
					connection->ProcessConnectionAck( reader );
					break;
					
				case eccConnectionClose:
					connection->ProcessConnectionClose( reader );
					break;
					
				case eccMessage:
					connection->ProcessMessage( reader );
					break;
					
				case eccReliableMessage:
					connection->ProcessReliableMessage( reader );
					break;
					
				case eccReliableLinkState:
					connection->ProcessReliableLinkState( reader );
					break;
					
				case eccReliableAck:
					connection->ProcessReliableAck( reader );
					break;
					
				case eccLinkUp:
					connection->ProcessLinkUp( reader );
					break;
					
				case eccLinkDown:
					connection->ProcessLinkDown( reader );
					break;
					
				case eccLinkUpdate:
					connection->ProcessLinkUpdate( reader );
					break;
					
				default:
					LogWarn( "Invalid command code, rejected!" );
				}
				
			}else{
				LogWarn( "Invalid datagram: Sender does not match any connection!" );
				{
				LogWarnFormat("Incoming address: %s (command %d)", pAddressReceive->ToString().GetString(), command);
				debnConnection *c = pHeadConnection;
				while(c){
					LogWarnFormat("- %s", c->GetRemoteAddress() ? c->GetRemoteAddress()->ToString().GetString() : "<null>");
					c = c->GetNextConnection();
				}
				}
			}
		}
		
		bnSocket = bnSocket->GetNextSocket();
	}
}

void deNetworkBasic::pProcessConnections( float elapsedTime ){
	debnConnection *connection = pHeadConnection;
	
	while( connection ){
		connection->Process( elapsedTime );
		
		connection = connection->GetNextConnection();
	}
}
