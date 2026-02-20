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

#ifndef _DEOGLLIGHT_H_
#define _DEOGLLIGHT_H_

#include "deoglRLight.h"
#include "../canvas/deoglCanvasViewListener.h"
#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicLight.h>

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
	deoglRLight::Ref pRLight;
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
	bool pDirtySkinStateStates;
	bool pSkinStatePrepareRenderables;
	
	bool pDynamicSkinRenderablesChanged;
	bool pDynamicSkinRequiresSync;
	
	bool pLightCanvasRequiresSync;
	
	bool pRequiresUpdateEverySync;
	
	decTLinkedList<deoglLight>::Element pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light peer. */
	deoglLight(deGraphicOpenGl &ogl, const deLight &light);
	
	/** Clean up light peer. */
	~deoglLight() override;
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
	void SetParentWorld(deoglWorld *world);
	
	
	
	/** Render light. */
	inline const deoglRLight::Ref &GetRLight() const{ return pRLight; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	void DynamicSkinRequiresSync();
	void DirtyRenderableMapping();
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	void DynamicSkinDestroyed() override;
	void DynamicSkinRenderablesChanged() override;
	void DynamicSkinRenderableChanged(deoglDSRenderable &renderable) override;
	void DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable) override;
	/*@}*/
	
	
	
	/** \name Canvas view listener */
	/*@{*/
	void CanvasViewDestroyed() override;
	void CanvasViewRequiresSync() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Light type changed. */
	void TypeChanged() override;
	
	/** Geomtry parameter changed. */
	void GeometryParameterChanged() override;
	
	/** Attenuation changed. */
	void AttenuationChanged() override;
	
	/**
	 * Light parameter changed.
	 * 
	 * - Color
	 * - Intensity
	 * - Ambient Ratio
	 * - Spot Smoothness
	 * - Spot Exponent
	 */
	void LightParameterChanged() override;
	
	/** Shadow parameter changed. */
	void ShadowParameterChanged() override;
	
	/** Light shape changed. */
	void ShapeChanged() override;
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** Shadow layer mask changed. */
	void ShadowLayerMaskChanged() override;
	
	/** Shadow ignore components changed. */
	void ShadowIgnoreComponentsChanged() override;
	
	/** Position changed. */
	void PositionChanged() override;
	
	/** Orientation changed. */
	void OrientationChanged() override;
	
	/** Image, video or canvas view changed. */
	void SourceChanged() override;
	
	/** Transform changed. */
	void TransformChanged() override;
	
	/** Activated changed. */
	void ActivatedChanged() override;
	
	/** Hint changed. */
	void HintChanged() override;
	
	
	
	/** World syncing linked list. */
	inline decTLinkedList<deoglLight>::Element &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decTLinkedList<deoglLight>::Element &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pSyncSource();
	void pCheckRequiresUpdateEverySync();
	
	void pRequiresSync();
};

#endif
