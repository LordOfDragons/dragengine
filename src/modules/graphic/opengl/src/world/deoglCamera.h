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

#include "deoglRCamera.h"

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCamera.h>

class deoglWorld;
class deoglDSRenderableCamera;
class deoglCanvasRenderWorld;

class deGraphicOpenGl;
class deCamera;



/**
 * Camera peer.
 */
class deoglCamera : public deBaseGraphicCamera{
private:
	deGraphicOpenGl &pOgl;
	const deCamera &pCamera;
	deoglRCamera::Ref pRCamera;
	
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
	
	decTOrderedSet<deoglDSRenderableCamera*> pNotifyRenderables;
	decTOrderedSet<deoglCanvasRenderWorld*> pNotifyCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new opengl camera peer. */
	deoglCamera(deGraphicOpenGl &ogl, const deCamera &camera);
	
	/** Clean up camera peer. */
	~deoglCamera() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render camera. */
	inline const deoglRCamera::Ref &GetRCamera() const{ return pRCamera; }
	
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
	inline decTOrderedSet<deoglDSRenderableCamera*> &GetNotifyRenderables(){ return pNotifyRenderables; }
	inline const decTOrderedSet<deoglDSRenderableCamera*> &GetNotifyRenderables() const{ return pNotifyRenderables; }
	
	/** Canvas to notify about dirty events. */
	inline decTOrderedSet<deoglCanvasRenderWorld*> &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decTOrderedSet<deoglCanvasRenderWorld*> &GetNotifyCanvas() const{ return pNotifyCanvas; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	void PositionChanged() override;
	
	/** Orientation changed. */
	void OrientationChanged() override;
	
	/** Camera parameter changed. */
	void ParameterChanged() override;
	
	/** Adaption parameter changed. */
	void AdaptionChanged() override;
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** Request graphic module to reset adapted intensity to optimal value. */
	void ResetAdaptedIntensity() override;
	
	
	
	/** Effect has been added. */
	void EffectAdded(int index, deEffect *effect) override;
	
	/** Effect has been removed. */
	void EffectRemoved(int index, deEffect *effect) override;
	
	/** All effects have been removed. */
	void AllEffectsRemoved() override;
	/*@}*/
	
	
	
	/** \name For use by VR Module only */
	/*@{*/
	/** Camera assigned to HMD. */
	void VRAssignedToHMD() override;
	
	/** Camera resigned from HMD. */
	void VRResignedFromHMD() override;
	
	/** VR Render parameters changed. */
	void VRRenderParametersChanged() override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pRequiresSync();
};

#endif
