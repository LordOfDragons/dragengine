/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERTASKSHAREDPOOL_H_
#define _DEOGLRENDERTASKSHAREDPOOL_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/threading/deMutex.h>

class deoglRenderThread;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;
class deoglSkinTexture;



/**
 * Global pool for render task shared resources.
 */
class deoglRenderTaskSharedPool{
private:
	deoglRenderThread &pRenderThread;
	
	decObjectList pTextures;
	decObjectList pVAOs;
	decObjectList pInstances;
	decPointerList pSkinTextures;
	
	decPointerList pFreeTextures;
	decPointerList pFreeVAOs;
	decPointerList pFreeInstances;
	decIntList pFreeSkinTextures;
	
	deMutex pMutexSkinTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task shared pool. */
	deoglRenderTaskSharedPool( deoglRenderThread &renderThread );
	
	/** Clean up render task shared pool. */
	~deoglRenderTaskSharedPool();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get texture. */
	deoglRenderTaskSharedTexture *GetTexture();
	
	/** Get VAO. */
	deoglRenderTaskSharedVAO *GetVAO();
	
	/** Get instance. */
	deoglRenderTaskSharedInstance *GetInstance();
	
	/** Assign skin texture. */
	int AssignSkinTexture( deoglSkinTexture *skinTexture );
	
	
	
	/** Get texture at index. */
	deoglRenderTaskSharedTexture &GetTextureAt( int index ) const;
	
	/** Get VAO at index. */
	deoglRenderTaskSharedVAO &GetVAOAt( int index ) const;
	
	/** Get instance at index. */
	deoglRenderTaskSharedInstance &GetInstanceAt( int index ) const;
	
	/** Get skin texture at index. Manually lock GetMutexSkinTextures(). */
	deoglSkinTexture *GetSkinTextureAt( int index ) const;
	
	/** Get skin texture count. Manually lock GetMutexSkinTextures(). */
	int GetSkinTextureCount() const;
	
	
	
	/** Return texture. */
	void ReturnTexture( deoglRenderTaskSharedTexture *texture );
	
	/** Return VAO. */
	void ReturnVAO( deoglRenderTaskSharedVAO *vao );
	
	/** Return instance. */
	void ReturnInstance( deoglRenderTaskSharedInstance *instance );
	
	/** Return skin texture. */
	void ReturnSkinTexture( int slot );
	
	
	
	/** Skin textures mutex. */
	inline deMutex &GetMutexSkinTextures(){ return pMutexSkinTextures; }
	/*@}*/
};

#endif
