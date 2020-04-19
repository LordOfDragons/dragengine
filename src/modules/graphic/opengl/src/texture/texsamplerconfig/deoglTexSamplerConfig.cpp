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

#include "deoglTexSamplerConfig.h"
#include "../deoglTextureStageManager.h"
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
	if( pSamplerObject ){
		OGL_CHECK( pRenderThread, pglDeleteSamplers( 1, &pSamplerObject ) );
		pSamplerObject = 0;
	}
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
