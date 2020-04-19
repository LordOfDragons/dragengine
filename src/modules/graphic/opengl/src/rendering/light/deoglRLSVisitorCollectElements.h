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
 * @brief Render Light Sky Visitor Add Elements.
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
	deoglCollideList *pCollideList;
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
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglRLSVisitorCollectElements( deoglCollideList *collideList );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the frustum box min extend. */
	inline const decVector &GetFrustumBoxMinExtend() const{ return pFrustumBoxMinExtend; }
	/** Retrieves the frustum box max extend. */
	inline const decVector &GetFrustumBoxMaxExtend() const{ return pFrustumBoxMaxExtend; }
	
	/** Retrieves the x light axis. */
	inline const decVector &GetAxisX() const{ return pAxisX; }
	/** Retrieves the y light axis. */
	inline const decVector &GetAxisY() const{ return pAxisY; }
	/** Retrieves the z light axis. */
	inline const decVector &GetAxisZ() const{ return pAxisZ; }
	/** Retrieves the absolute x light axis. */
	inline const decVector &GetAbsAxisX() const{ return pAbsAxisX; }
	/** Retrieves the absolute y light axis. */
	inline const decVector &GetAbsAxisY() const{ return pAbsAxisY; }
	/** Retrieves the absolute z light axis. */
	inline const decVector &GetAbsAxisZ() const{ return pAbsAxisZ; }
	/** Retrieves the light space matrix. */
	inline const decDMatrix &GetMatrixLightSpace() const{ return pMatrixLightSpace; }
	
	/** Init the visitor from a frustum. */
	void InitFromFrustum( deoglRenderPlan &plan, deoglRSkyInstanceLayer &skyLayer,
		float backtrack );
	
	/** Add a split to check. */
	void AddSplit( const decVector &minExtend, const decVector &maxExtend, const decVector2 &sizeThreshold );
	/** Visit a world octree using this visitor. */
	void VisitWorldOctree( deoglWorldOctree &octree );
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visits an octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** Test axis aligned box for visibility. */
	bool TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend );
	/** Test axis aligned box for visibility and store the shadow box split extends to be written to the element. */
	bool TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend, int &splitMask );
	/*@}*/
};

#endif
