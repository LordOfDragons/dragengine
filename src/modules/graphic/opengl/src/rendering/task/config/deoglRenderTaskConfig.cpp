/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <string.h>

#include "deoglRenderTaskConfig.h"
#include "deoglRenderTaskConfigTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskConfig
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskConfig::deoglRenderTaskConfig() :
pTextures( NULL ),
pTextureCount( 0 ),
pTextureSize( 0 ){
}

deoglRenderTaskConfig::~deoglRenderTaskConfig(){
	if( pTextures ){
		delete [] pTextures;
	}
}



// Management
///////////////

deoglRenderTaskConfigTexture &deoglRenderTaskConfig::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	return pTextures[ index ];
}

deoglRenderTaskConfigTexture &deoglRenderTaskConfig::AddTexture(){
	if( pTextureCount == pTextureSize ){
		const int newSize = pTextureCount * 3 / 2 + 1;
		deoglRenderTaskConfigTexture * const newArray = new deoglRenderTaskConfigTexture[ newSize ];
		if( pTextures ){
			memcpy( newArray, pTextures, sizeof( deoglRenderTaskConfigTexture ) * pTextureCount );
			delete [] pTextures;
		}
		pTextures = newArray;
		pTextureSize = newSize;
	}
	
	deoglRenderTaskConfigTexture &texture = pTextures[ pTextureCount++ ];
	texture.Clear();
	return texture;
}

void deoglRenderTaskConfig::RemoveAllTextures(){
	pTextureCount = 0;
}
