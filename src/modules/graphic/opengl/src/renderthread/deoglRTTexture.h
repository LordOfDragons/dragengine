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

#ifndef _DEOGLRRTTEXTURE_H_
#define _DEOGLRRTTEXTURE_H_

#include "../deoglBasics.h"

class deoglRenderThread;

class deoglCombinedTextureList;
class deoglRenderableTexture1DManager;
class deoglRenderableArrayTextureManager;
class deoglRenderableColorCubeMapManager;
class deoglRenderableColorTextureManager;
class deoglRenderableDepthCubeMapManager;
class deoglRenderableDepthTextureManager;
class deoglTextureStageManager;
class deoglOcclusionMapPool;



/**
 * Render thread texture related objects.
 */
class deoglRTTexture{
private:
	deoglTextureStageManager *pTextureStageManager;
	deoglCombinedTextureList *pCombinedTextureList;
	
	deoglRenderableColorTextureManager *pRenColorTexMgr;
	deoglRenderableDepthTextureManager *pRenDepthTexMgr;
	deoglRenderableColorCubeMapManager *pRenColorCubeMgr;
	deoglRenderableDepthCubeMapManager *pRenDepthCubeMgr;
	deoglRenderableTexture1DManager *pRenTex1DMgr;
	deoglRenderableArrayTextureManager *pRenArrTexMgr;
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
	
	/** Renderable 1d texture manager. */
	inline deoglRenderableTexture1DManager &GetRenderableTexture1D() const{ return *pRenTex1DMgr; }
	
	/** Renderable array texture manager. */
	inline deoglRenderableArrayTextureManager &GetRenderableArrayTexture() const{ return *pRenArrTexMgr; }
	
	/** Occlusion map pool. */
	inline deoglOcclusionMapPool &GetOcclusionMapPool() const{ return *pOcclusionMapPool; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
