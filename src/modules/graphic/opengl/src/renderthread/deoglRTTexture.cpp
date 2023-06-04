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
#include "../occlusiontest/deoglOcclusionMapPool.h"
#include "../skin/combinedTexture/deoglCombinedTextureList.h"
#include "../texture/arraytexture/deoglRenderableColorArrayTextureManager.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/deoglImageStageManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTTexture
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTTexture::deoglRTTexture( deoglRenderThread &renderThread ) :
pTextureStageManager( NULL ),
pImageStageManager( NULL ),
pCombinedTextureList( NULL ),
pRenColorTexMgr( NULL ),
pRenDepthTexMgr( NULL ),
pRenColorCubeMgr( NULL ),
pRenDepthCubeMgr( NULL ),
pRenColorArrTexMgr( NULL ),
pRenDepthArrTexMgr( NULL ),
pOcclusionMapPool( NULL )
{
	try{
		pTextureStageManager = new deoglTextureStageManager( renderThread );
		pImageStageManager = new deoglImageStageManager( renderThread );
		pCombinedTextureList = new deoglCombinedTextureList( renderThread );
		
		pRenColorTexMgr = new deoglRenderableColorTextureManager( renderThread );
		pRenDepthTexMgr = new deoglRenderableDepthTextureManager( renderThread );
		pRenColorCubeMgr = new deoglRenderableColorCubeMapManager( renderThread );
		pRenDepthCubeMgr = new deoglRenderableDepthCubeMapManager( renderThread );
		pRenColorArrTexMgr = new deoglRenderableColorArrayTextureManager( renderThread );
		pRenDepthArrTexMgr = new deoglRenderableDepthArrayTextureManager( renderThread );
		
		pOcclusionMapPool = new deoglOcclusionMapPool( renderThread );
		
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
	if( pOcclusionMapPool ){
		delete pOcclusionMapPool;
	}
	
	if( pRenColorArrTexMgr ){
		delete pRenColorArrTexMgr;
	}
	if( pRenDepthArrTexMgr ){
		delete pRenDepthArrTexMgr;
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
	if( pImageStageManager ){
		delete pImageStageManager;
	}
}
