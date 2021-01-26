/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLOCCLUSIONTRACING_H_
#define _DEOGLOCCLUSIONTRACING_H_

#include "../collidelist/deoglCollideList.h"
#include "../utils/bvh/deoglBVH.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglDynamicTBO;
class deoglROcclusionMesh;
class deoglRWorld;


/**
 * \brief Occlusion tracing.
 * 
 * Stores occlusion meshes for shaders to apply ray tracing to.
 */
class deoglOcclusionTracing{
public:
	/** \brief Occlusion meshes. */
	struct sOccMesh{
		deoglROcclusionMesh *occlusionMesh;
		int indexNodes;
		int indexFaces;
	};
	
	/** \brief Occlusion mesh instance. */
	struct sOccMeshInstance{
		decMatrix matrix;
		int indexMesh;
		int indexMatrix;
		int indexInstance;
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	
	float pProbeSpacing;
	decPoint3 pProbeCount;
	
	deoglCollideList pCollideList;
	decDVector pPosition;
	
	sOccMesh *pOccMeshes;
	int pOccMeshCount;
	int pOccMeshSize;
	
	sOccMeshInstance *pOccMeshInstances;
	int pOccMeshInstanceCount;
	int pOccMeshInstanceSize;
	
	deoglBVH pBVHInstances;
	deoglBVH::sBuildPrimitive *pBVHBuildPrimitives;
	int pBVHBuildPrimitiveSize;
	
	deoglDynamicTBOFloat16 pTBONodeBox;
	deoglDynamicTBOUInt32 pTBOIndex;
	deoglDynamicTBOUInt32 pTBOInstance;
	deoglDynamicTBOFloat16 pTBOMatrix;
	deoglDynamicTBOFloat16 pTBOFace;
	int pBVHInstanceRootNode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create occlusion tracing. */
	deoglOcclusionTracing( deoglRenderThread &renderThread );
	
	/** \brief Clean up occlusion tracing. */
	~deoglOcclusionTracing();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** \brief TBO for BVH node boundaries. */
	inline const deoglDynamicTBOFloat16 &GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** \brief TBO for BVH node indices. */
	inline const deoglDynamicTBOUInt32 &GetTBOIndex() const{ return pTBOIndex; }
	
	/** \brief TBO for instance data. */
	inline const deoglDynamicTBOUInt32 &GetTBOInstance() const{ return pTBOInstance; }
	
	/** \brief TBO for instance matrices. */
	inline const deoglDynamicTBOFloat16 &GetTBOMatrix() const{ return pTBOMatrix; }
	
	/** \brief TBO for mesh faces. */
	inline const deoglDynamicTBOFloat16 &GetTBOFace() const{ return pTBOFace; }
	
	/** \brief Index of bvh instance root node. */
	inline int GetBVHInstanceRootNode() const{ return pBVHInstanceRootNode; }
	
	/** \brief Update. */
	void Update( deoglRWorld &world, const decDVector &position );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	decPoint3 pWorld2Grid( const decDVector &position ) const;
	decDVector pGrid2World( const decPoint3 &position ) const;
	void pFindComponents( deoglRWorld &world );
	void pAddOcclusionMeshes();
	void pAddOcclusionMesh( const decMatrix &matrix, deoglROcclusionMesh *occlusionMesh );
// 	bool pAddFace( const decVector &v1, const decVector &v2, const decVector &v3, bool doubleSided );
	void pFinish();
};

#endif
