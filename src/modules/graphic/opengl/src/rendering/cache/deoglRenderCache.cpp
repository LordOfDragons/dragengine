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

#include "deoglRenderCache.h"
#include "deoglRenderCacheLight.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../light/deoglRLight.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableColorCubeMap.h"
#include "../../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableColorTexture.h"
#include "../../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderCache
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCache::deoglRenderCache( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pLights( NULL ),
pLightCount( 0 ),
pLightSize( 0 ){
}

deoglRenderCache::~deoglRenderCache(){
	if( pLights ){
		while( pLightSize > 0 ){
			pLightSize--;
			if( pLights[ pLightSize ] ){
				delete pLights[ pLightSize ];
			}
		}
		
		delete [] pLights;
	}
}



// Management
///////////////

void deoglRenderCache::Clear(){
	RemoveAllLights();
}



// Lights
///////////

deoglRenderCacheLight *deoglRenderCache::GetLightAt( int index ) const{
	if( index < 0 || index >= pLightCount ) DETHROW( deeInvalidParam );
	
	return pLights[ index ];
}

deoglRenderCacheLight *deoglRenderCache::GetLightFor( deoglRLight *light ){
	if( ! light ) DETHROW( deeInvalidParam );
	
	int index = pIndexOfLightWith( light );
	if( index == -1 ){
		if( pLightCount == pLightSize ){
			int newSize = pLightSize * 3 / 2 + 1;
			deoglRenderCacheLight **newArray = new deoglRenderCacheLight*[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			
			memset( newArray, '\0', sizeof( deoglRenderCacheLight* ) * newSize );
			if( pLights ){
				memcpy( newArray, pLights, sizeof( deoglRenderCacheLight* ) * pLightSize );
				delete [] pLights;
			}
			
			pLights = newArray;
			pLightSize = newSize;
		}
		
		if( ! pLights[ pLightCount ] ){
			pLights[ pLightCount ] = new deoglRenderCacheLight( pRenderThread );
			if( ! pLights[ pLightCount ] ) DETHROW( deeOutOfMemory );
		}
		
		index = pLightCount;
		pLights[ pLightCount ]->SetLight( light );
		pLightCount++;
	}
	
	return pLights[ index ];
}

void deoglRenderCache::RemoveAllLights(){
	while( pLightCount > 0 ){
		pLightCount--;
		pLights[ pLightCount ]->SetLight( NULL );
	}
}



// Private Functions
//////////////////////

int deoglRenderCache::pIndexOfLightWith( deoglRLight *light ) const{
	int i;
	
	for( i=0; i<pLightCount; i++ ){
		if( light == pLights[ i ]->GetLight() ){
			return i;
		}
	}
	
	return -1;
}
