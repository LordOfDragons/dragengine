/* 
 * FBX Modules
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

#ifndef _FBXCONNECTIONMAP_H_
#define _FBXCONNECTIONMAP_H_


#include <stdint.h>

#include <dragengine/common/collection/decPointerList.h>


class fbxConnection;


/**
 * \brief FBX map storing connections keyed by source and target ID.
 * 
 * In detail this means the connection is entered into the map twice once using the
 * source id and once using the target id. For this two maps are stored one for source
 * and one for target. This works since source and target id never can be identical.
 * This allows to find all connections matching an id no matter if it is the source or
 * the target in a connection.
 */
class fbxConnectionMap{
private:
	struct sBucket{
		decPointerList connections;
	};
	
	sBucket *pBucketsSource;
	sBucket *pBucketsTarget;
	int pBucketCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection id map. */
	fbxConnectionMap( int expectedCount );
	
	/** \brief Clean up connection id map. */
	~fbxConnectionMap();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Add connection. */
	void Add( fbxConnection *connection );
	
	/** \brief Add connections matching ID to list. */
	void Get( int64_t id, decPointerList &list ) const;
	/*@}*/
};

#endif
