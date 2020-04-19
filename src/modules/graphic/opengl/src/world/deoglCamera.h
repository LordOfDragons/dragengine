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

#ifndef _DEOGLCAMERA_H_
#define _DEOGLCAMERA_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCamera.h>

class deoglRCamera;
class deoglWorld;

class deGraphicOpenGl;
class deCamera;



/**
 * \brief Camera peer.
 */
class deoglCamera : public deBaseGraphicCamera{
private:
	deGraphicOpenGl &pOgl;
	const deCamera &pCamera;
	deoglRCamera *pRCamera;
	
	deoglWorld *pParentWorld;
	
	float pNextSyncUpdateTime;
	
	bool pDirtyGeometry;
	bool pDirtyMatrices;
	bool pDirtyAdaptionParams;
	bool pDirtyLayerMask;
	bool pDirtyPlanCamParams;
	bool pDirtyPropFields;
	bool pDirtyEffects;
	
	decPointerSet pNotifyRenderables;
	decPointerSet pNotifyCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new opengl camera peer. */
	deoglCamera( deGraphicOpenGl &ogl, const deCamera &camera );
	
	/** \brief Clean up camera peer. */
	virtual ~deoglCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render camera. */
	inline deoglRCamera *GetRCamera() const{ return pRCamera; }
	
	/** \brief Camera. */
	inline const deCamera &GetCamera() const{ return pCamera; }
	
	/** \brief Parent world or \em NULL. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world. */
	void SetParentWorld( deoglWorld *world );
	
	
	
	/** \brief Updates the camera. */
	void Update( float elapsed );
	
	
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Renderables to notify about dirty events. */
	inline decPointerSet &GetNotifyRenderables(){ return pNotifyRenderables; }
	inline const decPointerSet &GetNotifyRenderables() const{ return pNotifyRenderables; }
	
	/** \brief Canvas to notify about dirty events. */
	inline decPointerSet &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decPointerSet &GetNotifyCanvas() const{ return pNotifyCanvas; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Camera parameter changed. */
	virtual void ParameterChanged();
	
	/** \brief Adaption parameter changed. */
	virtual void AdaptionChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	
	
	/** \brief Effect has been added. */
	virtual void EffectAdded( int index, deEffect *effect );
	
	/** \brief Effect has been removed. */
	virtual void EffectRemoved( int index, deEffect *effect );
	
	/** \brief All effects have been removed. */
	virtual void AllEffectsRemoved();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRequiresSync();
};

#endif
