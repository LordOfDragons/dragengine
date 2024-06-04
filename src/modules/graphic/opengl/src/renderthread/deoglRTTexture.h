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

#ifndef _DEOGLRRTTEXTURE_H_
#define _DEOGLRRTTEXTURE_H_

#include "../deoglBasics.h"

class deoglRenderThread;

class deoglCombinedTextureList;
class deoglRenderableColorArrayTextureManager;
class deoglRenderableColorCubeMapManager;
class deoglRenderableColorTextureManager;
class deoglRenderableDepthCubeMapManager;
class deoglRenderableDepthArrayTextureManager;
class deoglRenderableDepthTextureManager;
class deoglTextureStageManager;
class deoglImageStageManager;
class deoglOcclusionMapPool;



/**
 * Render thread texture related objects.
 */
class deoglRTTexture{
private:
	deoglTextureStageManager *pTextureStageManager;
	deoglImageStageManager *pImageStageManager;
	deoglCombinedTextureList *pCombinedTextureList;
	
	deoglRenderableColorTextureManager *pRenColorTexMgr;
	deoglRenderableDepthTextureManager *pRenDepthTexMgr;
	deoglRenderableColorCubeMapManager *pRenColorCubeMgr;
	deoglRenderableDepthCubeMapManager *pRenDepthCubeMgr;
	deoglRenderableColorArrayTextureManager *pRenColorArrTexMgr;
	deoglRenderableDepthArrayTextureManager *pRenDepthArrTexMgr;
	deoglOcclusionMapPool *pOcclusionMapPool;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread texture related object. */
	deoglRTTexture( deoglRenderThread &renderThread );
	
	/** Clean up render thread texture related object. */
	virtual ~deoglRTTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Texture stage manager. */
	inline deoglTextureStageManager &GetStages() const{ return *pTextureStageManager; }
	
	/** Image stage manager. */
	inline deoglImageStageManager &GetImageStages() const{ return *pImageStageManager; }
	
	/** Combined texture list. */
	inline deoglCombinedTextureList &GetCombinedTexture() const{ return *pCombinedTextureList; }
	
	/** Renderable color texture manager. */
	inline deoglRenderableColorTextureManager &GetRenderableColorTexture() const{ return *pRenColorTexMgr; }
	
	/** Renderable depth texture manager. */
	inline deoglRenderableDepthTextureManager &GetRenderableDepthTexture() const{ return *pRenDepthTexMgr; }
	
	/** Renderable color cubemap manager. */
	inline deoglRenderableColorCubeMapManager &GetRenderableColorCubeMap() const{ return *pRenColorCubeMgr; }
	
	/** Renderable depth cubemap manager. */
	inline deoglRenderableDepthCubeMapManager &GetRenderableDepthCubeMap() const{ return *pRenDepthCubeMgr; }
	
	/** Renderable color array texture manager. */
	inline deoglRenderableColorArrayTextureManager &GetRenderableColorArrayTexture() const{ return *pRenColorArrTexMgr; }
	
	/** Renderable depth array texture manager. */
	inline deoglRenderableDepthArrayTextureManager &GetRenderableDepthArrayTexture() const{ return *pRenDepthArrTexMgr; }
	
	/** Occlusion map pool. */
	inline deoglOcclusionMapPool &GetOcclusionMapPool() const{ return *pOcclusionMapPool; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
