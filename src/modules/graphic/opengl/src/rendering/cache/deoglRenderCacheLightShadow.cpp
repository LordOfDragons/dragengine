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

#include "deoglRenderCacheLightShadow.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
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



// Class deoglRenderCacheLightShadow
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCacheLightShadow::deoglRenderCacheLightShadow( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	pSize = 0;
	pMemoryConsumption = 0;
	
	pMapDepth = NULL;
	pMapDepthEncoded = NULL;
	pCubeDepth = NULL;
	pCubeDepthEncoded = NULL;
	
	pMapColor = NULL;
	pCubeColor = NULL;
}

deoglRenderCacheLightShadow::~deoglRenderCacheLightShadow(){
	pDropTextures();
}



// Management
///////////////

void deoglRenderCacheLightShadow::SetSize( int size ){
	if( size < 0 ) DETHROW( deeInvalidParam );
	
	if( size != pSize ){
		pSize = size;
		
		pDropTextures();
	}
}

void deoglRenderCacheLightShadow::SetMemoryConsumption( int consumption ){
	if( consumption < 0 ) DETHROW( deeInvalidParam );
	
	pMemoryConsumption = consumption;
}



deoglTexture *deoglRenderCacheLightShadow::GetMapDepth(){
	if( pSize == 0 ) DETHROW( deeInvalidParam );
	
	if( ! pMapDepth ){
		pMapDepth = pRenderThread.GetTexture().GetRenderableDepthTexture()
			.GetTextureWith( pSize, pSize, false, false );
	}
	
	return pMapDepth->GetTexture();
}

deoglTexture *deoglRenderCacheLightShadow::GetMapDepthEncoded(){
	if( pSize == 0 ) DETHROW( deeInvalidParam );
	
	if( ! pMapDepthEncoded ){
		pMapDepthEncoded = pRenderThread.GetTexture().GetRenderableColorTexture()
			.GetTextureWith( pSize, pSize, 4, false );
	}
	
	return pMapDepthEncoded->GetTexture();
}

deoglCubeMap *deoglRenderCacheLightShadow::GetCubeDepth(){
	if( pSize == 0 ) DETHROW( deeInvalidParam );
	
	if( ! pCubeDepth ){
		pCubeDepth = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( pSize );
	}
	
	return pCubeDepth->GetCubeMap();
}

deoglCubeMap *deoglRenderCacheLightShadow::GetCubeDepthEncoded(){
	if( pSize == 0 ) DETHROW( deeInvalidParam );
	
	if( ! pCubeDepthEncoded ){
		pCubeDepthEncoded = pRenderThread.GetTexture().GetRenderableColorCubeMap()
			.GetCubeMapWith( pSize, 4, false );
	}
	
	return pCubeDepthEncoded->GetCubeMap();
}



deoglTexture *deoglRenderCacheLightShadow::GetMapColor(){
	if( pSize == 0 ) DETHROW( deeInvalidParam );
	
	if( ! pMapColor ){
		pMapColor = pRenderThread.GetTexture().GetRenderableColorTexture().GetTextureWith( pSize, pSize, 4, false );
	}
	
	return pMapColor->GetTexture();
}

deoglCubeMap *deoglRenderCacheLightShadow::GetCubeColor(){
	if( pSize == 0 ) DETHROW( deeInvalidParam );
	
	if( ! pCubeColor ){
		pCubeColor = pRenderThread.GetTexture().GetRenderableColorCubeMap().GetCubeMapWith( pSize, 4, false );
	}
	
	return pCubeColor->GetCubeMap();
}



// Private Functions
//////////////////////

void deoglRenderCacheLightShadow::pDropTextures(){
	if( pMapDepth ){
		pMapDepth->SetInUse( false );
		pMapDepth = NULL;
	}
	if( pMapDepthEncoded ){
		pMapDepthEncoded->SetInUse( false );
		pMapDepthEncoded = NULL;
	}
	if( pCubeDepth ){
		pCubeDepth->SetInUse( false );
		pCubeDepth = NULL;
	}
	if( pCubeDepthEncoded ){
		pCubeDepthEncoded->SetInUse( false );
		pCubeDepthEncoded = NULL;
	}
	
	if( pMapColor ){
		pMapColor->SetInUse( false );
		pMapColor = NULL;
	}
	if( pCubeColor ){
		pCubeColor->SetInUse( false );
		pCubeColor = NULL;
	}
}
