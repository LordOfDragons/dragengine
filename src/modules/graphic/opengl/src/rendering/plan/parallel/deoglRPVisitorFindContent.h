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

#ifndef _DEOGLRPVISITORFINDCONTENT_H_
#define _DEOGLRPVISITORFINDCONTENT_H_

#include "../../../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/string/decString.h>

class deoglDCollisionFrustum;
class deoglRenderPlan;
class deoglWorldOctree;



/**
 * Render Plan World Octree Visitor Cull Elements.
 * Visits scene elements colliding with a frustum in a render plan prepare phase. Adds all elements
 * conservatively intersecting the frustum  and matching optimization filters. The sections describe
 * all the filters used for culling scene elements.
 * 
 * \par Frustum and boundary box
 * Culls elements conservatively against a frustum collision volume representing. Use Init to set
 * the frustum from camera and viewport parameters. This initializes various parameters also used
 * by other filters thus call this first before any other function.
 * 
 * \par Too small components
 * Culls components having a maximal size on screen less than a threshold pixel size. This filter
 * requires the camera parameters, a pixel size threshold and a scaling factor for the error metric
 * stored in components. The scaling factor is calculated during Init. The minimal pixel size
 * is set using SetCullPixelSize.
 * 
 * \par Dynamic Elements
 * Cull dynamic components if enabled. Use SetCullDynamicComponents to enable or disable this filter.
 */
class deoglRPVisitorFindContent {
private:
	deoglRenderPlan &pPlan;
	
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
	
	// include GI cascade
	bool pWithGICascade;
	decDVector pGICascadeMinExtend;
	decDVector pGICascadeMaxExtend;
	deoglDCollisionBox pGICascadeBox;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglRPVisitorFindContent(deoglRenderPlan &plan);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Initialize parameters. Call this first before any other culling function. */
	void Init(deoglDCollisionFrustum *frustum);
	
	
	
	/** Frustum. */
	inline deoglDCollisionFrustum *GetFrustum() const{ return pFrustum; }
	
	/** Frustum minimum extend. */
	inline const decDVector &GetFrustumMinExtend() const{ return pFrustumMinExtend; }
	
	/** Frustum maximum extend. */
	inline const decDVector &GetFrustumMaxExtend() const{ return pFrustumMaxExtend; }
	
	/** Calculate frustum boundary box from the plan camera parameters. */
	void CalculateFrustumBoundaryBox();
	
	
	
	/** Camera view vector for the too-small filter. */
	inline const decDVector &GetCameraView() const{ return pCameraView; }
	
	/** Minimal pixel size for the too-small filter. The value is set to 0.1 if set to less than 0.1 . */
	inline float GetCullPixelSize() const{ return pCullPixelSize; }
	
	/** Set minimal pixel size for the too-small filter. */
	void SetCullPixelSize(float cullPixelSize);
	
	/** Error scaling factor for the too-small filter. */
	inline float GetErrorScaling() const{ return pErrorScaling; }
	
	/** Set error scaling factor for the too-small filter. */
	void SetErrorScaling(float errorScaling);
	
	/** Calculate error scaling from the plan viewport and camera parameters. */
	void CalculateErrorScaling();
	
	
	
	/** Dynamic components are culled. */
	inline bool GetCullDynamicComponents() const{ return pCullDynamicComponents; }
	
	/** Set if dynamic components are culled. */
	void SetCullDynamicComponents(bool cullDynamicComponents);
	
	/** Layer mask is used for culling. */
	inline bool GetCullLayerMask() const{ return pCullLayerMask; }
	
	/** Set if layer mask is used for culling. */
	void SetCullLayerMask(bool cull);
	
	/** Layer mask. */
	const decLayerMask &GetLayerMask(){return pLayerMask;}
	
	/** Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** Enable GI Cascade. */
	void EnableGIBox(const decDVector &minExtend, const decDVector &maxExtend);
	
	/** Disable GI Cascade. */
	void DisableGIBox();
	
	
	
	/** Visit world octree. */
	void VisitWorldOctree(const deoglWorldOctree &octree);
	/*@}*/
	
	
	
private:
	void pVisitNode(const deoglWorldOctree &node, bool intersect);
	void pVisitNodeGICascade(const deoglWorldOctree &node, bool intersect);
	void pVisitComponents(const deoglWorldOctree &node, bool intersect);
	void pVisitBillboards(const deoglWorldOctree &node, bool intersect);
	void pVisitLights(const deoglWorldOctree &node, bool intersect);
	void pVisitLightsGICascade(const deoglWorldOctree &node, bool intersect);
	void pVisitParticleEmitters(const deoglWorldOctree &node, bool intersect);
};

#endif
