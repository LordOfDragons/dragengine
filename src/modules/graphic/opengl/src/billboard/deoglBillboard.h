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

#ifndef _DEOGLBILLBOARD_H_
#define _DEOGLBILLBOARD_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicBillboard.h>

class deGraphicOpenGl;
class deoglRBillboard;
class deoglDynamicSkin;
class deoglSkinStateController;
class deoglWorld;

class deBillboard;



/**
 * Billboard peer.
 */
class deoglBillboard : public deBaseGraphicBillboard, deoglDynamicSkinListener{
public:
	deGraphicOpenGl &pOgl;
	const deBillboard &pBillboard;
	
	deoglRBillboard *pRBillboard;
	
	deoglWorld *pParentWorld;
	deoglSkinStateController *pSkinStateController;
	deoglDynamicSkin *pDynamicSkin;
	
	float pAccumUpdate;
	
	bool pDirtyGeometry;
	bool pDirtyOctreeNode;
	bool pDirtyExtends;
	bool pDirtyLayerMask;
	bool pDirtySkinStateController;
	bool pDirtyCullSphere;
	bool pDirtyRenderEnvMap;
	bool pDirtySkin;
	bool pDirtyDynamicSkin;
	bool pDirtyRenderableMapping;
	bool pDirtySkinStateCalculatedProperties;
	bool pDirtySkinStateConstructedProperties;
	bool pSkinStatePrepareRenderables;
	
	bool pDynamicSkinRenderablesChanged;
	bool pDynamicSkinRequiresSync;
	bool pRequiresUpdateEverySync;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglBillboard( deGraphicOpenGl &ogl, const deBillboard &billboard );
	
	/** Clean up peer. */
	virtual ~deoglBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Billboard. */
	inline const deBillboard &GetBillboard() const{ return pBillboard; }
	
	
	
	/** Render billboard. */
	inline deoglRBillboard *GetRBillboard() const{ return pRBillboard; }
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld( deoglWorld *world );
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update. */
	void Update( float elapsed );
	
	
	
	void DirtyRenderableMapping();
	
	
	
	/** World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	virtual void DynamicSkinDestroyed();
	virtual void DynamicSkinRenderablesChanged();
	virtual void DynamicSkinRenderableChanged( deoglDSRenderable &renderable );
	virtual void DynamicSkinRenderableRequiresSync( deoglDSRenderable &renderable );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Axis changed. */
	virtual void AxisChanged();
	
	/** Size changed. */
	virtual void SizeChanged();
	
	/** Offset changed. */
	virtual void OffsetChanged();
	
	/** Skin changed. */
	virtual void SkinChanged();
	
	/** Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** Locked or spherical changed. */
	virtual void ParametersChanged();
	
	/** Visibility changed. */
	virtual void VisibilityChanged();
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSyncSkin();
	void pSyncDynamicSkin();
	void pCheckRequiresUpdateEverySync();
	
	void pRequiresSync();
};

#endif
