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

#include <dragengine/common/math/decMath.h>

#include "../../collidelist/deoglCollideList.h"


class deoglRenderThread;
class deoglRSkyInstance;
class deoglRSkyInstanceLayer;
class deoglRenderPlan;



/**
 * \brief Render plan sky light.
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
	deoglRenderThread &pRenderThread;
	deoglRSkyInstanceLayer *pLayer;
	
	deoglCollideList pCollideList;
	decVector pFrustumBoxMinExtend;
	decVector pFrustumBoxMaxExtend;
	
	bool pUseShadow;
	int pShadowLayerCount;
	sShadowLayer pShadowLayers[ 4 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render plan light. */
	deoglRenderPlanSkyLight( deoglRenderThread &renderThread );
	
	/** \brief Clean up render plan light. */
	~deoglRenderPlanSkyLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sky layer. */
	inline deoglRSkyInstanceLayer *GetLayer() const{ return pLayer; }
	
	/** \brief Set sky layer. */
	void SetLayer( deoglRSkyInstanceLayer *layer );
	
	
	
	/** \brief Collide list with potentially shadow casting elements. */
	inline deoglCollideList &GetCollideList(){ return pCollideList; }
	inline const deoglCollideList &GetCollideList() const{ return pCollideList; }
	
	/** \brief Frustum box min extend. */
	inline const decVector &GetFrustumBoxMinExtend() const{ return pFrustumBoxMinExtend; }
	
	/** \brief Frustum box max extend. */
	inline const decVector &GetFrustumBoxMaxExtend() const{ return pFrustumBoxMaxExtend; }
	
	
	
	/** \brief Use shadow casting. */
	inline bool GetUseShadow() const{ return pUseShadow; }
	
	/** \brief Shadow layer count. */
	inline int GetShadowLayerCount() const{ return pShadowLayerCount; }
	
	/** \brief Shadow layer at index. */
	sShadowLayer &GetShadowLayerAt( int index );
	const sShadowLayer &GetShadowLayerAt( int index ) const;
	
	
	
	/** \brief Clear. */
	void Clear();
	
	/** \brief Initialize plan. */
	void Init( deoglRenderPlan &plan );
	
	/**
	 * \brief Prepare for render.
	 * 
	 * \warning After this call various render states including temporary render targets are
	 *          potentially invalid. Call this only before doing any kind of rendering.
	 */
	void PrepareForRender( deoglRenderPlan &plan );
	
	
	
private:
	/** \brief Determine shadow parameters. */
	void pDetermineShadowParameters( deoglRenderPlan &plan );
	
	/** \brief Calculate shadow layer parameters. */
	void pCalcShadowLayerParams( deoglRenderPlan &plan );
	
	/** \brief Collect elements for shadow casting. */
	void pCollectElements( deoglRenderPlan &plan );
	/*@}*/
};

#endif
