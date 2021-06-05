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

#include "deoglRenderableColorArrayTexture.h"
#include "deoglRenderableColorArrayTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorArrayTextureManager
//////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorArrayTextureManager::deoglRenderableColorArrayTextureManager(
	deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pArrayTextures( NULL ),
pArrayTextureCount( 0 ),
pArrayTextureSize( 0 ){
}

deoglRenderableColorArrayTextureManager::~deoglRenderableColorArrayTextureManager(){
	if( pArrayTextures ){
		while( pArrayTextureCount > 0 ){
			pArrayTextureCount--;
			delete pArrayTextures[ pArrayTextureCount ];
		}
		delete [] pArrayTextures;
	}
}



// Management
///////////////

const deoglRenderableColorArrayTexture *deoglRenderableColorArrayTextureManager::GetAt( int index ) const{
	if( index < 0 || index >= pArrayTextureCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pArrayTextures[ index ];
}

deoglRenderableColorArrayTexture *deoglRenderableColorArrayTextureManager::GetWith(
int width, int height, int layerCount, int componentCount, bool isFloat ){
	deoglRenderableColorArrayTexture *texture = NULL;
	int i;
	
	// find the texture with the matching format
	for( i=0; i<pArrayTextureCount; i++ ){
		if( ! pArrayTextures[ i ]->GetInUse() && pArrayTextures[ i ]->Matches(
		width, height, layerCount, componentCount, isFloat ) ){
			texture = pArrayTextures[ i ];
			break;
		}
	}
	
	// if not found create a new one
	if( ! texture ){
		if( pArrayTextureCount == pArrayTextureSize ){
			int newSize = pArrayTextureSize * 3 / 2 + 1;
			deoglRenderableColorArrayTexture **newArray = new deoglRenderableColorArrayTexture*[ newSize ];
			
			if( pArrayTextures ){
				memcpy( newArray, pArrayTextures, sizeof( deoglRenderableColorArrayTexture* ) * pArrayTextureSize );
				delete [] pArrayTextures;
			}
			
			pArrayTextures = newArray;
			pArrayTextureSize = newSize;
		}
		
		texture = new deoglRenderableColorArrayTexture( pRenderThread,
			width, height, layerCount, componentCount, isFloat );
		
		pArrayTextures[ pArrayTextureCount ] = texture;
		pArrayTextureCount++;
	}
	
	// mark the texture in use and return it
	texture->SetInUse( true );
	return texture;
}
