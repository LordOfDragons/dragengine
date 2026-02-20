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

#ifndef _DEOGLBILLBOARD_H_
#define _DEOGLBILLBOARD_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"
#include "deoglRBillboard.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicBillboard.h>

class deGraphicOpenGl;
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
	
	deoglRBillboard::Ref pRBillboard;
	
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
	bool pDirtySkinStateStates;
	bool pSkinStatePrepareRenderables;
	
	bool pDynamicSkinRenderablesChanged;
	bool pDynamicSkinRequiresSync;
	bool pRequiresUpdateEverySync;
	
	decTLinkedList<deoglBillboard>::Element pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglBillboard(deGraphicOpenGl &ogl, const deBillboard &billboard);
	
	/** Clean up peer. */
	~deoglBillboard() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOpenGL() const{ return pOgl; }
	
	/** Billboard. */
	inline const deBillboard &GetBillboard() const{ return pBillboard; }
	
	
	
	/** Render billboard. */
	inline const deoglRBillboard::Ref &GetRBillboard() const{ return pRBillboard; }
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld(deoglWorld *world);
	
	
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update. */
	void Update(float elapsed);
	
	
	
	void DirtyRenderableMapping();
	
	
	
	/** World syncing linked list. */
	inline decTLinkedList<deoglBillboard>::Element &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decTLinkedList<deoglBillboard>::Element &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	void DynamicSkinDestroyed() override;
	void DynamicSkinRenderablesChanged() override;
	void DynamicSkinRenderableChanged(deoglDSRenderable &renderable) override;
	void DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable) override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	void PositionChanged() override;
	
	/** Axis changed. */
	void AxisChanged() override;
	
	/** Size changed. */
	void SizeChanged() override;
	
	/** Offset changed. */
	void OffsetChanged() override;
	
	/** Skin changed. */
	void SkinChanged() override;
	
	/** Dynamic skin changed. */
	void DynamicSkinChanged() override;
	
	/** Locked or spherical changed. */
	void ParametersChanged() override;
	
	/** Visibility changed. */
	void VisibilityChanged() override;
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSyncSkin();
	void pSyncDynamicSkin();
	void pCheckRequiresUpdateEverySync();
	
	void pRequiresSync();
};

#endif
