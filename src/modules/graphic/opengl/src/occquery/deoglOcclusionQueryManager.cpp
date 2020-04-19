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

#include "deoglOcclusionQuery.h"
#include "deoglOcclusionQueryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionQueryManager
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionQueryManager::deoglOcclusionQueryManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	pQueries = NULL;
	pQueryCount = 0;
	pQuerySize = 0;
	
	pFreeQueryCount = 0;
	
	pActiveQuery = NULL;
}

deoglOcclusionQueryManager::~deoglOcclusionQueryManager(){
	if( pQueries ){
		while( pQueryCount > 0 ){
			pQueryCount--;
			delete pQueries[ pQueryCount ];
		}
		
		delete [] pQueries;
	}
}



// Management
///////////////

deoglOcclusionQuery *deoglOcclusionQueryManager::ClaimQuery(){
	if( pFreeQueryCount == 0 ){
		if( pQueryCount == pQuerySize ){
			int newSize = pQuerySize + 10;
			deoglOcclusionQuery **newArray = new deoglOcclusionQuery*[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pQueries ){
				memcpy( newArray, pQueries, sizeof( deoglOcclusionQuery* ) * pQuerySize );
				delete [] pQueries;
			}
			memset( newArray + pQuerySize, '\0', sizeof( deoglOcclusionQuery* ) * ( newSize - pQuerySize ) );
			pQueries = newArray;
			pQuerySize = newSize;
		}
		
		if( ! pQueries[ pQueryCount ] ){
			pQueries[ pQueryCount ] = new deoglOcclusionQuery( pRenderThread );
			if( ! pQueries[ pQueryCount ] ) DETHROW( deeOutOfMemory );
		}
		
		return pQueries[ pQueryCount++ ];
		
	}else{
		pFreeQueryCount--;
		return pQueries[ pFreeQueryCount ];
	}
}

void deoglOcclusionQueryManager::ReleaseQuery( deoglOcclusionQuery *query ){
	if( ! query ) DETHROW( deeInvalidParam );
	
	int index = IndexOfQuery( query, pFreeQueryCount, pQueryCount );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( index > pFreeQueryCount ){
		deoglOcclusionQuery *exchange = pQueries[ index ];
		pQueries[ index ] = pQueries[ pFreeQueryCount ];
		pQueries[ pFreeQueryCount ] = exchange;
	}
	
	pFreeQueryCount++;
}

void deoglOcclusionQueryManager::SetActiveQuery( deoglOcclusionQuery *query ){
	pActiveQuery = query;
}

void deoglOcclusionQueryManager::EndActiveQuery(){
	if( pActiveQuery ){
		pActiveQuery->EndQuery();
	}
}



// Private Functions
//////////////////////

int deoglOcclusionQueryManager::IndexOfQuery( deoglOcclusionQuery *query, int from, int to ) const{
	int i;
	
	for( i=from; i<to; i++ ){
		if( pQueries[ i ] == query ){
			return i;
		}
	}
	
	return -1;
}
