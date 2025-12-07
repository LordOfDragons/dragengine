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

#ifndef _DEOGLCAMERA_H_
#define _DEOGLCAMERA_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCamera.h>

class deoglRCamera;
class deoglWorld;

class deGraphicOpenGl;
class deCamera;



/**
 * Camera peer.
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
	bool pResetAdaptedIntensity;
	bool pDirtyVR;
	
	bool pEnableVR;
	
	decPointerSet pNotifyRenderables;
	decPointerSet pNotifyCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new opengl camera peer. */
	deoglCamera(deGraphicOpenGl &ogl, const deCamera &camera);
	
	/** Clean up camera peer. */
	virtual ~deoglCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render camera. */
	inline deoglRCamera *GetRCamera() const{ return pRCamera; }
	
	/** Camera. */
	inline const deCamera &GetCamera() const{ return pCamera; }
	
	/** Parent world or \em NULL. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world. */
	void SetParentWorld(deoglWorld *world);
	
	
	
	/** Updates the camera. */
	void Update(float elapsed);
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Renderables to notify about dirty events. */
	inline decPointerSet &GetNotifyRenderables(){ return pNotifyRenderables; }
	inline const decPointerSet &GetNotifyRenderables() const{ return pNotifyRenderables; }
	
	/** Canvas to notify about dirty events. */
	inline decPointerSet &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decPointerSet &GetNotifyCanvas() const{ return pNotifyCanvas; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Camera parameter changed. */
	virtual void ParameterChanged();
	
	/** Adaption parameter changed. */
	virtual void AdaptionChanged();
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** Request graphic module to reset adapted intensity to optimal value. */
	virtual void ResetAdaptedIntensity();
	
	
	
	/** Effect has been added. */
	virtual void EffectAdded(int index, deEffect *effect);
	
	/** Effect has been removed. */
	virtual void EffectRemoved(int index, deEffect *effect);
	
	/** All effects have been removed. */
	virtual void AllEffectsRemoved();
	/*@}*/
	
	
	
	/** \name For use by VR Module only */
	/*@{*/
	/** Camera assigned to HMD. */
	virtual void VRAssignedToHMD();
	
	/** Camera resigned from HMD. */
	virtual void VRResignedFromHMD();
	
	/** VR Render parameters changed. */
	virtual void VRRenderParametersChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRequiresSync();
};

#endif
