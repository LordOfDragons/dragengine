/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
