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

#ifndef _DEOGLGIBVH_H_
#define _DEOGLGIBVH_H_

#include "../collidelist/deoglCollideList.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../utils/bvh/deoglBVH.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRComponentLOD;
class deoglRWorld;


/**
 * \brief Global illumination BVH.
 * 
 * build BVH from mesh instances. each mesh instance then uses the occlusion mesh bvh
 * with the matrix to do the local tracing. this allows creating occlusion mesh BVH
 * once and then copy it to GPU as required.
 * 
 * NOTE texture formats are restricted so something like RGB32F doesn't even exist.
 *      furthermore 3-component versions exists only with OGL 4.0 onwards
 * 
 * NOTE 16-bit floating point has too little precision causing problems.
 * 
 * requires these TBO to be build:
 * 
 * - TBONodeBox: RGBA32F (stride 2 pixels)
 *   stores bounding box of all bvh nodes. each node uses two pixels: minExtend(0:RGB)
 *   maxExtend(1:RGB). one component has to be wasted in each pixel due to format
 *   restrictions. contains package nodes of all mesh BVH then nodes of instance BVH.
 *   
 * - TBOIndex: RG32UI (stride 1 pixel)
 *   stores bvh node indiced.
 *   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
 *      faces in TBOPrimitiveData (absolute strided index). otherwise points to first child
 *      node in TBONodeBox/TBOIndex (absolute strided index).
 *   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
 *      TBONodeBox/TBOIndex (absolute strided index). for child nodes stays in instance BVH.
 *      for leaf nodes switches to mesh BVH traversal. points into TBOInstance and TBOMatrix.
 * 
 * - TBOInstance: R32UI (stride 1 pixel)
 *   stores instance offsets. bvhIndex(R) is the absolute strided index into TBONodeBox
 *   and TBOIndex with the mesh bvh root node.
 *   
 * - TBOMatrix: RGBA32F (stride 3 pixels)
 *   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
 *   
 * - TBOFace: RGBA32UI (stride 1 pixel)
 *   stores mesh faces. vertex1(R) vertex2(G) vertex3(B) doubleSided(A). indices into
 *   TBOVertex. face is doubleSided if doubleSided has value 1 or single sided if value is 0.
 *   
 * - TBOVertex: RGBA32F (stride 1 pixel)
 *   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
 *   
 * requires uniforms:
 * 
 * - uint IndexRootNode: index into TBONodeBox/TBOIndex containing instance BVH root node.
 */
class deoglGIBVH{
public:
	/** \brief Model. */
	struct sModel{
		deoglRComponentLOD *component;
		int indexNodes;
		int indexFaces;
		int indexVertices;
	};
	
	/** \brief Component. */
	struct sComponent{
		decMatrix matrix;
		int indexModel;
		int indexMatrix;
		int indexInstance;
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	float pMaxDetectionRange;
	deoglCollideList pCollideList;
	
	sModel *pModels;
	int pModelCount;
	int pModelSize;
	
	sComponent *pComponents;
	int pComponentCount;
	int pComponentSize;
	
	deoglBVH pBVH;
	deoglBVH::sBuildPrimitive *pPrimitives;
	int pPrimitiveSize;
	
	int pIndexRootNode;
	
	deoglDynamicTBOFloat32 pTBONodeBox;
	deoglDynamicTBOUInt32 pTBOIndex;
	deoglDynamicTBOUInt32 pTBOInstance;
	deoglDynamicTBOFloat32 pTBOMatrix;
	deoglDynamicTBOUInt32 pTBOFace;
	deoglDynamicTBOFloat32 pTBOVertex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create global illumination BVH. */
	deoglGIBVH( deoglRenderThread &renderThread );
	
	/** \brief Clean up global illumination BVH. */
	~deoglGIBVH();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Collide list. */
	inline const deoglCollideList &GetCollideList() const{ return pCollideList; }
	
	/** \brief Index of bvh component root node or -1 if there are no components. */
	inline int GetIndexRootNode() const{ return pIndexRootNode; }
	
	/** \brief TBO for BVH node boundaries. */
	inline const deoglDynamicTBOFloat32 &GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** \brief TBO for BVH node indices. */
	inline const deoglDynamicTBOUInt32 &GetTBOIndex() const{ return pTBOIndex; }
	
	/** \brief TBO for instance data. */
	inline const deoglDynamicTBOUInt32 &GetTBOInstance() const{ return pTBOInstance; }
	
	/** \brief TBO for instance matrices. */
	inline const deoglDynamicTBOFloat32 &GetTBOMatrix() const{ return pTBOMatrix; }
	
	/** \brief TBO for mesh faces. */
	inline const deoglDynamicTBOUInt32 &GetTBOFace() const{ return pTBOFace; }
	
	/** \brief TBO for mesh vertices. */
	inline const deoglDynamicTBOFloat32 &GetTBOVertex() const{ return pTBOVertex; }
	
	
	
	/** \brief Clear BVH. */
	void Clear();
	
	/** \brief Find components. */
	void FindComponents( deoglRWorld &world, const decDVector &position, const decVector &detectionBox );
	
	/** \brief Add static components. */
	void AddStaticComponents( const decDVector &position );
	
	/** \brief Add static component. */
	void AddStaticComponent( const decMatrix &matrix, deoglRComponentLOD &lod );
	
	/** \brief Build BVH. */
	void BuildBVH();
	
	/** \brief Debug print BVH. */
	void DebugPrint( const decDVector &position );
	/*@}*/
	
	
	
private:
	sModel &pAddModel();
	sComponent &pAddComponent( int indexModel, const decMatrix &matrix );
	void pAddBVH( const deoglBVH &bvh, int rootIndexNodes, int rootIndexFaces );
};

#endif
