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
#include <stdint.h>

#include "debnAddress.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_UNIX
#	include <netdb.h>
#endif

#ifdef OS_W32
#	include <iphlpapi.h>
#endif



// Class debnAddress
//////////////////////

// Constructor, destructor
////////////////////////////

debnAddress::debnAddress(){
	SetIPv4Loopback();
}

debnAddress::~debnAddress(){
}



// Management
///////////////

void debnAddress::SetType( eAddressType type ){
	pType = type;
	
	switch( type ){
	case eatIPv4:
		SetIPv4Loopback();
		break;
		
	case eatIPv6:
		SetIPv6Loopback();
		break;
	}
}

int debnAddress::GetValueAt( int location ) const{
	DEASSERT_TRUE( location < 0 )
	DEASSERT_TRUE( location >= pValueCount )
	
	return pValues[ location ];
}

void debnAddress::SetValueAt( int location, int value ){
	DEASSERT_TRUE( location < 0 )
	DEASSERT_TRUE( location >= pValueCount )
	DEASSERT_TRUE( value < 0 )
	DEASSERT_TRUE( value > 255 )
	
	pValues[ location ] = ( unsigned char )value;
}

void debnAddress::SetPort( int port ){
	DEASSERT_TRUE( port < 0 )
	
	pPort = port;
}

void debnAddress::SetIPv4( int values[ 4 ], int port ){
	DEASSERT_TRUE( port < 0 )
	
	int i;
	for( i=0; i<4; i++ ){
		DEASSERT_TRUE( values[ i ] < 0 )
		DEASSERT_TRUE( values[ i ] > 255 )
	}
	
	pType = eatIPv4;
	pPort = port;
	
	memset( pValues, 0, sizeof( pValues ) );
	for( i=0; i<4; i++ ){
		pValues[ i ] = ( unsigned char )values[ i ];
	}
	pValueCount = 4;
}

void debnAddress::SetIPv4Any(){
	pType = eatIPv4;
	pPort = 0;
	
	memset( pValues, 0, sizeof( pValues ) );
	pValueCount = 4;
}

void debnAddress::SetIPv4Loopback(){
	pType = eatIPv4;
	pPort = 3413;
	
	memset( pValues, 0, sizeof( pValues ) );
	pValues[ 0 ] = 127;
	pValues[ 3 ] = 1;
	pValueCount = 4;
}

void debnAddress::SetIPv4FromSocket( const sockaddr_in &address ){
	uint32_t sockAddr = ntohl( address.sin_addr.s_addr );
	
	pType = eatIPv4;
	pPort = ntohs( address.sin_port );
	
	memset( pValues, 0, sizeof( pValues ) );
	pValues[ 0 ] = ( unsigned char )( ( sockAddr >> 24 ) & 0xff );
	pValues[ 1 ] = ( unsigned char )( ( sockAddr >> 16 ) & 0xff );
	pValues[ 2 ] = ( unsigned char )( ( sockAddr >> 8 ) & 0xff );
	pValues[ 3 ] = ( unsigned char )( sockAddr & 0xff );
	pValueCount = 4;
}

void debnAddress::SetSocketIPv4( sockaddr_in &address ) const{
	DEASSERT_TRUE( pType == eatIPv4 )
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( ( pValues[ 0 ] << 24 ) + ( pValues[ 1 ] << 16 )
		+ ( pValues[ 2 ] << 8 ) + pValues[ 3 ] );
	address.sin_port = htons( pPort );
}

void debnAddress::SetIPv6Any(){
	pType = eatIPv6;
	pPort = 0;
	memset( pValues, 0, sizeof( pValues ) );
	pValueCount = 16;
}

void debnAddress::SetIPv6Loopback(){
	pType = eatIPv6;
	pPort = 3413;
	
	memset( pValues, 0, sizeof( pValues ) );
	pValues[ 15 ] = 1;
	pValueCount = 16;
}

