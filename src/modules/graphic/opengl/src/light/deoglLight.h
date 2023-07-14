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

#ifndef _DEOGLLIGHT_H_
#define _DEOGLLIGHT_H_

#include "../canvas/deoglCanvasViewListener.h"
#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicLight.h>

class deoglRLight;
class deoglSkinStateController;
class deoglWorld;
class deoglDynamicSkin;
class deoglCanvasView;
class deoglSkin;

class deGraphicOpenGl;
class deLight;



/**
 * Light Resource Peer.
 */
class deoglLight : public deBaseGraphicLight, deoglDynamicSkinListener, deoglCanvasViewListener{
public:
	deGraphicOpenGl &pOgl;
	const deLight &pLight;
	deoglRLight *pRLight;
	deoglSkinStateController *pSkinStateController;
	deoglWorld *pParentWorld;
	deoglDynamicSkin *pDynamicSkin;
	deoglSkin *pLightSkin;
	deoglCanvasView *pLightCanvas;
	
	float pAccumUpdate;
	
	bool pDirtyAttenuation;
	bool pDirtyCollideLists;
	bool pDirtyColVol;
	bool pDirtyConvexVolumeList;
	bool pDirtyExtends;
	bool pDirtyFullExtends;
	bool pDirtyLight;
	bool pDirtyLightParameters;
	bool pDirtyMatrices;
	bool pDirtyOctree;
	bool pDirtyShadowParameters;
	bool pDirtyShadows;
	bool pDirtySource;
	bool pDirtyDynamicSkin;
	bool pDirtyTouching;
	bool pDirtyTransform;
	bool pDirtyType;
	bool pDirtyEnvMapNotifyLightChanged;
	
	bool pDirtyRenderableMapping;
	bool pDirtySkinStateController;
	bool pDirtySkinStateCalculatedProperties;
	bool pDirtySkinStateConstructedProperties;
	bool pSkinStatePrepareRenderables;
	
	bool pDynamicSkinRenderablesChanged;
	bool pDynamicSkinRequiresSync;
	
	bool pLightCanvasRequiresSync;
	
	bool pRequiresUpdateEverySync;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light peer. */
	deoglLight( deGraphicOpenGl &ogl, const deLight &light );
	
	/** Clean up light peer. */
	virtual ~deoglLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Light resource. */
	inline const deLight &GetLight() const{ return pLight; }
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld( deoglWorld *world );
	
	
	
	/** Render light. */
	inline deoglRLight *GetRLight() const{ return pRLight; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	void DynamicSkinRequiresSync();
	void DirtyRenderableMapping();
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	virtual void DynamicSkinDestroyed();
	virtual void DynamicSkinRenderablesChanged();
	virtual void DynamicSkinRenderableChanged( deoglDSRenderable &renderable );
	virtual void DynamicSkinRenderableRequiresSync( deoglDSRenderable &renderable );
	/*@}*/
	
	
	
	/** \name Canvas view listener */
	/*@{*/
	virtual void CanvasViewDestroyed();
	virtual void CanvasViewRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Light type changed. */
	virtual void TypeChanged();
	
	/** Geomtry parameter changed. */
	virtual void GeometryParameterChanged();
	
	/** Attenuation changed. */
	virtual void AttenuationChanged();
	
	/**
	 * Light parameter changed.
	 * 
	 * - Color
	 * - Intensity
	 * - Ambient Ratio
	 * - Spot Smoothness
	 * - Spot Exponent
	 */
	virtual void LightParameterChanged();
	
	/** Shadow parameter changed. */
	virtual void ShadowParameterChanged();
	
	/** Light shape changed. */
	virtual void ShapeChanged();
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** Shadow layer mask changed. */
	virtual void ShadowLayerMaskChanged();
	
	/** Shadow ignore components changed. */
	virtual void ShadowIgnoreComponentsChanged();
	
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Image, video or canvas view changed. */
	virtual void SourceChanged();
	
	/** Transform changed. */
	virtual void TransformChanged();
	
	/** Activated changed. */
	virtual void ActivatedChanged();
	
	/** Hint changed. */
	virtual void HintChanged();
	
	
	
	/** World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pSyncSource();
	void pCheckRequiresUpdateEverySync();
	
	void pRequiresSync();
};

#endif
