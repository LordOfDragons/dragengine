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

#ifndef _DEOGLRLSVISITORCOLLECTELEMENTS_H_
#define _DEOGLRLSVISITORCOLLECTELEMENTS_H_

#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/math/decMath.h>

#include "../../../utils/collision/deoglDCollisionBox.h"
#include "../../../utils/collision/deoglDCollisionFrustum.h"
#include "../../../world/deoglDefaultWorldOctreeVisitor.h"

class deoglCollideList;
class deoglDCollisionBox;
class deoglDCollisionFrustum;
class deoglOcclusionTest;
class deoglHTView;
class deoglRenderPlan;
class deoglRSkyLayer;
class deoglRSkyInstanceLayer;
class deoglRWorld;


/**
 * Render Light Sky Visitor Add Elements. World octree visitor to add elements for light
 * sky shadow casting. This is a version of the deoglRLSVisitorCollectElements class
 * modified for speed and additional testing. Elements are tested against a transformed
 * frustum using an orthogonal extended shadow shaft enclosing the element extending all
 * the way along the remaining of the shadow box. The frustum test is done using a
 * box-frustum check and a check of the box against the convex hull of the frustum
 * projected to the shadow plane. This allows to cull many more elements from the shadow
 * casting than possible using deoglRLSVisitorCollectElements.
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
	
	deoglOcclusionTest *pOcclusionTest;
	decDVector pReferencePosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglRLSVisitorCollectElements( deoglCollideList &collideList );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Frustum box min extend. */
	inline const decVector &GetFrustumBoxMinExtend() const{ return pFrustumBoxMinExtend; }
	
	/** Frustum box max extend. */
	inline const decVector &GetFrustumBoxMaxExtend() const{ return pFrustumBoxMaxExtend; }
	
	/** X light axis. */
	inline const decVector &GetAxisX() const{ return pAxisX; }
	
	/** Y light axis. */
	inline const decVector &GetAxisY() const{ return pAxisY; }
	
	/** Z light axis. */
	inline const decVector &GetAxisZ() const{ return pAxisZ; }
	
	/** Absolute x light axis. */
	inline const decVector &GetAbsAxisX() const{ return pAbsAxisX; }
	
	/** Absolute y light axis. */
	inline const decVector &GetAbsAxisY() const{ return pAbsAxisY; }
	
	/** Absolute z light axis. */
	inline const decVector &GetAbsAxisZ() const{ return pAbsAxisZ; }
	
	/** Light space matrix. */
	inline const decDMatrix &GetMatrixLightSpace() const{ return pMatrixLightSpace; }
	
	/** Layer mask is used for culling. */
	inline bool GetCullLayerMask() const{ return pCullLayerMask; }
	
	/** Set if layer mask is used for culling. */
	void SetCullLayerMask( bool cull );
	
	/** Layer mask. */
	const decLayerMask &GetLayerMask(){ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Init visitor from frustum. */
	void InitFromFrustum( deoglRenderPlan &plan, deoglRSkyInstanceLayer &skyLayer, float backtrack );
	
	/** Init visitor from box. */
	void InitFromGIBox( const decDVector &position, const decVector &detectionBox,
		deoglRSkyInstanceLayer &skyLayer, float backtrack );
	
	/** Add split to check. */
	void AddSplit( const decVector &minExtend, const decVector &maxExtend, const decVector2 &sizeThreshold );
	
	/** Add occlusion test input data. */
	void AddOcclusionTestInputData( deoglOcclusionTest *occlusionTest, const decDVector &referencePosition );
	
	/** Visit world octree using visitor. */
	void VisitWorldOctree( deoglWorldOctree &octree );
	
	/** Visit height terrain view. */
	void VisitHTView( const deoglHTView &htview );
	
	/** Visit prop fields view. */
	void VisitPropFields( const deoglRWorld &world );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit an octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** Test axis aligned box for visibility. */
	bool TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** Test axis aligned box for visibility and store the shadow box split extends to be written to the element. */
	bool TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend, int &splitMask );
	/*@}*/
	
	
	
private:
	void pVisitComponents( const deoglWorldOctree &sonode );
	void pVisitBillboards( const deoglWorldOctree &sonode );
};

#endif
