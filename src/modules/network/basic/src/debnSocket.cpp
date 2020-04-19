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
#include <unistd.h>

#ifdef OS_UNIX
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <sys/poll.h>
#endif

#ifdef OS_W32
#	include <dragengine/app/include_windows.h>
typedef int socklen_t;
#endif

#include "debnSocket.h"
#include "debnServer.h"
#include "debnAddress.h"
#include "deNetworkBasic.h"

#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/common/exceptions.h>



// Class debnSocket
/////////////////////

// Constructor, destructor
////////////////////////////

debnSocket::debnSocket( deNetworkBasic *netBasic ){
	if( ! netBasic ){
		DETHROW( deeInvalidParam );
	}
	
	pNetBasic = netBasic;
	pAddress = NULL;
	pSocket = -1;
	
	pPreviousSocket = NULL;
	pNextSocket = NULL;
	pIsRegistered = false;
	
	try{
		pAddress = new debnAddress;
		if( ! pAddress ) DETHROW( deeOutOfMemory );
		
		// create socket
		pSocket = socket( PF_INET, SOCK_DGRAM, 0 );
		if( pSocket == -1 ){
			pNetBasic->LogError( "socket failed." );
			DETHROW( deeInvalidParam );
		}
		
		// register us
		netBasic->RegisterSocket( this );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debnSocket::~debnSocket(){
	pCleanUp();
}



// Management
///////////////

void debnSocket::Bind(){
	struct sockaddr_in sa;
	
	// prepare the socket address
	memset( &sa, '\0', sizeof( sa ) );
    sa.sin_family = AF_INET;
    pAddress->SetSocketIPv4( sa );
	
	// bind the socket
// 	pNetBasic->LogInfoFormat( "binding socket to %d.%d.%d.%d:%d.", pAddress->GetValueAt( 0 ),
// 		pAddress->GetValueAt( 1 ), pAddress->GetValueAt( 2 ), pAddress->GetValueAt( 3 ),
// 		pAddress->GetPort() );
	
	if( bind( pSocket, ( struct sockaddr * )&sa, sizeof( sockaddr ) ) ){
		pNetBasic->LogError( "bind failed." );
		DETHROW( deeInvalidParam );
	}
	
	// retrieve the state of the socket
	socklen_t slen = sizeof( sockaddr );
	
	if( getsockname( pSocket, ( struct sockaddr * )&sa, &slen ) ){
		pNetBasic->LogError( "getsockname failed." );
		DETHROW( deeInvalidParam );
	}
	pAddress->SetIPv4FromSocket( sa );
	
// 	pNetBasic->LogInfoFormat( "socket bound to %d.%d.%d.%d:%d", pAddress->GetValueAt( 0 ),
// 		pAddress->GetValueAt( 1 ), pAddress->GetValueAt( 2 ),
// 		pAddress->GetValueAt( 3 ), pAddress->GetPort() );
}

bool debnSocket::ReceiveDatagram( deNetworkMessage *stream, debnAddress *address ){
	if( ! stream || ! address ){
		DETHROW( deeInvalidParam );
	}
	
	socklen_t slen = sizeof( sockaddr );
	struct sockaddr_in sa;
#ifdef OS_UNIX
	struct pollfd ufd;
#endif
#ifdef OS_W32
	fd_set fd;
#endif
	int dataLen = 0;
	
#ifdef OS_UNIX
	ufd.fd = pSocket;
	ufd.events = POLLIN;
	if( poll( &ufd, 1, 0 ) > 0 )
#endif
#ifdef OS_W32
	FD_ZERO( &fd );
	FD_SET( pSocket, &fd );
	
	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if( select( 0, &fd, NULL, NULL, &tv ) == 1 )
#endif
	{
		dataLen = 8192; // TEMP HACK !!!
		
		stream->SetDataLength( dataLen );
		
		#ifdef OS_W32
		dataLen = recvfrom( pSocket, ( char* )stream->GetBuffer(),
			dataLen, 0, ( struct sockaddr * )&sa, &slen );
		#else
		dataLen = recvfrom( pSocket, stream->GetBuffer(),
			dataLen, 0, ( struct sockaddr * )&sa, &slen );
		#endif
		
		if( dataLen > 0 ){
			address->SetIPv4FromSocket( sa );
			
// 			pNetBasic->LogInfoFormat( "Received datagram with length %d from %d.%d.%d.%d:%d command code %d",
// 				dataLen, address->GetValueAt( 0 ), address->GetValueAt( 1 ),
// 				address->GetValueAt( 2 ), address->GetValueAt( 3 ),
// 				address->GetPort(), stream->GetBuffer()[ 0 ] );
			
		}else if( dataLen < 0 ){
			// connection closed. return 0 length
			dataLen = 0;
		}
	}
	
	stream->SetDataLength( dataLen );
	
	return ( dataLen > 0 );
}

void debnSocket::SendDatagram( const deNetworkMessage *stream, const debnAddress *address ){
	if( ! stream || ! address ){
		DETHROW( deeInvalidParam );
	}
	
	struct sockaddr_in sa;
	
	memset( &sa, '\0', sizeof( sa ) );
    sa.sin_family = AF_INET;
	address->SetSocketIPv4( sa );
	
	#ifdef OS_W32
	sendto( pSocket, ( char* )stream->GetBuffer(), stream->GetDataLength(),
		0, ( struct sockaddr * )&sa, sizeof( sockaddr ) );
	#else
	sendto( pSocket, stream->GetBuffer(), stream->GetDataLength(),
		0, ( struct sockaddr * )&sa, sizeof( sockaddr ) );
	#endif
	
// 	pNetBasic->LogInfoFormat( "Send datagram with length %d to %d.%d.%d.%d:%d command code %d",
// 		stream->GetDataLength(), address->GetValueAt( 0 ), address->GetValueAt( 1 ),
// 		address->GetValueAt( 2 ), address->GetValueAt( 3 ), address->GetPort(),
// 		stream->GetBuffer()[ 0 ] );
}



// Linked List
////////////////

void debnSocket::SetPreviousSocket( debnSocket *bnSocket ){
	pPreviousSocket = bnSocket;
}

void debnSocket::SetNextSocket( debnSocket *bnSocket ){
	pNextSocket = bnSocket;
}

void debnSocket::SetIsRegistered( bool isRegistered ){
	pIsRegistered = isRegistered;
}



// Private Functions
//////////////////////

void debnSocket::pCleanUp(){
	if( pNetBasic ){
		pNetBasic->UnregisterSocket( this );
	}
	
	if( pSocket != -1 ){
		close( pSocket );
	}
	if( pAddress ){
		delete pAddress;
	}
}
