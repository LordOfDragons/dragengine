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
	bool pDirtySkinStateStates;
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
	inline deoglRLight *GetRLight() const{ return pRLight; }
	
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
