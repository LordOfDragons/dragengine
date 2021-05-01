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

#ifndef _DEOGLRENDERPLANSKYLIGHT_H_
#define _DEOGLRENDERPLANSKYLIGHT_H_

#include "../task/persistent/deoglPersistentRenderTask.h"
#include "../task/persistent/deoglAddToPersistentRenderTask.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../component/deoglComponentListener.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>


class deoglRenderThread;
class deoglRSkyInstance;
class deoglRSkyInstanceLayer;
class deoglRPTSkyLightFindContent;
class deoglRenderPlan;
class deoglOcclusionTest;


/**
 * Render plan sky light.
 */
class deoglRenderPlanSkyLight{
public:
	struct sShadowLayer{
		float frustumNear;
		float frustumFar;
		float layerBorder;
		float zscale;
		float zoffset;
		decVector minExtend;
		decVector maxExtend;
		decVector position;
		decVector scale;
		decMatrix matrix;
	};
	
	
	
private:
	class cGIComponentChangeListener : public deoglComponentListener {
	private:
		deoglRenderPlanSkyLight &pPlan;
		
	public:
		cGIComponentChangeListener( deoglRenderPlanSkyLight &plan );
		virtual void ComponentDestroyed( deoglRComponent &component );
		virtual void TUCChanged( deoglRComponent &component );
	};
	
	
	
	deoglRenderPlan &pPlan;
	deoglRSkyInstance *pSky;
	deoglRSkyInstanceLayer *pLayer;
	
	deoglCollideList pCollideList;
	decVector pFrustumBoxMinExtend;
	decVector pFrustumBoxMaxExtend;
	
	deoglOcclusionTest *pOcclusionTest;
	
	bool pPlanned;
	bool pUseLight;
	bool pUseShadow;
	int pShadowLayerCount;
	sShadowLayer pShadowLayers[ 4 ];
	
	deoglCollideList pGICollideList;
	decVector pGIBoxMinExtend;
	decVector pGIBoxMaxExtend;
	sShadowLayer pGIShadowLayer;
	deoglPersistentRenderTask pGIRenderTask;
	deoglAddToPersistentRenderTask pGIRenderTaskAdd;
	bool pGIRenderTaskUpdateMarker;
	deObjectReference pGIComponentChangeListener;
	
	deoglRPTSkyLightFindContent *pTaskFindContent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render plan light. */
	deoglRenderPlanSkyLight( deoglRenderPlan &plan );
	
	/** Clean up render plan light. */
	~deoglRenderPlanSkyLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render plan. */
	inline deoglRenderPlan &GetPlan() const{ return pPlan; }
	
	/** Sky instance. */
	inline deoglRSkyInstance *GetSky() const{ return pSky; }
	
	/** Sky layer. */
	inline deoglRSkyInstanceLayer *GetLayer() const{ return pLayer; }
	
	/** Set sky layer. */
	void SetLayer( deoglRSkyInstance *sky, deoglRSkyInstanceLayer *layer );
	
	
	
	/** Collide list with potentially shadow casting elements. */
	inline deoglCollideList &GetCollideList(){ return pCollideList; }
	inline const deoglCollideList &GetCollideList() const{ return pCollideList; }
	
	/** Frustum box min extend. */
	inline const decVector &GetFrustumBoxMinExtend() const{ return pFrustumBoxMinExtend; }
	
	/** Frustum box max extend. */
	inline const decVector &GetFrustumBoxMaxExtend() const{ return pFrustumBoxMaxExtend; }
	
	/** Set frustom box extends. */
	void SetFrustumBoxExtend( const decVector &minExtend, const decVector &maxExtend );
	
	
	
	/** Occlusion test. */
	inline deoglOcclusionTest *GetOcclusionTest() const{ return pOcclusionTest; }
	
	/** Set occlusion test. */
	void SetOcclusionTest( deoglOcclusionTest *occlusionTest );
	
	
	
	/** Plan has been planed. */
	inline bool GetPlanned() const{ return pPlanned; }
	
	/** Clear plan is planned. */
	void ClearPlanned();
	
	/** Use light. */
	inline bool GetUseLight() const{ return pUseLight; }
	
	/** Use shadow casting. */
	inline bool GetUseShadow() const{ return pUseShadow; }
	
	/** Shadow layer count. */
	inline int GetShadowLayerCount() const{ return pShadowLayerCount; }
	
	/** Shadow layer at index. */
	sShadowLayer &GetShadowLayerAt( int index );
	const sShadowLayer &GetShadowLayerAt( int index ) const;
	
	
	
	/** GI collide list with potentially shadow casting elements. */
	inline deoglCollideList &GetGICollideList(){ return pGICollideList; }
	inline const deoglCollideList &GetGICollideList() const{ return pGICollideList; }
	
	/** GI box min extend. */
	inline const decVector &GetGIBoxMinExtend() const{ return pGIBoxMinExtend; }
	
	/** GI box max extend. */
	inline const decVector &GetGIBoxMaxExtend() const{ return pGIBoxMaxExtend; }
	
	/** GI shadow layer. */
	inline sShadowLayer &GetGIShadowLayer(){ return pGIShadowLayer; }
	inline const sShadowLayer &GetGIShadowLayer() const{ return pGIShadowLayer; }
	
	/** GI render task. */
	inline deoglPersistentRenderTask &GetGIRenderTask(){ return pGIRenderTask; }
	inline const deoglPersistentRenderTask &GetGIRenderTask() const{ return pGIRenderTask; }
	
	/** GI component destroyed. */
	void GIComponentDestroyed( deoglRComponent &component );
	
	/** GI component changed TUC. */
	void GIComponentChangedTUC( deoglRComponent &component );
	
	
	
	/** Clear. */
	void Clear();
	
	/** Plan sky light. */
	void Plan();
	
	/** Start find content. */
	void StartFindContent();
	
	/** Start occlusion tests*/
	void StartOcclusionTests();
	
	/** Prepare for rendering. */
	void Prepare();
	
	/** Clean up after rendering. */
	void CleanUp();
	
	
	
private:
	void pDetermineShadowParameters();
	void pCalcShadowLayerParams();
	void pWaitFinishedFindContent();
	void pGICalcShadowLayerParams();
	void pGICollectElements();
	void pGIUpdateRenderTask();
	/*@}*/
};

#endif
