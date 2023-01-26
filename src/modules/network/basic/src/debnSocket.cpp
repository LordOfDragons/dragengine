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
#include "deNetworkBasic.h"

#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/common/exceptions.h>

#ifdef OS_UNIX
#	include <errno.h>
#	include <netdb.h>
#	include <sys/poll.h>
#	include <ifaddrs.h>
#	include <net/if.h>
#	include <unistd.h>
#endif

#ifdef OS_BEOS
#	include <sys/sockio.h>
#endif

#ifdef OS_W32
#	include <iphlpapi.h>
typedef int socklen_t;
#endif



// Class debnSocket
/////////////////////

// Constructor, destructor
////////////////////////////

debnSocket::debnSocket( deNetworkBasic &netBasic ) :
pNetBasic( netBasic ),
#ifdef OS_W32
	pSocket( -1 ),
#else
	pSocket( -1 ),
#endif
pPreviousSocket( nullptr ),
pNextSocket( nullptr ),
pIsRegistered( false )
{
	netBasic.RegisterSocket( this );
}

debnSocket::~debnSocket(){
	pCleanUp();
}



// Management
///////////////

void debnSocket::Bind(){
	DEASSERT_TRUE( pSocket == -1 )
	
	pNetBasic.LogInfoFormat( "Bind socket to '%s'", pAddress.ToString().GetString() );
	
	if( pAddress.GetType() == debnAddress::eatIPv6 ){
		// create socket
		pSocket = socket( PF_INET6, SOCK_DGRAM, 0 );
		if( pSocket == -1 ){
			ThrowSocketError( "socket failed" );
		}
		
		// prepare the socket address
		sockaddr_in6 sa;
		memset( &sa, 0, sizeof( sa ) );
		pAddress.SetSocketIPv6( sa );
		
		sa.sin6_scope_id = pScopeIdFor( sa ); // required for local links or it fails to bind
		
		// bind the socket
		if( bind( pSocket, ( sockaddr* )&sa, sizeof( sa ) ) ){
			ThrowSocketError( "bind failed" );
		}
		
		// retrieve the state of the socket
		socklen_t slen = sizeof( sa );
		if( getsockname( pSocket, ( sockaddr* )&sa, &slen ) ){
			ThrowSocketError( "getsockname failed" );
		}
		pAddress.SetIPv6FromSocket( sa );
		
	}else{
		// create socket
		pSocket = socket( PF_INET, SOCK_DGRAM, 0 );
		if( pSocket == -1 ){
			ThrowSocketError( "socket failed" );
		}
		
		// prepare the socket address
		sockaddr_in sa;
		memset( &sa, 0, sizeof( sa ) );
		pAddress.SetSocketIPv4( sa );
		
		// bind the socket
		if( bind( pSocket, ( sockaddr * )&sa, sizeof( sa ) ) ){
			ThrowSocketError( "bind failed" );
		}
		
		// retrieve the state of the socket
		socklen_t slen = sizeof( sa );
		if( getsockname( pSocket, ( sockaddr * )&sa, &slen ) ){
			ThrowSocketError( "getsockname failed" );
		}
		pAddress.SetIPv4FromSocket( sa );
	}
	
	pNetBasic.LogInfoFormat( "Bound socket to '%s'", pAddress.ToString().GetString() );
}

