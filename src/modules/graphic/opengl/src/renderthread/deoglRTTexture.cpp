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

#include "deoglRTTexture.h"
#include "deoglRenderThread.h"
#include "../skin/combinedTexture/deoglCombinedTextureList.h"
#include "../texture/arraytexture/deoglRenderableArrayTextureManager.h"
#include "../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture1d/deoglRenderableTexture1DManager.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTTexture
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTTexture::deoglRTTexture( deoglRenderThread &renderThread ) :
pTextureStageManager( NULL ),
pCombinedTextureList( NULL ),
pRenColorTexMgr( NULL ),
pRenDepthTexMgr( NULL ),
pRenColorCubeMgr( NULL ),
pRenDepthCubeMgr( NULL ),
pRenTex1DMgr( NULL ),
pRenArrTexMgr( NULL )
{
	try{
		pTextureStageManager = new deoglTextureStageManager( renderThread );
		pCombinedTextureList = new deoglCombinedTextureList( renderThread );
		
		pRenColorTexMgr = new deoglRenderableColorTextureManager( renderThread );
		pRenDepthTexMgr = new deoglRenderableDepthTextureManager( renderThread );
		pRenColorCubeMgr = new deoglRenderableColorCubeMapManager( renderThread );
		pRenDepthCubeMgr = new deoglRenderableDepthCubeMapManager( renderThread );
		pRenTex1DMgr = new deoglRenderableTexture1DManager( renderThread );
		pRenArrTexMgr = new deoglRenderableArrayTextureManager( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTTexture::~deoglRTTexture(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglRTTexture::pCleanUp(){
	if( pRenArrTexMgr ){
		delete pRenArrTexMgr;
	}
	if( pRenTex1DMgr ){
		delete pRenTex1DMgr;
	}
	if( pRenColorCubeMgr ){
		delete pRenColorCubeMgr;
	}
	if( pRenDepthCubeMgr ){
		delete pRenDepthCubeMgr;
	}
	if( pRenColorTexMgr ){
		delete pRenColorTexMgr;
	}
	if( pRenDepthTexMgr ){
		delete pRenDepthTexMgr;
	}
	
	if( pCombinedTextureList ){
		delete pCombinedTextureList;
	}
	if( pTextureStageManager ){
		delete pTextureStageManager;
	}
}
