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

#include "deoglTexSamplerConfig.h"
#include "../deoglTextureStageManager.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglTexSamplerConfig
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexSamplerConfig::deoglTexSamplerConfig( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pSamplerObject( 0 ),
pMinLodLevel( 0 ),
pMaxLodLevel( 20 ),
pFilterMode( deoglTextureStageManager::etfLinear ),
pWrapMode( GL_CLAMP_TO_EDGE ),
pDepthCompareMode( false ),
pDepthCompareFunc( GL_LEQUAL )
{
	OGL_CHECK( renderThread, pglGenSamplers( 1, &pSamplerObject ) );
	
	// set the opengl parameters to match our default parameters
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
	
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_WRAP_S, pWrapMode ) );
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_WRAP_T, pWrapMode ) );
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_WRAP_R, pWrapMode ) );
	
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_NONE ) );
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_COMPARE_FUNC, pDepthCompareFunc ) );
	
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_LOD, pMinLodLevel ) );
	OGL_CHECK( renderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAX_LOD, pMaxLodLevel ) );
}

deoglTexSamplerConfig::~deoglTexSamplerConfig(){
	pRenderThread.GetDelayedOperations().DeleteOpenGLSampler( pSamplerObject );
}



// Management
///////////////

void deoglTexSamplerConfig::SetMinLodLevel( int level ){
	if( level < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( level == pMinLodLevel ){
		return;
	}
	
	pMinLodLevel = level;
	OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_LOD, level ) );
}

void deoglTexSamplerConfig::SetMaxLodLevel( int level ){
	if( level < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( level == pMaxLodLevel ){
		return;
	}
	
	pMaxLodLevel = level;
	OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAX_LOD, level ) );
}

void deoglTexSamplerConfig::SetFilterMode( deoglTextureStageManager::eTextureFiltering filterMode ){
	if( filterMode == pFilterMode ){
		return;
	}
	
	pFilterMode = filterMode;
	
	switch( filterMode ){
	case deoglTextureStageManager::etfNearest:
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
		break;
		
	case deoglTextureStageManager::etfNearestMipMap:
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST ) );
		break;
		
	case deoglTextureStageManager::etfNearestMipMapLinear:
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ) );
		break;
		
	case deoglTextureStageManager::etfLinear:
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
		break;
		
	case deoglTextureStageManager::etfLinearMipMap:
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ) );
		break;
		
	case deoglTextureStageManager::etfLinearMipMapNearest:
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST ) );
		break;
	}
}

void deoglTexSamplerConfig::SetWrapMode( GLenum wrapMode ){
	if( wrapMode == pWrapMode ){
		return;
	}
	
	pWrapMode = wrapMode;
	
	OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_WRAP_S, wrapMode ) );
	OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_WRAP_T, wrapMode ) );
	OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_WRAP_R, wrapMode ) );
}

void deoglTexSamplerConfig::SetDepthCompareMode( bool depthCompareMode ){
	if( depthCompareMode == pDepthCompareMode ){
		return;
	}
	
	pDepthCompareMode = depthCompareMode;
	
	if( depthCompareMode ){
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_NONE ) );
	}
}

void deoglTexSamplerConfig::SetDepthCompareFunc( GLenum func ){
	if( func == pDepthCompareFunc ){
		return;
	}
	
	pDepthCompareFunc = func;
	
	OGL_CHECK( pRenderThread, pglSamplerParameteri( pSamplerObject, GL_TEXTURE_COMPARE_FUNC, func ) );
}
