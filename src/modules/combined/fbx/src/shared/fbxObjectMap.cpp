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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "fbxObjectMap.h"
#include "fbxNode.h"

#include <dragengine/common/exceptions.h>


// Class fbxObjectMap
///////////////////////

// Constructor, destructor
////////////////////////////

fbxObjectMap::fbxObjectMap( int expectedCount ) :
pBuckets( NULL ),
pBucketCount( 0 )
{
	if( expectedCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = decMath::max( ( int )( ( float )expectedCount * 0.75f ), 1 );
	pBuckets = new sBucket[ count ];
	pBucketCount = count;
}

fbxObjectMap::~fbxObjectMap(){
	if( pBuckets ){
		delete [] pBuckets;
	}
}



// Management
///////////////

void fbxObjectMap::Add( fbxNode *node ){
	if( ! node || node->GetID() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pBuckets[ node->GetID() % pBucketCount ].objects.Add( node );
}

fbxNode *fbxObjectMap::GetAt( int64_t id ) const{
	const decPointerList &objects = pBuckets[ id % pBucketCount ].objects;
	const int count = objects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		fbxNode * const node = ( fbxNode* )objects.GetAt( i );
		if( node->GetID() == id ){
			return node;
		}
	}
	
	return NULL;
}