bool debnSocket::ReceiveDatagram( deNetworkMessage &stream, debnAddress &address ){
	int dataLen = 0;
	
#ifdef OS_UNIX
	pollfd ufd;
	ufd.fd = pSocket;
	ufd.events = POLLIN;
	if( poll( &ufd, 1, 0 ) > 0 )
	
#elif defined OS_W32
	fd_set fd;
	FD_ZERO( &fd );
	FD_SET( pSocket, &fd );
	
	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if( select( 0, &fd, NULL, NULL, &tv ) == 1 )
#endif
	{
		dataLen = 8192; // TEMP HACK !!!
		stream.SetDataLength( dataLen );
		
		if( pAddress.GetType() == debnAddress::eatIPv6 ){
			sockaddr_in6 sa;
			socklen_t slen = sizeof( sa );
			
			#ifdef OS_W32
			dataLen = recvfrom( pSocket, ( char* )stream.GetBuffer(), dataLen, 0, ( sockaddr* )&sa, &slen );
			#else
			dataLen = recvfrom( pSocket, stream.GetBuffer(), dataLen, 0, ( sockaddr* )&sa, &slen );
			#endif
			
			if( dataLen > 0 ){
				address.SetIPv6FromSocket( sa );
			}
			
		}else{
			sockaddr_in sa;
			socklen_t slen = sizeof( sa );
			
			#ifdef OS_W32
			dataLen = recvfrom( pSocket, ( char* )stream.GetBuffer(), dataLen, 0, ( sockaddr* )&sa, &slen );
			#else
			dataLen = recvfrom( pSocket, stream.GetBuffer(), dataLen, 0, ( sockaddr* )&sa, &slen );
			#endif
			
			if( dataLen > 0 ){
				address.SetIPv4FromSocket( sa );
			}
		}
		
		if( dataLen < 0 ){
			// connection closed. return 0 length
			dataLen = 0;
		}
		
		/*
		if( dataLen > 0 ){
			pNetBasic.LogInfoFormat( "Received datagram with length %d from '%s' command code %d",
				dataLen, address.ToString().GetString(), stream.GetBuffer()[ 0 ] );
		}
		*/
	}
	
	stream.SetDataLength( dataLen );
	return dataLen > 0;
}

void debnSocket::SendDatagram( const deNetworkMessage &stream, const debnAddress &address ){
	if( pAddress.GetType() == debnAddress::eatIPv6 ){
		sockaddr_in6 sa;
		memset( &sa, 0, sizeof( sa ) );
		address.SetSocketIPv6( sa );
		
		#ifdef OS_W32
		sendto( pSocket, ( char* )stream.GetBuffer(), stream.GetDataLength(), 0, ( sockaddr* )&sa, sizeof( sa ) );
		#else
		sendto( pSocket, stream.GetBuffer(), stream.GetDataLength(), 0, ( sockaddr* )&sa, sizeof( sa ) );
		#endif
		
	}else{
		sockaddr_in sa;
		memset( &sa, 0, sizeof( sa ) );
		address.SetSocketIPv4( sa );
		
		#ifdef OS_W32
		sendto( pSocket, ( char* )stream.GetBuffer(), stream.GetDataLength(), 0, ( sockaddr* )&sa, sizeof( sa ) );
		#else
		sendto( pSocket, stream.GetBuffer(), stream.GetDataLength(), 0, ( sockaddr* )&sa, sizeof( sa ) );
		#endif
	}
	
// 	pNetBasic.LogInfoFormat( "Send datagram with length %d to '%s' command code %d",
// 		stream.GetDataLength(), address.ToString().GetString(), stream.GetBuffer()[ 0 ] );
}

void debnSocket::ThrowSocketError( const char *message ){
	decString s;
	
#ifdef OS_W32
	const int error = WSAGetLastError();
	char *errbuf = NULL;
	FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), ( LPSTR )&errbuf, 0, NULL );
	s.Format( "%s: %s (%d)", message, errbuf, error );
	LocalFree( errbuf );
	
#else
	const int error = errno;
	s.Format( "%s: %s (%d)", message, strerror( error ), error );
#endif
	
	DETHROW_INFO( deeInvalidAction, s );
}

