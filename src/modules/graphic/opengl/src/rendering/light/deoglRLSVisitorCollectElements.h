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

#ifndef _DEOGLRLSVISITORCOLLECTELEMENTS_H_
#define _DEOGLRLSVISITORCOLLECTELEMENTS_H_

#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/math/decMath.h>

#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../world/deoglDefaultWorldOctreeVisitor.h"

class deoglCollideList;
class deoglRenderPlan;
class deoglRSkyLayer;
class deoglRSkyInstanceLayer;
class deoglDCollisionBox;
class deoglDCollisionFrustum;


/**
 * \brief Render Light Sky Visitor Add Elements.
 * 
 * World octree visitor to add elements for light sky shadow casting. This is a version of the
 * deoglRLSVisitorCollectElements class modified for speed and additional testing. Elements are tested
 * against a transformed frustum using an orthogonal extended shadow shaft enclosing the element
 * extending all the way along the remaining of the shadow box. The frustum test is done using
 * a box-frustum check and a check of the box against the convex hull of the frustum projected
 * to the shadow plane. This allows to cull many more elements from the shadow casting than
 * possible using deoglRLSVisitorCollectElements.
 */
class deoglRLSVisitorCollectElements : public deoglDefaultWorldOctreeVisitor{
private:
	deoglCollideList &pCollideList;
	deoglDCollisionBox pVolumeShadowBox;
	decDVector pBoundaryBoxMinExtend;
	decDVector pBoundaryBoxMaxExtend;
	
	decDMatrix pMatrixLightSpace;
	
	decVector pFrustumBoxMinExtend;
	decVector pFrustumBoxMaxExtend;
	
	decVector pSplitMinExtend[ 4 ];
	decVector pSplitMaxExtend[ 4 ];
	decVector2 pSplitSizeThreshold[ 4 ];
	int pSplitCount;
	
	float pShaftFar;
	decVector pAxisX;
	decVector pAxisY;
	decVector pAxisZ;
	decVector pAbsAxisX;
	decVector pAbsAxisY;
	decVector pAbsAxisZ;
	
	decVector pFrustumPlaneNormal[ 6 ];
	decVector pFrustumPlaneNormalAbs[ 6 ];
	double pFrustumPlaneDistance[ 6 ];
	int pFrustumPlaneCount;
	
	decVector2 pEdgeNormal[ 5 ];
	decVector2 pEdgeNormalAbs[ 5 ];
	float pEdgeDistance[ 5 ];
	int pEdgeCount;
	
	bool pCullLayerMask;
	decLayerMask pLayerMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoglRLSVisitorCollectElements( deoglCollideList &collideList );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Frustum box min extend. */
	inline const decVector &GetFrustumBoxMinExtend() const{ return pFrustumBoxMinExtend; }
	
	/** \brief Frustum box max extend. */
	inline const decVector &GetFrustumBoxMaxExtend() const{ return pFrustumBoxMaxExtend; }
	
	/** \brief X light axis. */
	inline const decVector &GetAxisX() const{ return pAxisX; }
	
	/** \brief Y light axis. */
	inline const decVector &GetAxisY() const{ return pAxisY; }
	
	/** \brief Z light axis. */
	inline const decVector &GetAxisZ() const{ return pAxisZ; }
	
	/** \brief Absolute x light axis. */
	inline const decVector &GetAbsAxisX() const{ return pAbsAxisX; }
	
	/** \brief Absolute y light axis. */
	inline const decVector &GetAbsAxisY() const{ return pAbsAxisY; }
	
	/** \brief Absolute z light axis. */
	inline const decVector &GetAbsAxisZ() const{ return pAbsAxisZ; }
	
	/** \brief Light space matrix. */
	inline const decDMatrix &GetMatrixLightSpace() const{ return pMatrixLightSpace; }
	
	/** \brief Layer mask is used for culling. */
	inline bool GetCullLayerMask() const{ return pCullLayerMask; }
	
	/** \brief Set if layer mask is used for culling. */
	void SetCullLayerMask( bool cull );
	
	/** \brief Layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Init visitor from frustum. */
	void InitFromFrustum( deoglRenderPlan &plan, deoglRSkyInstanceLayer &skyLayer, float backtrack );
	
	/** \brief Init visitor from box. */
	void InitFromGIBox( const decDVector &position, const decVector &detectionBox,
		deoglRSkyInstanceLayer &skyLayer, float backtrack );
	
	/** \brief Add split to check. */
	void AddSplit( const decVector &minExtend, const decVector &maxExtend, const decVector2 &sizeThreshold );
	
	/** \brief Visit world octree using visitor. */
	void VisitWorldOctree( deoglWorldOctree &octree );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit an octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** \brief Test axis aligned box for visibility. */
	bool TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** \brief Test axis aligned box for visibility and store the shadow box split extends to be written to the element. */
	bool TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend, int &splitMask );
	/*@}*/
};

#endif
