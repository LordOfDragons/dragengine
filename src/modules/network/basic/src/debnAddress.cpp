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
#endif

#ifdef OS_W32
#	include <dragengine/app/include_windows.h>
typedef unsigned int uint32_t;
#endif

#include "debnAddress.h"
#include <dragengine/common/exceptions.h>



// Class debnAddress
//////////////////////

// Constructor, destructor
////////////////////////////

debnAddress::debnAddress() :
pType( eatIPv4 )
{
	pValues[ 0 ] = 0;
	pValues[ 1 ] = 0;
	pValues[ 2 ] = 0;
	pValues[ 3 ] = 0;
	pValueCount = 4;
	pPort = 0;
}

debnAddress::~debnAddress(){
}



// Management
///////////////

void debnAddress::SetType( int type ){
	if( type == eatIPv4 ){
		pValues[ 0 ] = 127;
		pValues[ 1 ] = 0;
		pValues[ 2 ] = 0;
		pValues[ 3 ] = 1;
		pValueCount = 4;
		
	}else if( type == eatIPv6 ){
		DETHROW( deeInvalidParam );
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	pType = type;
}

int debnAddress::GetValueAt( int location ) const{
	if( location < 0 || location >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pValues[ location ];
}

void debnAddress::SetValueAt( int location, int value ){
	if( location < 0 || location >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	if( value < 0 || value > 255 ){
		DETHROW( deeInvalidParam );
	}
	
	pValues[ location ] = ( unsigned char )value;
}

void debnAddress::SetPort( int port ){
	if( port < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPort = port;
}

void debnAddress::SetIPv4( int values[ 4 ], int port ){
	if( port < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<4; i++ ){
		if( values[ i ] < 0 || values[ i ] > 255 ){
			DETHROW( deeInvalidParam );
		}
	}
	
	pType = eatIPv4;
	for( i=0; i<4; i++ ){
		pValues[ i ] = ( unsigned char )values[ i ];
	}
	pValueCount = 4;
	
	pPort = port;
}

void debnAddress::SetIPv4Any(){
	pType = eatIPv4;
	
	pValues[ 0 ] = 0;
	pValues[ 1 ] = 0;
	pValues[ 2 ] = 0;
	pValues[ 3 ] = 0;
	pValueCount = 4;
	
	pPort = 0;
}

void debnAddress::SetIPv4Loopback(){
	pType = eatIPv4;
	
	pValues[ 0 ] = 127;
	pValues[ 1 ] = 0;
	pValues[ 2 ] = 0;
	pValues[ 3 ] = 1;
	pValueCount = 4;
	
	pPort = 3413;
}

void debnAddress::SetIPv4FromString( const char *address ){
	if( ! address ){
		DETHROW( deeInvalidParam );
	}
	
	const char *delimiter = strchr( address, ':' );
	struct hostent *he = NULL;
	uint32_t sockAddr;
	int port = 3413;
	
	// get address and port if present
	if( delimiter ){
		port = ( int )strtol( delimiter + 1, NULL, 10 );
		int offset = ( int )( delimiter - address );
		
		char *tempAddress = new char[ offset + 1 ];
		
		strncpy( tempAddress, address, offset );
		tempAddress[ offset ] = '\0';
		
		he = gethostbyname( tempAddress );
		
		delete [] tempAddress;
		
	}else{
		he = gethostbyname( address );
	}
	
	if( ! he ){
		DETHROW( deeInvalidParam );
	}
	
	if( port < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	// set address and port
	pType = eatIPv4;
	
	sockAddr = ntohl( ( ( struct in_addr * )he->h_addr )->s_addr );
	pValues[ 0 ] = ( unsigned char )( ( sockAddr >> 24 ) & 0xff );
	pValues[ 1 ] = ( unsigned char )( ( sockAddr >> 16 ) & 0xff );
	pValues[ 2 ] = ( unsigned char )( ( sockAddr >> 8 ) & 0xff );
	pValues[ 3 ] = ( unsigned char )( sockAddr & 0xff );
	pValueCount = 4;
	
	pPort = port;
}

void debnAddress::SetIPv4FromSocket( const struct sockaddr_in &address ){
	uint32_t sockAddr = ntohl( address.sin_addr.s_addr );
	
	pType = eatIPv4;
	
	pValues[ 0 ] = ( unsigned char )( ( sockAddr >> 24 ) & 0xff );
	pValues[ 1 ] = ( unsigned char )( ( sockAddr >> 16 ) & 0xff );
	pValues[ 2 ] = ( unsigned char )( ( sockAddr >> 8 ) & 0xff );
	pValues[ 3 ] = ( unsigned char )( sockAddr & 0xff );
	pValueCount = 4;
	
	pPort = ntohs( address.sin_port );
}

void debnAddress::SetSocketIPv4( struct sockaddr_in &address ) const{
	if( pType != eatIPv4 ) DETHROW( deeInvalidParam );
	
	address.sin_addr.s_addr = htonl( ( pValues[ 0 ] << 24 ) + ( pValues[ 1 ] << 16 )
		+ ( pValues[ 2 ] << 8 ) + pValues[ 3 ] );
	address.sin_port = htons( pPort );
}

decString debnAddress::ToString() const{
	decString string;
	
	if( pType == eatIPv4 ){
		string.Format( "%d.%d.%d.%d:%d", pValues[ 0 ], pValues[ 1 ], pValues[ 2 ], pValues[ 3 ], pPort );
		
	}else{
		string = "?";
	}
	
	return string;
}



// Operators
//////////////

debnAddress &debnAddress::operator=( const debnAddress &address ){
	pType = address.pType;
	pPort = address.pPort;
	
	int i;
	for( i=0; i<4; i++ ){
		pValues[ i ] = address.pValues[ i ];
	}
	pValueCount = address.pValueCount;
	
	return *this;
}

bool debnAddress::operator==( const debnAddress &address ) const{
	if( pType != address.pType ){
		return false;
	}
	if( pPort != address.pPort ){
		return false;
	}
	
	int i;
	for( i=0; i<pValueCount; i++ ){
		if( pValues[ i ] != address.pValues[ i ] ){
			return false;
		}
	}
	
	return true;
}
