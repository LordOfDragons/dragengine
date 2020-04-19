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

#include "deoglSkyLayerShadowCaster.h"
#include "../texture/arraytexture/deoglArrayTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkyLayerShadowCaster
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkyLayerShadowCaster::deoglSkyLayerShadowCaster( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pLayers( NULL ),
pLayerCount( 0 ),
pSolidShadowMap( NULL ){
}

deoglSkyLayerShadowCaster::~deoglSkyLayerShadowCaster(){
	DropSolidShadowMap();
	if( pLayers ){
		delete [] pLayers;
	}
}



// Management
///////////////

void deoglSkyLayerShadowCaster::SetLayerCount( int layerCount ){
	if( layerCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	DropSolidShadowMap();
	
	if( pLayers ){
		delete [] pLayers;
		pLayers = NULL;
		pLayerCount = 0;
	}
	
	if( layerCount > 0 ){
		pLayers = new sLayer[ layerCount ];
		pLayerCount = layerCount;
	}
}

deoglArrayTexture* deoglSkyLayerShadowCaster::GetSolidShadowMap( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSolidShadowMap ){
		if( pSolidShadowMap->GetWidth() == size && pSolidShadowMap->GetHeight() == size
		&& pSolidShadowMap->GetLayerCount() == pLayerCount ){
			return pSolidShadowMap;
		}
		
		DropSolidShadowMap();
	}
	
	pSolidShadowMap = new deoglArrayTexture( pRenderThread );
	pSolidShadowMap->SetSize( size, size, pLayerCount );
	pSolidShadowMap->SetDepthFormat( false );
	pSolidShadowMap->CreateTexture();
	
	return pSolidShadowMap;
}

void deoglSkyLayerShadowCaster::DropSolidShadowMap(){
	if( pSolidShadowMap ){
		delete pSolidShadowMap;
		pSolidShadowMap = NULL;
	}
}