void debnAddress::SetIPv6( int values[ 16 ], int port ){
	DEASSERT_TRUE( port < 0 )
	
	int i;
	for( i=0; i<16; i++ ){
		DEASSERT_TRUE( values[ i ] < 0 )
		DEASSERT_TRUE( values[ i ] > 255 )
	}
	
	pType = eatIPv6;
	pPort = port;
	
	memset( pValues, 0, sizeof( pValues ) );
	for( i=0; i<16; i++ ){
		pValues[ i ] = ( unsigned char )values[ i ];
	}
	pValueCount = 16;
}

void debnAddress::SetIPv6FromSocket( const sockaddr_in6 &address ){
	int i;
	
	pType = eatIPv6;
	
#ifdef OS_W32
	const USHORT * const sain = address.sin6_addr.u.Word;
	const uint16_t sa16[ 8 ] = { ntohs( sain[ 0 ] ), ntohs( sain[ 1 ] ), ntohs( sain[ 2 ] ),
		ntohs( sain[ 3 ] ), ntohs( sain[ 4 ] ), ntohs( sain[ 5 ] ), ntohs( sain[ 6 ] ), ntohs( sain[ 7 ] ) };
	
	for( i=0; i<16; i+=2 ){
		const uint16_t &in = sa16[ i / 2 ];
		pValues[ i ] = ( uint8_t )( ( in >> 8 ) & 0xff );
		pValues[ i + 1 ] = ( uint8_t )( in & 0xff );
	}
	
#else
	const uint32_t * const sain = address.sin6_addr.s6_addr32;
	const uint32_t sa32[ 4 ] = { ntohl( sain[ 0 ] ), ntohl( sain[ 1 ] ), ntohl( sain[ 2 ] ), ntohl( sain[ 3 ] ) };
	
	for( i=0; i<16; i+=4 ){
		const uint32_t &in = sa32[ i / 4 ];
		pValues[ i ] = ( uint8_t )( ( in >> 24 ) & 0xff );
		pValues[ i + 1 ] = ( uint8_t )( ( in >> 16 ) & 0xff );
		pValues[ i + 2 ] = ( uint8_t )( ( in >> 8 ) & 0xff );
		pValues[ i + 3 ] = ( uint8_t )( in & 0xff );
	}
#endif
	
	pValueCount = 16;
	pPort = ntohs( address.sin6_port );
}

void debnAddress::SetSocketIPv6( sockaddr_in6 &address ) const{
	DEASSERT_TRUE( pType == eatIPv6 )
	
	int i;
	
#ifdef OS_W32
	for( i=0; i<16; i+=2 ){
		address.sin6_addr.u.Word[ i / 2 ] = htons( ( pValues[ i ] << 8 ) + pValues[ i + 1 ] );
	}
	
#else
	for( i=0; i<16; i+=4 ){
		address.sin6_addr.s6_addr32[ i / 4 ] = htonl( ( pValues[ i ] << 24 )
			+ ( pValues[ i + 1 ] << 16 ) + ( pValues[ i + 2 ] << 8 ) + pValues[ i + 3 ] );
	}
#endif
	
	address.sin6_family = AF_INET6;
	address.sin6_port = htons( pPort );
}

