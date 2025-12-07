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
	deoglRenderTaskSharedPool(deoglRenderThread &renderThread);
	
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
	int AssignSkinTexture(deoglSkinTexture *skinTexture);
	
	
	
	/** Get texture at index. */
	deoglRenderTaskSharedTexture &GetTextureAt(int index) const;
	
	/** Get VAO at index. */
	deoglRenderTaskSharedVAO &GetVAOAt(int index) const;
	
	/** Get instance at index. */
	deoglRenderTaskSharedInstance &GetInstanceAt(int index) const;
	
	/** Get skin texture at index. Manually lock GetMutexSkinTextures(). */
	deoglSkinTexture *GetSkinTextureAt(int index) const;
	
	/** Get skin texture count. Manually lock GetMutexSkinTextures(). */
	int GetSkinTextureCount() const;
	
	
	
	/** Return texture. */
	void ReturnTexture(deoglRenderTaskSharedTexture *texture);
	
	/** Return VAO. */
	void ReturnVAO(deoglRenderTaskSharedVAO *vao);
	
	/** Return instance. */
	void ReturnInstance(deoglRenderTaskSharedInstance *instance);
	
	/** Return skin texture. */
	void ReturnSkinTexture(int slot);
	
	
	
	/** Skin textures mutex. */
	inline deMutex &GetMutexSkinTextures(){return pMutexSkinTextures;}
	/*@}*/
};

#endif
