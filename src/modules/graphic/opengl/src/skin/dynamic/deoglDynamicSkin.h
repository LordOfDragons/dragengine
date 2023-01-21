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
	deoglDynamicSkin( deGraphicOpenGl &ogl, const deDynamicSkin &dynamicSkin );
	
	/** Clean up peer. */
	virtual ~deoglDynamicSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Dynamic skin resources. */
	inline const deDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Render dynamic skin. */
	inline deoglRDynamicSkin *GetRDynamicSkin() const{ return pRDynamicSkin; }
	
	
	
	
	/** Number of renderables. */
	int GetRenderableCount() const;
	
	/** Renderable at index. */
	deoglDSRenderable *GetRenderableAt( int index ) const;
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener( deoglDynamicSkinListener *listener );
	
	/** Remove listener if existing. */
	void RemoveListener( deoglDynamicSkinListener *listener );
	
	/** Notify all dynamic skin has been destroyed. */
	void NotifyDestroyed();
	
	/** Notify all renderable structure changed. */
	void NotifyRenderablesChanged();
	
	/** Notify all renderable changed. */
	void NotifyRenderableChanged( deoglDSRenderable &renderable );
	
	/** Notify all renderable content requires sync. */
	void NotifyRenderableRequiresSync( deoglDSRenderable &renderable );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Renderable added. */
	virtual void RenderableAdded( int index, deDSRenderable *renderable );
	
	/** Renderable changed. */
	virtual void RenderableChanged( int index, deDSRenderable *renderable );
	
	/** Renderable removed. */
	virtual void RenderableRemoved( int index, deDSRenderable *renderable );
	
	/** All renderables removed. */
	virtual void AllRenderablesRemoved();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