void debnSocket::FindAddresses( decStringList &list, bool onlyPublic ){
#ifdef OS_W32
	// get size and allocate buffer
	PIP_ADAPTER_ADDRESSES addresses = ( IP_ADAPTER_ADDRESSES* )HeapAlloc( GetProcessHeap(), 0, 15000 );
	DEASSERT_NOTNULL( addresses )
	
	ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST
		| GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_INCLUDE_PREFIX;
	
	// get the data. this stupid way to do it is actually recommended by microsoft
	ULONG outBufLen = 15000;
	ULONG iterations = 0;
	DWORD dwRetVal = NO_ERROR;
	
	do{
		addresses = ( IP_ADAPTER_ADDRESSES* )HeapAlloc( GetProcessHeap(), 0, outBufLen );
		DEASSERT_NOTNULL( addresses )
		
		dwRetVal = GetAdaptersAddresses( AF_UNSPEC, flags, NULL, addresses, &outBufLen );
		if( dwRetVal == ERROR_BUFFER_OVERFLOW ){
			HeapFree( GetProcessHeap(), 0, addresses );
			addresses = NULL;
			
		}else{
			break;
		}
		
		iterations++;
	}while( dwRetVal == ERROR_BUFFER_OVERFLOW && iterations < 3 );
	
	if( dwRetVal != NO_ERROR ){
		HeapFree( GetProcessHeap(), 0, addresses );
		DETHROW_INFO( deeInvalidAction, "GetAdaptersAddresses failed" );
	}
	
	// evaluate the result
	try{
		// find IPv6 address
		PIP_ADAPTER_ADDRESSES curAddress = addresses;
		while( curAddress ){
			PIP_ADAPTER_UNICAST_ADDRESS ua = curAddress->FirstUnicastAddress;
			while( ua ){
				if( ua->Address.lpSockaddr->sa_family != AF_INET6 ){
					ua = ua->Next;
					continue;
				}
				
				sockaddr_in6 * const si = ( sockaddr_in6* )( ua->Address.lpSockaddr );
				char ip[ INET6_ADDRSTRLEN ];
				memset( ip, 0, sizeof( ip ) );
				
				if( ! inet_ntop( AF_INET6, &si->sin6_addr, ip, sizeof( ip ) ) ){
					ua = ua->Next;
					continue;
				}
				
				if( onlyPublic && strcmp( ip, "::1" ) == 0 ){
					ua = ua->Next;
					continue;
				}
				
				list.Add( ip );
				ua = ua->Next;
			}
			
			curAddress = curAddress->Next;
		}
		
		// find IPv8 address
		curAddress = addresses;
		while( curAddress ){
			PIP_ADAPTER_UNICAST_ADDRESS ua = curAddress->FirstUnicastAddress;
			while( ua ){
				if( ua->Address.lpSockaddr->sa_family != AF_INET ){
					ua = ua->Next;
					continue;
				}
				
				sockaddr_in * const si = ( sockaddr_in* )( ua->Address.lpSockaddr );
				char ip[ INET_ADDRSTRLEN ];
				memset( ip, 0, sizeof( ip ) );
				
				if( ! inet_ntop( AF_INET, &si->sin_addr, ip, sizeof( ip ) ) ){
					ua = ua->Next;
					continue;
				}
				
				if( onlyPublic && strcmp( ip, "127.0.0.1" ) == 0 ){
					ua = ua->Next;
					continue;
				}
				
				list.Add( ip );
				ua = ua->Next;
			}
			
			curAddress = curAddress->Next;
		}
		
		HeapFree( GetProcessHeap(), 0, addresses );
		
	}catch( ... ){
		HeapFree( GetProcessHeap(), 0, addresses );
		throw;
	}
	
#else
	ifaddrs *ifaddr, *ifiter;
	char bufferIPv6[ INET6_ADDRSTRLEN ];
	char bufferIPv4[ INET_ADDRSTRLEN ];
	
	if( getifaddrs( &ifaddr ) == -1 ){
		ThrowSocketError( "getifaddrs failed" );
	}
	
	try{
		// find first IPv6 address
		for( ifiter=ifaddr; ifiter; ifiter=ifiter->ifa_next ){
			if( ! ifiter->ifa_addr || ifiter->ifa_addr->sa_family != AF_INET6 ){
				continue;
			}
			
			const in6_addr &saddr = ( ( sockaddr_in6* )ifiter->ifa_addr )->sin6_addr;
			if( ! inet_ntop( AF_INET6, &saddr, bufferIPv6, INET6_ADDRSTRLEN ) ){
				continue;
			}
			
			if( onlyPublic && ( ifiter->ifa_flags & IFF_LOOPBACK ) != 0 ){
				continue;
			}
			
			list.Add( bufferIPv6 );
		}
		
		// then find IPv4 address
		for( ifiter=ifaddr; ifiter; ifiter=ifiter->ifa_next ){
			if( ! ifiter->ifa_addr || ifiter->ifa_addr->sa_family != AF_INET ){
				continue;
			}
			
			const in_addr &saddr = ( ( sockaddr_in* )ifiter->ifa_addr )->sin_addr;
			if( ! inet_ntop( AF_INET, &saddr, bufferIPv4, INET_ADDRSTRLEN ) ){
				continue;
			}
			
			if( onlyPublic && ( ifiter->ifa_flags & IFF_LOOPBACK ) != 0 ){
				continue;
			}
			
			list.Add( bufferIPv4 );
		}
		
		freeifaddrs( ifaddr );
		
	}catch( ... ){
		freeifaddrs( ifaddr );
		throw;
	}
#endif
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
	pNetBasic.UnregisterSocket( this );
	
	if( pSocket != -1 ){
		#ifdef OS_W32
			closesocket( pSocket );
		#else
			close( pSocket );
		#endif
	}
}

