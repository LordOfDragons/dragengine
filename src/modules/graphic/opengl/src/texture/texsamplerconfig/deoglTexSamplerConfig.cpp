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

deoglTexSamplerConfig::deoglTexSamplerConfig(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pSamplerObject(0),
pMinLodLevel(0),
pMaxLodLevel(20),
pFilterMode(deoglTextureStageManager::etfLinear),
pWrapModeU(GL_CLAMP_TO_EDGE),
pWrapModeV(GL_CLAMP_TO_EDGE),
pWrapModeW(GL_CLAMP_TO_EDGE),
pDepthCompareMode(false),
pDepthCompareFunc(GL_LEQUAL)
{
	OGL_CHECK(renderThread, pglGenSamplers(1, &pSamplerObject));
	
	// set the opengl parameters to match our default parameters
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_WRAP_S, pWrapModeU));
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_WRAP_T, pWrapModeV));
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_WRAP_R, pWrapModeW));
	
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_NONE));
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_COMPARE_FUNC, pDepthCompareFunc));
	
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_LOD, pMinLodLevel));
	OGL_CHECK(renderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAX_LOD, pMaxLodLevel));
}

deoglTexSamplerConfig::~deoglTexSamplerConfig(){
	pRenderThread.GetDelayedOperations().DeleteOpenGLSampler(pSamplerObject);
}



// Management
///////////////

void deoglTexSamplerConfig::SetMinLodLevel(int level){
	if(level < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(level == pMinLodLevel){
		return;
	}
	
	pMinLodLevel = level;
	OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_LOD, level));
}

void deoglTexSamplerConfig::SetMaxLodLevel(int level){
	if(level < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(level == pMaxLodLevel){
		return;
	}
	
	pMaxLodLevel = level;
	OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAX_LOD, level));
}

void deoglTexSamplerConfig::SetFilterMode(deoglTextureStageManager::eTextureFiltering filterMode){
	if(filterMode == pFilterMode){
		return;
	}
	
	pFilterMode = filterMode;
	
	switch(filterMode){
	case deoglTextureStageManager::etfNearest:
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		break;
		
	case deoglTextureStageManager::etfNearestMipMap:
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		break;
		
	case deoglTextureStageManager::etfNearestMipMapLinear:
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
		break;
		
	case deoglTextureStageManager::etfLinear:
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		break;
		
	case deoglTextureStageManager::etfLinearMipMap:
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		break;
		
	case deoglTextureStageManager::etfLinearMipMapNearest:
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		break;
	}
}

void deoglTexSamplerConfig::SetWrapModeU(GLenum wrapMode){
	if(wrapMode == pWrapModeU){
		return;
	}
	
	pWrapModeU = wrapMode;
	OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_WRAP_S, wrapMode));
}

void deoglTexSamplerConfig::SetWrapModeV(GLenum wrapMode){
	if(wrapMode == pWrapModeV){
		return;
	}
	
	pWrapModeV = wrapMode;
	OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_WRAP_T, wrapMode));
}

void deoglTexSamplerConfig::SetWrapModeW(GLenum wrapMode){
	if(wrapMode == pWrapModeW){
		return;
	}
	
	pWrapModeW = wrapMode;
	OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_WRAP_R, wrapMode));
}

void deoglTexSamplerConfig::SetWrapMode(GLenum wrapMode){
	SetWrapModeU(wrapMode);
	SetWrapModeV(wrapMode);
	SetWrapModeW(wrapMode);
}

void deoglTexSamplerConfig::SetWrapMode(GLenum u, GLenum v, GLenum w){
	SetWrapModeU(u);
	SetWrapModeV(v);
	SetWrapModeW(w);
}

void deoglTexSamplerConfig::SetDepthCompareMode(bool depthCompareMode){
	if(depthCompareMode == pDepthCompareMode){
		return;
	}
	
	pDepthCompareMode = depthCompareMode;
	
	if(depthCompareMode){
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));
		
	}else{
		OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_COMPARE_MODE, GL_NONE));
	}
}

void deoglTexSamplerConfig::SetDepthCompareFunc(GLenum func){
	if(func == pDepthCompareFunc){
		return;
	}
	
	pDepthCompareFunc = func;
	
	OGL_CHECK(pRenderThread, pglSamplerParameteri(pSamplerObject, GL_TEXTURE_COMPARE_FUNC, func));
}
