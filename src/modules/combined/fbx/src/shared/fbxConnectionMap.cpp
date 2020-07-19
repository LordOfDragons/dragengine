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

#include "fbxConnectionMap.h"
#include "fbxConnection.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>


// Class fbxConnectionMap
///////////////////////////

// Constructor, destructor
////////////////////////////

fbxConnectionMap::fbxConnectionMap( int expectedCount ) :
pBucketsSource( NULL ),
pBucketsTarget( NULL ),
pBucketCount( 0 )
{
	if( expectedCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = decMath::max( ( int )( ( float )expectedCount * 0.75f ), 1 );
	pBucketsSource = new sBucket[ count ];
	pBucketsTarget = new sBucket[ count ];
	pBucketCount = count;
}

fbxConnectionMap::~fbxConnectionMap(){
	if( pBucketsTarget ){
		delete [] pBucketsTarget;
	}
	if( pBucketsSource ){
		delete [] pBucketsSource;
	}
}



// Management
///////////////

void fbxConnectionMap::Add( fbxConnection *connection ){
	if( ! connection ){
		DETHROW( deeInvalidParam );
	}
	
	if( connection->GetSource() != 0 ){
		pBucketsSource[ connection->GetSource() % pBucketCount ].connections.Add( connection );
	}
	if( connection->GetTarget() != 0 ){
		pBucketsTarget[ connection->GetTarget() % pBucketCount ].connections.Add( connection );
	}
}

void fbxConnectionMap::Get( int64_t id, decPointerList &list ) const{
	const decPointerList &consSource = pBucketsSource[ id % pBucketCount ].connections;
	const decPointerList &consTarget = pBucketsTarget[ id % pBucketCount ].connections;
	const int consSourceCount = consSource.GetCount();
	const int consTargetCount = consTarget.GetCount();
	int i;
	
	for( i=0; i<consSourceCount; i++ ){
		fbxConnection * const connection = ( fbxConnection* )consSource.GetAt( i );
		if( connection->GetSource() == id ){
			list.Add( connection );
		}
	}
	
	for( i=0; i<consTargetCount; i++ ){
		fbxConnection * const connection = ( fbxConnection* )consTarget.GetAt( i );
		if( connection->GetTarget() == id ){
			list.Add( connection );
		}
	}
}
