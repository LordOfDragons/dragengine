/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglPointSieve.h"
#include "deoglPointSieveBucket.h"

#include <dragengine/common/exceptions.h>



// Class deoglPointSieve
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglPointSieve::deoglPointSieve( int bucketCountX, int bucketCountZ, float sieveSizeX, float sieveSizeZ ){
	if( bucketCountX < 1 || bucketCountZ < 1 || sieveSizeX <= 0.0f || sieveSizeZ <= 0.0f ) DETHROW( deeInvalidParam );
	
	pSieveSizeX = sieveSizeX;
	pSieveSizeZ = sieveSizeZ;
	pSieveLeft = -sieveSizeX * 0.5f;
	pSieveTop = sieveSizeZ * 0.5f;
	pSieveDivX = ( float )bucketCountX / sieveSizeX;
	pSieveDivZ = ( float )bucketCountZ / sieveSizeZ;
	
	pBuckets = NULL;
	pBucketCountX = bucketCountX;
	pBucketCountZ = bucketCountZ;
	pBucketCount = bucketCountX * bucketCountZ;
	
	pBuckets = new deoglPointSieveBucket[ pBucketCount ];
	if( ! pBuckets ) DETHROW( deeOutOfMemory );
}

deoglPointSieve::~deoglPointSieve(){
	if( pBuckets ) delete [] pBuckets;
}



// Management
///////////////

deoglPointSieveBucket &deoglPointSieve::GetBucketAt( int index ) const{
	if( index < 0 || index >= pBucketCount ) DETHROW( deeInvalidParam );
	
	return pBuckets[ index ];
}

deoglPointSieveBucket &deoglPointSieve::GetBucketWith( int x, int z ) const{
	if( x < 0 || x >= pBucketCountX || z < 0 || z >= pBucketCountZ ) DETHROW( deeInvalidParam );
	
	return pBuckets[ pBucketCountX * z + x ];
}

deoglPointSieveBucket &deoglPointSieve::GetBucketContaining( float x, float z ) const{
	int bx = ( int )( ( x - pSieveLeft ) * pSieveDivX );
	int bz = ( int )( ( pSieveTop - z ) * pSieveDivZ );
	
	if( bx < 0 ){
		bx = 0;
		
	}else if( bx >= pBucketCountX ){
		bx = pBucketCountX - 1;
	}
	
	if( bz < 0 ){
		bz = 0;
		
	}else if( bz >= pBucketCountZ ){
		bz = pBucketCountZ - 1;
	}
	
	return pBuckets[ pBucketCountX * bz + bx ];
}



void deoglPointSieve::DropPoint( float x, float z, int index ){
	GetBucketContaining( x, z ).AddIndex( index );
}



void deoglPointSieve::Clear(){
	int b;
	
	for( b=0; b<pBucketCount; b++ ){
		pBuckets[ b ].RemoveAllIndices();
	}
}
