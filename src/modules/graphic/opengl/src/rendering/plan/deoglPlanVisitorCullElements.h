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

#ifndef _DEOGLPLANVISITORCULLELEMENTS_H_
#define _DEOGLPLANVISITORCULLELEMENTS_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>

#include "../../world/deoglDefaultWorldOctreeVisitor.h"

class deoglDCollisionFrustum;
class deoglRenderPlan;
class deoglWorldOctree;



/**
 * @brief Render Plan World Octree Visitor Cull Elements.
 * Visits scene elements colliding with a frustum in a render plan prepare phase. Adds all elements
 * conservatively intersecting the frustum  and matching optimization filters. The sections describe
 * all the filters used for culling scene elements.
 * 
 * @par Frustum and boundary box
 * Culls elements conservatively against a frustum collision volume representing. Use Init to set
 * the frustum from camera and viewport parameters. This initializes various parameters also used
 * by other filters thus call this first before any other function.
 * 
 * @par Too small components
 * Culls components having a maximal size on screen less than a threshold pixel size. This filter
 * requires the camera parameters, a pixel size threshold and a scaling factor for the error metric
 * stored in components. The scaling factor is calculated during Init. The minimal pixel size
 * is set using SetCullPixelSize.
 * 
 * @par Dynamic Elements
 * Cull dynamic components if enabled. Use SetCullDynamicComponents to enable or disable this filter.
 */
class deoglPlanVisitorCullElements : public deoglDefaultWorldOctreeVisitor{
private:
	deoglRenderPlan *pPlan;
	
	decDVector pFrustumMinExtend;
	decDVector pFrustumMaxExtend;
	deoglDCollisionFrustum *pFrustum;
	
	// cull components with a maximal on-screen size less than a certain pixel threshold
	decDVector pCameraView;
	float pCullPixelSize;
	float pErrorScaling;
	
	// cull elements if they are dynamic
	bool pCullDynamicComponents;
	
	// cull using layermask and group name
	bool pCullLayerMask;
	decLayerMask pLayerMask;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglPlanVisitorCullElements( deoglRenderPlan *plan );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Initializes various parameters. Call this first before any other culling function. */
	void Init( deoglDCollisionFrustum *frustum );
	
	/** Retrieves the frustum. */
	inline deoglDCollisionFrustum *GetFrustum() const{ return pFrustum; }
	/** Retrieves the frustum minimum extend. */
	inline const decDVector &GetFrustumMinExtend() const{ return pFrustumMinExtend; }
	/** Retrieves the frustum maximum extend. */
	inline const decDVector &GetFrustumMaxExtend() const{ return pFrustumMaxExtend; }
	/** Sets the frustum boundary box extends. */
	void SetFrustumExtends( const decDVector &minExtend, const decDVector &maxExtend );
	/** Calculates the frustum boundary box from the plan camera parameters. */
	void CalculateFrustumBoundaryBox();
	
	/** Retrieves the camera view vector for the too-small filter. */
	inline const decDVector &GetCameraView() const{ return pCameraView; }
	/** Retrieves the minimal pixel size for the too-small filter. The value is set to 0.1 if set to less than 0.1 . */
	inline float GetCullPixelSize() const{ return pCullPixelSize; }
	/** Sets the minimal pixel size for the too-small filter. */
	void SetCullPixelSize( float cullPixelSize );
	/** Retrieves the error scaling factor for the too-small filter. */
	inline float GetErrorScaling() const{ return pErrorScaling; }
	/** Sets the error scaling factor for the too-small filter. */
	void SetErrorScaling( float errorScaling );
	/** Calculate the error scaling from the plan viewport and camera parameters. */
	void CalculateErrorScaling();
	
	/** Determines if dynamic components are culled. */
	inline bool GetCullDynamicComponents() const{ return pCullDynamicComponents; }
	/** Sets if dynamic components are culled. */
	void SetCullDynamicComponents( bool cullDynamicComponents );
	
	/** Determines if a layer mask is used for culling. */
	inline bool GetCullLayerMask() const{ return pCullLayerMask; }
	/** Sets if a layer mask is used for culling. */
	void SetCullLayerMask( bool cull );
	/** Retrieves the layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	/** Sets the layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Visit a world octree using this visitor. */
	void VisitWorldOctree( deoglWorldOctree &octree );
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visits an octree node. The default implementation is to visit all world elements stored in the node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	/*@}*/
};

#endif
