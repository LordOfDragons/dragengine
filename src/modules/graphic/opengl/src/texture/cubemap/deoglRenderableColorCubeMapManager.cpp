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
#include <string.h>
#include <stdlib.h>

#include "deoglRenderableColorCubeMap.h"
#include "deoglRenderableColorCubeMapManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorCubeMapManager
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorCubeMapManager::deoglRenderableColorCubeMapManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pCubeMaps( NULL ),
pCubeMapCount( 0 ),
pCubeMapSize( 0 ){
}

deoglRenderableColorCubeMapManager::~deoglRenderableColorCubeMapManager(){
	if( pCubeMaps ){
		while( pCubeMapCount > 0 ){
			pCubeMapCount--;
			delete pCubeMaps[ pCubeMapCount ];
		}
		delete [] pCubeMaps;
	}
}



// Management
///////////////

const deoglRenderableColorCubeMap *deoglRenderableColorCubeMapManager::GetCubeMapAt( int index ) const{
	if( index < 0 || index >= pCubeMapCount ) DETHROW( deeInvalidParam );
	
	return pCubeMaps[ index ];
}

deoglRenderableColorCubeMap *deoglRenderableColorCubeMapManager::GetCubeMapWith( int size, int componentCount, bool isFloat ){
	deoglRenderableColorCubeMap *cubemap = NULL;
	int i;
	
	// find the cubemap with the matching format
	for( i=0; i<pCubeMapCount; i++ ){
		if( ! pCubeMaps[ i ]->GetInUse() && pCubeMaps[ i ]->Matches( size, componentCount, isFloat ) ){
			cubemap = pCubeMaps[ i ];
			break;
		}
	}
	
	// if not found create a new one
	if( ! cubemap ){
		if( pCubeMapCount == pCubeMapSize ){
			int newSize = pCubeMapSize * 3 / 2 + 1;
			deoglRenderableColorCubeMap **newArray = new deoglRenderableColorCubeMap*[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pCubeMaps ){
				memcpy( newArray, pCubeMaps, sizeof( deoglRenderableColorCubeMap* ) * pCubeMapSize );
				delete [] pCubeMaps;
			}
			pCubeMaps = newArray;
			pCubeMapSize = newSize;
		}
		
		cubemap = new deoglRenderableColorCubeMap( pRenderThread, size, componentCount, isFloat );
		if( ! cubemap ) DETHROW( deeOutOfMemory );
		
		pCubeMaps[ pCubeMapCount ] = cubemap;
		pCubeMapCount++;
	}
	
	// mark the cubemap in use and return it
	cubemap->SetInUse( true );
	return cubemap;
}