uint32_t debnSocket::pScopeIdFor( const sockaddr_in6 &address ){
#ifdef OS_W32
	// get size and allocate buffer
	PIP_ADAPTER_ADDRESSES addresses = ( IP_ADAPTER_ADDRESSES* )HeapAlloc( GetProcessHeap(), 0, 15000 );
	DEASSERT_NOTNULL( addresses )
	
	ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST
		| GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_INCLUDE_PREFIX;
	
	// get the data. this stupid way to do it is actually recommended by microsoft
	ULONG outBufLen = 15000;
	ULONG iterations = 0;
	DWORD dwRetVal = NO_ERROR;
	
	do{
		addresses = ( IP_ADAPTER_ADDRESSES* )HeapAlloc( GetProcessHeap(), 0, outBufLen );
		DEASSERT_NOTNULL( addresses )
		
		dwRetVal = GetAdaptersAddresses( AF_UNSPEC, flags, NULL, addresses, &outBufLen );
		if( dwRetVal == ERROR_BUFFER_OVERFLOW ){
			HeapFree( GetProcessHeap(), 0, addresses );
			addresses = NULL;
			
		}else{
			break;
		}
		
		iterations++;
	}while( dwRetVal == ERROR_BUFFER_OVERFLOW && iterations < 3 );
	
	if( dwRetVal != NO_ERROR ){
		HeapFree( GetProcessHeap(), 0, addresses );
		DETHROW_INFO( deeInvalidAction, "GetAdaptersAddresses failed" );
	}
	
	// evaluate the result
	uint32_t scope = 0;
	bool found = false;
	
	try{
		PIP_ADAPTER_ADDRESSES curAddress = addresses;
		while( curAddress && ! found ){
			PIP_ADAPTER_UNICAST_ADDRESS ua = curAddress->FirstUnicastAddress;
			while( ua && ! found ){
				if( ua->Address.lpSockaddr->sa_family == AF_INET6 ){
					const sockaddr_in6 * const si = ( sockaddr_in6* )( ua->Address.lpSockaddr );
					
					if( memcmp( &si->sin6_addr, &address.sin6_addr, sizeof( address.sin6_addr ) ) == 0 ){
						scope = si->sin6_scope_id;
						found = true;
						break;
					}
				}
				ua = ua->Next;
			}
			
			curAddress = curAddress->Next;
		}
		
		HeapFree( GetProcessHeap(), 0, addresses );
		
	}catch( ... ){
		HeapFree( GetProcessHeap(), 0, addresses );
		throw;
	}
	
	return scope;
	
#else
	ifaddrs *ifaddr, *ifiter;
	
	if( getifaddrs( &ifaddr ) == -1 ){
		ThrowSocketError( "getifaddrs failed" );
	}
	
	uint32_t scope = 0;
	
	try{
		for( ifiter=ifaddr; ifiter; ifiter=ifiter->ifa_next ){
			if( ! ifiter->ifa_addr || ifiter->ifa_addr->sa_family != AF_INET6 ){
				continue;
			}
			
			const sockaddr_in6 &ifa = *( const sockaddr_in6 * )ifiter->ifa_addr;
			
			if( memcmp( &ifa.sin6_addr, &address.sin6_addr, sizeof( address.sin6_addr ) ) == 0 ){
				scope = ( ( sockaddr_in6* )ifiter->ifa_addr )->sin6_scope_id;
				break;
			}
		}
		
		freeifaddrs( ifaddr );
		
	}catch( ... ){
		freeifaddrs( ifaddr );
		throw;
	}
	
	return scope;
#endif
}
