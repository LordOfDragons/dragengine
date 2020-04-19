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

#include "deoglRenderableTexture1D.h"
#include "deoglRenderableTexture1DManager.h"
#include "../../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableTexture1DManager
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableTexture1DManager::deoglRenderableTexture1DManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglRenderableTexture1DManager::~deoglRenderableTexture1DManager(){
	int count = pTextures.GetCount();
	while( count > 0 ){
		count--;
		delete ( deoglRenderableTexture1D* )pTextures.GetAt( count );
		pTextures.RemoveFrom( count );
	}
}



// Management
///////////////

int deoglRenderableTexture1DManager::GetTextureCount() const{
	return pTextures.GetCount();
}

const deoglRenderableTexture1D *deoglRenderableTexture1DManager::GetTextureAt( int index ) const{
	return ( deoglRenderableTexture1D* )pTextures.GetAt( index );
}

deoglRenderableTexture1D *deoglRenderableTexture1DManager::GetTextureWith( int size, int format ){
	// find the texture with the matching format
	deoglRenderableTexture1D *texture = NULL;
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		texture = ( deoglRenderableTexture1D* )pTextures.GetAt( i );
		if( ! texture->GetInUse() && texture->Matches( size, format ) ){
			break;
		}
	}
	
	// if not found create a new one
	if( i == count ){
		texture = new deoglRenderableTexture1D( pRenderThread, size, format );
		pTextures.Add( texture );
	}
	
	// mark the texture in use and return it
	texture->SetInUse( true );
	return texture;
}
