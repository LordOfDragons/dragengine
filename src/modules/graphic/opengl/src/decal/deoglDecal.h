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

#ifndef _DEOGLDECAL_H_
#define _DEOGLDECAL_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicDecal.h>

class deoglDynamicSkin;
class deoglRDecal;
class deoglComponent;

class deGraphicOpenGl;
class deDecal;



/**
 * Decal peer.
 */
class deoglDecal : public deBaseGraphicDecal, deoglDynamicSkinListener{
public:
	deGraphicOpenGl &pOgl;
	const deDecal &pDecal;
	
	deoglRDecal *pRDecal;
	
	deoglDynamicSkin *pDynamicSkin;
	
	bool pDirtyVBO;
	bool pDirtyGeometry;
	bool pDirtyTransform;
	bool pDirtySkin;
	bool pDirtyDynamicSkin;
	bool pDirtyVisibility;
	bool pDirtyParamBlocks;
	bool pDirtyRenderableMapping;
	bool pDirtyStaticTexture;
	
	bool pDynamicSkinRequiresSync;
	
	bool pNotifyTextureChanged;
	bool pNotifyTUCChanged;
	
	deoglComponent *pParentComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglDecal( deGraphicOpenGl &ogl, const deDecal &decal );
	
	/** Clean up peer. */
	virtual ~deoglDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Decal engine resource. */
	inline const deDecal &GetDecal() const{ return pDecal; }
	
	
	
	/** Render decal. */
	inline deoglRDecal *GetRDecal() const{ return pRDecal; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Parent component or \em NULL. */
	inline deoglComponent *GetParentComponent() const{ return pParentComponent; }
	
	/** Set parent component or \em NULL. */
	void SetParentComponent( deoglComponent *component );
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
	/** Position, orientation or size changed. */
	virtual void GeometryChanged();
	
	/** Texture coordinates transformation changed. */
	virtual void TransformChanged();
	
	/** Skin changed. */
	virtual void SkinChanged();
	
	/** Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** Visible changed. */
	virtual void VisibleChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pSyncSkin();
	void pSyncDynamicSkin();
	
	void pRequiresSync();
};

#endif
