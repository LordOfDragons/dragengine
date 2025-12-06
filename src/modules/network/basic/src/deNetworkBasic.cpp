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

#ifdef OS_W32
#include <winsock2.h>
#include <dragengine/app/include_windows.h>
#endif

#include "debnServer.h"
#include "debnSocket.h"
#include "debnAddress.h"
#include "debnConnection.h"
#include "debnWorld.h"
#include "deNetworkBasic.h"
#include "configuration/debnLoadConfiguration.h"
#include "parameters/debug/debnPLogLevel.h"
#include "parameters/reliability/debnPConnectResendInterval.h"
#include "parameters/reliability/debnPConnectTimeout.h"
#include "parameters/reliability/debnPReliableResendInterval.h"
#include "parameters/reliability/debnPReliableTimeout.h"
#include "states/debnState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageReader.h>
#include <dragengine/resources/network/deNetworkMessageWriter.h>
#include <dragengine/systems/modules/deModuleParameter.h>



// Export Definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *BasicNetworkCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
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
deBaseNetworkModule( loadableModule ),
pHeadConnection( nullptr ),
pTailConnection( nullptr ),
pHeadServer( nullptr ),
pTailServer( nullptr ),
pHeadSocket( nullptr ),
pTailSocket( nullptr ),
pDatagram( nullptr )
#ifdef OS_W32
,pWSAStartupCalled( false )
#endif
{
	try{
		pParameters.AddParameter( new debnPLogLevel( *this ) );
		/*
		pParameters.AddParameter( new debnPConnectResendInterval( *this ) );
		pParameters.AddParameter( new debnPConnectTimeout( *this ) );
		pParameters.AddParameter( new debnPReliableResendInterval( *this ) );
		pParameters.AddParameter( new debnPReliableTimeout( *this ) );
		*/
		
		debnLoadConfiguration( *this ).LoadConfig( pConfiguration );
		
	}catch( const deException & ){
		CleanUp();
		throw;
	}
}

deNetworkBasic::~deNetworkBasic(){
	CleanUp();
}



// Management
///////////////

bool deNetworkBasic::Init(){
	try{
		// operating system specific startup calls
		#ifdef OS_W32
		WSADATA wsaData;
		if( WSAStartup(MAKEWORD( 2, 2 ), &wsaData ) ){
			DETHROW_INFO( deeInvalidAction, "WSAStartup failed" );
		}
		pWSAStartupCalled = true;
		
		if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ){
			DETHROW_INFO( deeInvalidAction, "WSAStartup succeeded but returned unsupported version" );
		}
		#endif
		
		// create shared datagram
		pDatagram = new deNetworkMessage;
		pDatagram->SetDataLength( 1024 );
		
		// create receive address
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
		
	}catch( const deException &e ){
		LogException( e );
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
	
	// operating system specific startup calls
	#ifdef OS_W32
	if( pWSAStartupCalled ){
		pWSAStartupCalled = false;
		WSACleanup();
	}
	#endif
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
	debnSocket::FindAddresses( list, true );
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



int deNetworkBasic::GetParameterCount() const{
	return pParameters.GetParameterCount();
}

void deNetworkBasic::GetParameterInfo( int index, deModuleParameter &info ) const{
	info = pParameters.GetParameterAt( index );
}

int deNetworkBasic::IndexOfParameterNamed( const char *name ) const{
	return pParameters.IndexOfParameterNamed( name );
}

decString deNetworkBasic::GetParameterValue( const char *name ) const{
	return pParameters.GetParameterNamed( name ).GetParameterValue();
}

void deNetworkBasic::SetParameterValue( const char *name, const char *value ){
	pParameters.GetParameterNamed( name ).SetParameterValue( value );
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

debnConnection *deNetworkBasic::pFindConnection( const debnSocket *bnSocket, const debnAddress &address ) const{
	debnConnection *connection = pHeadConnection;
	while( connection ){
		if( connection->Matches( bnSocket, address ) ){
			return connection;
		}
		connection = connection->GetNextConnection();
	}
	return nullptr;
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
		while( bnSocket->ReceiveDatagram( *pDatagram, pAddressReceive ) ){
			deNetworkMessageReader::Ref reader(deNetworkMessageReader::Ref::NewWith(pDatagram));
			
			debnConnection * const connection = pFindConnection( bnSocket, pAddressReceive );
			const eCommandCodes command = ( eCommandCodes )reader->ReadByte();
			
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
					
				case eccReliableMessageLong:
					connection->ProcessReliableMessageLong( reader );
					break;
					
				case eccReliableLinkStateLong:
					connection->ProcessReliableLinkStateLong( reader );
					break;
					
				default:
					break;
				}
				
			}else if( command == eccConnectionRequest ){
				debnServer * const server = pFindServer( bnSocket );
				if( server ){
					server->ProcessConnectionRequest( pAddressReceive, reader );
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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class denbModuleInternal : public deInternalModule{
public:
	denbModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("BasicNetwork");
		SetDescription("Basic network module.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtNetwork);
		SetDirectoryName("basic");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(BasicNetworkCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *denbRegisterInternalModule(deModuleSystem *system){
	return new denbModuleInternal(system);
}
#endif
