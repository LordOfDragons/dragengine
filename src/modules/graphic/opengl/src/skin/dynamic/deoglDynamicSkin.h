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

#ifndef _DEOGLDYNAMICSKIN_H_
#define _DEOGLDYNAMICSKIN_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicDynamicSkin.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decPointerSet.h>

class deoglDSRenderable;
class deoglRDynamicSkin;
class deoglDynamicSkinListener;

class deGraphicOpenGl;
class deDynamicSkin;



/**
 * Dynamic skin peer.
 */
class deoglDynamicSkin : public deBaseGraphicDynamicSkin{
public:
	deGraphicOpenGl &pOgl;
	const deDynamicSkin &pDynamicSkin;
	
	deoglRDynamicSkin *pRDynamicSkin;
	
	decPointerList pRenderables;
	bool pDirtyRenderables;
	
	decPointerSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglDynamicSkin(deGraphicOpenGl &ogl, const deDynamicSkin &dynamicSkin);
	
	/** Clean up peer. */
	virtual ~deoglDynamicSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Graphic module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Dynamic skin resources. */
	inline const deDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Render dynamic skin. */
	inline deoglRDynamicSkin *GetRDynamicSkin() const{ return pRDynamicSkin; }
	
	
	
	
	/** Number of renderables. */
	int GetRenderableCount() const;
	
	/** Renderable at index. */
	deoglDSRenderable *GetRenderableAt(int index) const;
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener(deoglDynamicSkinListener *listener);
	
	/** Remove listener if existing. */
	void RemoveListener(deoglDynamicSkinListener *listener);
	
	/** Notify all dynamic skin has been destroyed. */
	void NotifyDestroyed();
	
	/** Notify all renderable structure changed. */
	void NotifyRenderablesChanged();
	
	/** Notify all renderable changed. */
	void NotifyRenderableChanged(deoglDSRenderable &renderable);
	
	/** Notify all renderable content requires sync. */
	void NotifyRenderableRequiresSync(deoglDSRenderable &renderable);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Renderable added. */
	virtual void RenderableAdded(int index, deDSRenderable *renderable);
	
	/** Renderable changed. */
	virtual void RenderableChanged(int index, deDSRenderable *renderable);
	
	/** Renderable removed. */
	virtual void RenderableRemoved(int index, deDSRenderable *renderable);
	
	/** All renderables removed. */
	virtual void AllRenderablesRemoved();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
