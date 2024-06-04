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
