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

#ifndef _DEOGLRRTFRAMEBUFFER_H_
#define _DEOGLRRTFRAMEBUFFER_H_

#include "../deoglBasics.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"

class deoglRenderThread;


/**
 * Render thread framebuffer related objects.
 */
class deoglRTFramebuffer{
private:
	deoglFramebufferManager pManager;
	
	deoglFramebuffer *pActive;
	const deoglFramebuffer::Ref pPrimary, pEnvMap, pEnvMapMaterial;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread framebuffer related object. */
	deoglRTFramebuffer( deoglRenderThread &renderThread );
	
	/** Clean up render thread framebuffer related object. */
	virtual ~deoglRTFramebuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Framebuffer manager. */
	inline deoglFramebufferManager &GetManager(){ return pManager; }
	inline const deoglFramebufferManager &GetManager() const{ return pManager; }
	
	/** Primary framebuffer. */
	inline const deoglFramebuffer::Ref &GetPrimary() const{ return pPrimary; }
	
	/** Active framebuffer. */
	inline deoglFramebuffer *GetActive() const{ return pActive; }
	
	/** Activate framebuffer or \em NULL to activate the primary framebuffer. */
	void Activate(deoglFramebuffer *framebuffer);
	
	/** Environment map framebuffer. */
	inline const deoglFramebuffer::Ref &GetEnvMap() const{ return pEnvMap; }
	
	/** Environment material map framebuffer. */
	inline const deoglFramebuffer::Ref &GetEnvMapMaterial() const{ return pEnvMapMaterial; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
