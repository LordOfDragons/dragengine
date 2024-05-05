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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debpPointSieve.h"
#include "debpPointSieveBucket.h"

#include <dragengine/common/exceptions.h>



// Class debpPointSieve
/////////////////////////

// Constructor, destructor
////////////////////////////

debpPointSieve::debpPointSieve( int bucketCountX, int bucketCountZ, float sieveSizeX, float sieveSizeZ ){
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
	
	pBuckets = new debpPointSieveBucket[ pBucketCount ];
	if( ! pBuckets ) DETHROW( deeOutOfMemory );
}

debpPointSieve::~debpPointSieve(){
	if( pBuckets ) delete [] pBuckets;
}



// Management
///////////////

debpPointSieveBucket &debpPointSieve::GetBucketAt( int index ) const{
	if( index < 0 || index >= pBucketCount ) DETHROW( deeInvalidParam );
	
	return pBuckets[ index ];
}

debpPointSieveBucket &debpPointSieve::GetBucketWith( int x, int z ) const{
	if( x < 0 || x >= pBucketCountX || z < 0 || z >= pBucketCountZ ) DETHROW( deeInvalidParam );
	
	return pBuckets[ pBucketCountX * z + x ];
}

debpPointSieveBucket &debpPointSieve::GetBucketContaining( float x, float z ) const{
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



void debpPointSieve::DropPoint( float x, float z, int index ){
	GetBucketContaining( x, z ).AddIndex( index );
}



void debpPointSieve::Clear(){
	int b;
	
	for( b=0; b<pBucketCount; b++ ){
		pBuckets[ b ].RemoveAllIndices();
	}
}
