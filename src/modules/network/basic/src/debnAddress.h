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

#ifndef _DEBNADDRESS_H_
#define _DEBNADDRESS_H_

#include <dragengine/dragengine_configuration.h>

#include <dragengine/common/string/decString.h>

#ifdef OS_UNIX
#	include <arpa/inet.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#endif

#ifdef OS_W32
#	ifdef _WIN32_WINNT
#		undef _WIN32_WINNT
#	endif
#	define _WIN32_WINNT _WIN32_WINNT_WIN7
#	include <ws2tcpip.h>
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
	eAddressType pType;
	unsigned char pValues[16];
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
	inline eAddressType GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(eAddressType type);
	
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
	int GetValueAt(int location) const;
	
	/**
	 * \brief Set address value at the given location.
	 * 
	 * The location is measured from the start of the address which is the left most value.
	 */
	void SetValueAt(int location, int value);
	
	/** \brief Port. */
	inline int GetPort() const{ return pPort; }
	
	/** \brief Set port. */
	void SetPort(int port);
	
	/**
	 * \brief Set address to an IPv4 address.
	 * 
	 * The values represent the address values with 0 being the left most value.
	 */
	void SetIPv4(int values[4], int port);
	
	/** \brief Set address to an IPv4 any address. */
	void SetIPv4Any();
	
	/** \brief Set address to an IPv4 loopback address. */
	void SetIPv4Loopback();
	
	/** \brief Set address to an IPv4 address from a socket address. */
	void SetIPv4FromSocket(const sockaddr_in &address);
	
	/** \brief Set address in a socket address. */
	void SetSocketIPv4(sockaddr_in &address) const;
	
	/**
	 * \brief Set IPv6 address.
	 * 
	 * The values represent the address values with 0 being the left most value.
	 */
	void SetIPv6(int values[16], int port);
	
	/** \brief Set IPv6 any address. */
	void SetIPv6Any();
	
	/** \brief Set IPv6 loopback address. */
	void SetIPv6Loopback();
	
	/** \brief Set address to an IPv6 address from a socket address. */
	void SetIPv6FromSocket(const sockaddr_in6 &address);
	
	/** \brief Set address in a socket address. */
	void SetSocketIPv6(sockaddr_in6 &address) const;
	
	/**
	 * \brief Set address to an IPv4/IPv6 address using the provided string.
	 * 
	 * The string can contain a valid IPv4 address, a valid IPv6 address or a valid domain
	 * name with an optional port. If specified the port follows the address separated by
	 * a semicolon. Supported formats are these:
	 * - "[IPv6]:port"
	 * - "IPv6"
	 * - "IPv4:port"
	 * - "IPv4"
	 * - "hostname:port"
	 * - "hostname"
	 */
	void SetFromString(const char *address);
	
	/** \brief Address in string form. */
	decString ToString() const;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assign. */
	debnAddress &operator=(const debnAddress &address);
	
	/** \brief Equals. */
	bool operator==(const debnAddress &address) const;
	/*@}*/
};

#endif
