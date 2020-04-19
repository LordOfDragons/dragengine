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

#ifndef _DEBNADDRESS_H_
#define _DEBNADDRESS_H_

#include <dragengine/common/string/decString.h>
#ifdef OS_UNIX
#	include <arpa/inet.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#endif
#ifdef OS_W32
#	include <dragengine/app/include_windows.h>
#endif



/**
 * \brief Address class.
 * 
 * Stores a computer address. The address can be currently only a IPv4
 * address but IPv6 might follow. Provides support to obtain an address
 * from an address string doing proper DNS lookup if required as well
 * as extracting the port from an address if specified. In addition
 * provides support to fill a socket address or to obtain an address
 * from a socket address.
 */
class debnAddress{
public:
	/** \brief Type of the address. */
	enum eAddressType{
		/** \brief IPv4 address. */
		eatIPv4,
		
		/** \brief IPv6 address. */
		eatIPv6
	};
	
	
	
private:
	int pType;
	unsigned char pValues[ 4 ];
	int pValueCount;
	int pPort;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create address. */
	debnAddress();
	
	/** \brief Clean up address. */
	~debnAddress();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline int GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( int type );
	
	/**
	 * \brief Number of address values.
	 * 
	 * Depends on the type of the address.
	 */
	inline int GetValueCount() const{ return pValueCount; }
	
	/**
	 * \brief Address value at the given location.
	 * 
	 * The location is  measured from the start of the address which is the left most value.
	 */
	int GetValueAt( int location ) const;
	
	/**
	 * \brief Set address value at the given location.
	 * 
	 * The location is measured from the start of the address which is the left most value.
	 */
	void SetValueAt( int location, int value );
	
	/** \brief Port. */
	inline int GetPort() const{ return pPort; }
	
	/** \brief Set port. */
	void SetPort( int port );
	
	/**
	 * \brief Set address to an IPv4 address.
	 * 
	 * The values represent the address values with 0 being the left most value.
	 */
	void SetIPv4( int values[ 4 ], int port );
	
	/** \brief Set address to an IPv4 any address. */
	void SetIPv4Any();
	
	/** \brief Set address to an IPv4 loopback address. */
	void SetIPv4Loopback();
	
	/**
	 * \brief Set address to an IPv4 address using the provided string.
	 * 
	 * The string can contain a valid IPv4 address or a valid domain name with
	 * an optional port. If specified the port follows the address separated
	 * by a semicolon.
	 */
	void SetIPv4FromString( const char *address );
	
	/** \brief Set address to an IPv4 address from a socket address. */
	void SetIPv4FromSocket( const struct sockaddr_in &address );
	
	/** \brief Set address in a socket address. */
	void SetSocketIPv4( struct sockaddr_in &address ) const;
	
	/** \brief Address in string form. */
	decString ToString() const;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assign. */
	debnAddress &operator=( const debnAddress &address );
	
	/** \brief Equals. */
	bool operator==( const debnAddress &address ) const;
	/*@}*/
};

#endif
