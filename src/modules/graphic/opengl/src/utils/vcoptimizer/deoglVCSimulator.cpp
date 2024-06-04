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

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "deoglVCSimulator.h"

#include <dragengine/common/exceptions.h>



// Class deoglVCSimulator
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVCSimulator::deoglVCSimulator(){
	pCacheHitCount = 0;
	pCacheMissCount = 0;
	pFaceCount = 0;
	
	pCacheSize = 32;
	pCache = new int[ pCacheSize ];
}

deoglVCSimulator::~deoglVCSimulator(){
	if( pCache ){
		delete [] pCache;
	}
}



// Management
///////////////

void deoglVCSimulator::SetCacheSize( int cacheSize ){
	if( cacheSize < 4 ){
		DETHROW( deeInvalidParam );
	}
	
	int *cache = new int[ cacheSize ];
	
	if( pCache ){
		delete [] pCache;
	}
	
	pCache = cache;
	pCacheSize = cacheSize;
}



void deoglVCSimulator::Reset(){
	int i;
	
	for( i=0; i<pCacheSize; i++ ){
		pCache[ i ] = -1;
	}
	
	pCacheHitCount = 0;
	pCacheMissCount = 0;
	pFaceCount = 0;
}

void deoglVCSimulator::ProcessFace( int vertex1, int vertex2, int vertex3 ){
	if( vertex1 < 0 || vertex2 < 0 || vertex3 < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pFaceCount++;
	TestVertexAgainstCache( vertex1 );
	TestVertexAgainstCache( vertex2 );
	TestVertexAgainstCache( vertex3 );
}



float deoglVCSimulator::GetAvgCacheMissRatio() const{
	if( pFaceCount == 0 ){
		return 0.0f;
		
	}else{
		return ( float )pCacheMissCount / ( float )pFaceCount;
	}
}



void deoglVCSimulator::TestVertexAgainstCache( int vertex ){
	int i;
	
	for( i=0; i<pCacheSize; i++ ){
		if( pCache[ i ] == vertex ){
			pCacheHitCount++;
			return;
		}
	}
	
	pCacheMissCount++;
	for( i=pCacheSize-1; i>0; i-- ){
		pCache[ i ] = pCache[ i - 1 ];
	}
	pCache[ 0 ] = vertex;
}
