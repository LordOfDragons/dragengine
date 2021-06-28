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

#include "deoglRenderableDepthArrayTexture.h"
#include "deoglRenderableDepthArrayTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthArrayTextureManager
//////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthArrayTextureManager::deoglRenderableDepthArrayTextureManager(
	deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pArrayTextures( NULL ),
pArrayTextureCount( 0 ),
pArrayTextureSize( 0 ){
}

deoglRenderableDepthArrayTextureManager::~deoglRenderableDepthArrayTextureManager(){
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

const deoglRenderableDepthArrayTexture *deoglRenderableDepthArrayTextureManager::GetAt( int index ) const{
	if( index < 0 || index >= pArrayTextureCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pArrayTextures[ index ];
}

deoglRenderableDepthArrayTexture *deoglRenderableDepthArrayTextureManager::GetWith(
int width, int height, int layerCount, bool withStencil, bool useFloat ){
	deoglRenderableDepthArrayTexture *texture = NULL;
	int i;
	
	// find the texture with the matching format
	for( i=0; i<pArrayTextureCount; i++ ){
		if( ! pArrayTextures[ i ]->GetInUse() && pArrayTextures[ i ]->Matches(
		width, height, layerCount, withStencil, useFloat ) ){
			texture = pArrayTextures[ i ];
			break;
		}
	}
	
	// if not found create a new one
	if( ! texture ){
		if( pArrayTextureCount == pArrayTextureSize ){
			int newSize = pArrayTextureSize * 3 / 2 + 1;
			deoglRenderableDepthArrayTexture **newArray = new deoglRenderableDepthArrayTexture*[ newSize ];
			if( pArrayTextures ){
				memcpy( newArray, pArrayTextures, sizeof( deoglRenderableDepthArrayTexture* ) * pArrayTextureSize );
				delete [] pArrayTextures;
			}
			pArrayTextures = newArray;
			pArrayTextureSize = newSize;
		}
		
		texture = new deoglRenderableDepthArrayTexture( pRenderThread,
			width, height, layerCount, withStencil, useFloat );
		pArrayTextures[ pArrayTextureCount ] = texture;
		pArrayTextureCount++;
	}
	
	// mark the texture in use and return it
	texture->SetInUse( true );
	return texture;
}
