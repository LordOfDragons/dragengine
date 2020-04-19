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

class deGraphicOpenGl;
class deDynamicSkin;



/**
 * \brief Dynamic skin peer.
 */
class deoglDynamicSkin : public deBaseGraphicDynamicSkin{
public:
	deGraphicOpenGl &pOgl;
	const deDynamicSkin &pDynamicSkin;
	
	deoglRDynamicSkin *pRDynamicSkin;
	
	decPointerList pRenderables;
	bool pDirtyRenderables;
	
	decPointerSet pNotifyComponents;
	decPointerSet pNotifyComponentTextures;
	decPointerSet pNotifyBillboards;
	decPointerSet pNotifyDecals;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglDynamicSkin( deGraphicOpenGl &ogl, const deDynamicSkin &dynamicSkin );
	
	/** \brief Clean up peer. */
	virtual ~deoglDynamicSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dynamic skin resources. */
	inline const deDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Render dynamic skin. */
	inline deoglRDynamicSkin *GetRDynamicSkin() const{ return pRDynamicSkin; }
	
	
	
	
	/** \brief Number of renderables. */
	int GetRenderableCount() const;
	
	/** \brief Renderable at index. */
	deoglDSRenderable *GetRenderableAt( int index ) const;
	
	/** \brief Components to notify about dirty events. */
	inline decPointerSet &GetNotifyComponents(){ return pNotifyComponents; }
	inline const decPointerSet &GetNotifyComponents() const{ return pNotifyComponents; }
	
	/** \brief Component textures to notify about dirty events. */
	inline decPointerSet &GetNotifyComponentTextures(){ return pNotifyComponentTextures; }
	inline const decPointerSet &GetNotifyComponentTextures() const{ return pNotifyComponentTextures; }
	
	/** \brief Billboards to notify about dirty events. */
	inline decPointerSet &GetNotifyBillboards(){ return pNotifyBillboards; }
	inline const decPointerSet &GetNotifyBillboards() const{ return pNotifyBillboards; }
	
	/** \brief Decals to notify about dirty events. */
	inline decPointerSet &GetNotifyDecals(){ return pNotifyDecals; }
	inline const decPointerSet &GetNotifyDecals() const{ return pNotifyDecals; }
	
	
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Renderable requires sync. */
	void RenderableRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Renderable added. */
	virtual void RenderableAdded( int index, deDSRenderable *renderable );
	
	/** \brief Renderable changed. */
	virtual void RenderableChanged( int index, deDSRenderable *renderable );
	
	/** \brief Renderable removed. */
	virtual void RenderableRemoved( int index, deDSRenderable *renderable );
	
	/** \brief All renderables removed. */
	virtual void AllRenderablesRemoved();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRequiresSync();
};

#endif
