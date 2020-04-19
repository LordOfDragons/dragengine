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

#include "deoglRenderableColorTexture.h"
#include "deoglRenderableColorTextureManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorTextureManager
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorTextureManager::deoglRenderableColorTextureManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pTextures( NULL ),
pTextureCount( 0 ),
pTextureSize( 0 ){
}

deoglRenderableColorTextureManager::~deoglRenderableColorTextureManager(){
	if( pTextures ){
		while( pTextureCount > 0 ){
			pTextureCount--;
			delete pTextures[ pTextureCount ];
		}
		delete [] pTextures;
	}
}



// Management
///////////////

const deoglRenderableColorTexture *deoglRenderableColorTextureManager::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextures[ index ];
}

deoglRenderableColorTexture *deoglRenderableColorTextureManager::GetTextureWith( int width, int height, int componentCount, bool isFloat ){
	deoglRenderableColorTexture *texture = NULL;
	int i;
	
	// find the texture with the matching format
	for( i=0; i<pTextureCount; i++ ){
		if( ! pTextures[ i ]->GetInUse() && pTextures[ i ]->Matches( width, height, componentCount, isFloat ) ){
			texture = pTextures[ i ];
			break;
		}
	}
	
	// if not found create a new one
	if( ! texture ){
		if( pTextureCount == pTextureSize ){
			int newSize = pTextureSize * 3 / 2 + 1;
			deoglRenderableColorTexture **newArray = new deoglRenderableColorTexture*[ newSize ];
			
			if( pTextures ){
				memcpy( newArray, pTextures, sizeof( deoglRenderableColorTexture* ) * pTextureSize );
				delete [] pTextures;
			}
			
			pTextures = newArray;
			pTextureSize = newSize;
		}
		
		texture = new deoglRenderableColorTexture( pRenderThread, width, height, componentCount, isFloat );
		
		pTextures[ pTextureCount ] = texture;
		pTextureCount++;
	}
	
	// mark the texture in use and return it
	texture->SetInUse( true );
	return texture;
}