void debnAddress::SetFromString( const char *address ){
	const decString saddress( address );
	DEASSERT_FALSE( saddress.IsEmpty() )
	
	addrinfo hints;
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;
	
	const int delimiter = saddress.FindReverse( ':' );
	int portBegin = -1;
	decString node;
	
	if( delimiter != -1 ){
		if( saddress[ 0 ] == '[' ){
			// "[IPv6]:port"
			DEASSERT_TRUE( saddress[ delimiter - 1 ] == ']' )
			
			node = saddress.GetMiddle( 1, delimiter - 1 );
			portBegin = delimiter + 1;
			
			hints.ai_family = AF_INET6;
			hints.ai_flags |= AI_NUMERICHOST;
			
		}else if( saddress.Find( ':' ) != delimiter ){
			// "IPv6"
			node = saddress;
			
			hints.ai_family = AF_INET6;
			hints.ai_flags |= AI_NUMERICHOST;
			
		}else{
			// "IPv4:port" or "hostname:port"
			node = saddress.GetMiddle( 0, delimiter );
			portBegin = delimiter + 1;
			
			hints.ai_family = AF_UNSPEC;
		}
		
	}else{
		// "IPv4" or "hostname"
		node = saddress;
		
		hints.ai_family = AF_UNSPEC;
	}
	
	uint16_t port = 3413;
	
	if( portBegin != -1 ){
		char *end;
		port = ( uint16_t )strtol( address + portBegin, &end, 10 );
		DEASSERT_TRUE( *end == 0 )
	}
	
	addrinfo *result;
	int retcode = getaddrinfo( node, nullptr, &hints, &result );
	if( retcode ){
		decString message;
		message.Format( "getaddrinfo: %s (%d)", gai_strerrorA( retcode ), retcode );
		DETHROW_INFO( deeInvalidParam, message );
	}
	
	try{
		// there can be more than one address but we use the first one. using AI_ADDRCONFIG
		// should give us IPv6 if the host system has an IPv6 address otherwise IPv4.
		// should this be a problem we have to do this differently.
		// 
		// according to documentation the first returned address should be used since the
		// lookup function has internal sorting logic returning the preferred address first
		const addrinfo &rai = result[ 0 ];
		
		// set address and port
		if( rai.ai_family == AF_INET6 ){
			SetIPv6FromSocket( *( ( const sockaddr_in6 * )rai.ai_addr ) );
			
		}else if(rai.ai_family == AF_INET){
			SetIPv4FromSocket( *( ( const sockaddr_in * )rai.ai_addr ) );
			
		}else{
			DETHROW_INFO( deeInvalidParam, "unknown family" );
		}
		
		pPort = port; // has to come afterwards since port has not been used for resolving
		
		freeaddrinfo( result );
		
	}catch(...){
		freeaddrinfo( result );
		throw;
	}
}

decString debnAddress::ToString() const{
	bool groupingZeros = false;
	bool canGroupZeros = true;
	decString s;
	int i;
	
	switch( pType ){
	case eatIPv4:
		s.Format( "%d.%d.%d.%d:%d", pValues[ 0 ], pValues[ 1 ], pValues[ 2 ], pValues[ 3 ], pPort );
		break;
		
	case eatIPv6:
		s.Append( "[" );
		
		for( i=0; i<8; i++ ){
			const int a = pValues[ i * 2 ];
			const int b = pValues[ i * 2 + 1 ];
			
			// groups of 0 can be truncated but only once
			if( ! a && ! b ){
				if( groupingZeros ){
					continue;
					
				}else if( canGroupZeros ){
					s.Append( ":" );
					groupingZeros = true;
					continue;
				}
				
			}else if( groupingZeros ){
				groupingZeros = false;
				canGroupZeros = false;
			}
			
			// leading zeros can be truncated
			if( i > 0 ){
				s.Append( ":" );
			}
			s.AppendFormat( "%x", ( a << 8 ) | b );
		}
		
		if( groupingZeros ){
			s.Append( ":" );
		}
		
		s.Append( "]:" );
		s.AppendValue( pPort );
		break;
		
	default:
		s = "?";
	}
	
	return s;
}



// Operators
//////////////

debnAddress &debnAddress::operator=( const debnAddress &address ){
	pType = address.pType;
	pPort = address.pPort;
	memcpy( pValues, address.pValues, sizeof( pValues ) );
	pValueCount = address.pValueCount;
	return *this;
}

bool debnAddress::operator==( const debnAddress &address ) const{
	return pType == address.pType && pPort == address.pPort && pValueCount == address.pValueCount
		&& memcmp( &pValues, &address.pValues, pValueCount ) == 0;
}
