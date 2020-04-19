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
