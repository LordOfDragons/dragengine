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
#include "../utils/bvh/deoglBVH.h"
#include "../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglGIBVHLocal;
class deoglGIInstance;
class deoglGIInstances;
class deoglRComponent;
class deoglRComponentLOD;
class deoglRComponentTexture;
class deoglRDynamicSkin;
class deoglRenderPlan;
class deoglRenderTaskTexture;
class deoglRenderThread;
class deoglRSkin;
class deoglRWorld;
class deoglSkinState;
class deoglSkinTexture;
class deoglDynamicTBOFloat32;
class deoglDynamicTBOFloat16;
class deoglDynamicTBOUInt32;
class deoglDynamicTBOUInt16;
class deoglDynamicTBOShared;


/**
 * Global illumination BVH.
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
 * - TBOIndex: RG16UI (stride 1 pixel)
 *   stores bvh node indiced.
 *   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
 *      faces in TBOPrimitiveData (relative strided index). otherwise points to first child
 *      node in TBONodeBox/TBOIndex (relative strided index).
 *   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
 *      TBONodeBox/TBOIndex (absolute strided index). for child nodes stays in instance BVH.
 *      for leaf nodes switches to mesh BVH traversal. points into TBOInstance and TBOMatrix.
 * 
 * - TBOInstance: RGBA32UI (stride 1 pixel)
 *   stores instance offsets. bvhIndex(R) is the absolute strided index into TBONodeBox
 *   and TBOIndex with the mesh bvh root node. materialIndex(G) is the absolute strided
 *   index into TBOMaterial* of the first instance material. vertexIndex(B) is the absolute
 *   strided index into TBOVertex of the first instance vertex. faceIndex(A) is the absolute
 *   strided index into TBOFace of the first instance face.
 *   
 * - TBOMatrix: RGBA32F (stride 3 pixels)
 *   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
 *   
 * - TBOFace: RGBA16UI (stride 1 pixel)
 *   stores mesh faces. vertex1(R) vertex2(G) vertex3(B) textureIndex(A). indices into
 *   TBOVertex. textureIndex is relative index into TBOMaterial*.
 *   
 * - TBOVertex: RGBA32F (stride 1 pixel)
 *   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
 *   
 * - TBOTexCoord: RG16F (stride 3 pixel)
 *   stores mesh texture coordinates. same unstrided index as TBOFace.
 *   
 * - TBOMaterial: RGBA32UI (stride 1 pixel)
 *   stores the instance material parameters in bit-field packed form. each pixel stores one
 *   texture material. hence multiple consequitive pixels belong to one instance and the
 *   number of textures per instance is variable.
 *     TODO modify TBOInstance to be RG32UI and store in green component the index to the
 *          first material pixel. the count is not required to be stored since the material
 *          index in the model is keep inside the range
 *   
 *   stores these material properties (bits required for each component and total bits):
 *   - color.tint:              8 8 8     => 24
 *   - color.gamma:             8         => 8
 *   - roughness.remap.*:       8 8       => 16
 *   - roughness.gamma:         8         => 8
 *   - reflectivity.multiplier: 8         => 8
 *   - ignore:                  1         => 1
 *   - texCoord.clamp:          1         => 1
 *   - has solidity:            1         => 1
 *   
 *   also store the index of the material in the material texture atlas as 16 bits value.
 *   14 bits allows for a maximum of 16383 materials. this equals a 128x128 atlas size or
 *   16 pixel material maps as the worst case. still using 16 bits to simplify layout.
 *   
 *   gamma is limited to the range from 0.4 to 2.2 mapped to 8 bits. this maps pixel value
 *   85 precisely to gamma value 1.
 *   
 *   this is a total of 83 bits. unfortunately TBOs do not allow for RGB format until
 *   opengl 4.3 or higher. this way 96 bits could be used to reduce the wasted space.
 *   
 *   the following layout is used:
 *   - red:
 *     - 16: material atlas index
 *     - 12: (reserved)
 *     - 1: ignore back face
 *     - 1: has solidity
 *     - 1: tex-coord clamp
 *     - 1: ignore material
 *   - green:
 *     - 24: color.tint.rgb
 *     - 8: color.gamma
 *   - blue:
 *     - 8: roughness.remap.lower
 *     - 8: roughness.remap.upper
 *     - 8: roughness.gamma
 *     - 8: reflectivity.multiplier
 *   - alpha:
 *     - 32: (reserved)
 *   
 *   for ray casting material index and flags are required. this allows to reduce sampling
 *   in the ray caster to 1 32-bit component to increase performance.
 *   - bvec3 flags = notEqual(uvec3(values.r) & uvec3(0x4, 0x2, 0x1), uvec3(0));
 *   - int materialIndex = int(values.r >> 16);
 *   
 *   this uses 96 bits (48 unused)
 *   
 *   possible compacting to gain more free bits can be done like this:
 *   - color.tint => 565 (16 bits instead of 24)
 *   
 * - TBOMaterial2: RGBA16F (stride 3 pixel)
 *   contains material parameters requiring floating point values to be stored:
 *   - pixel 1: texCoordMatrix.row1(rgb) unused(a)
 *   - pixel 2: texCoordMatrix.row2(rgb) unused(a)
 *   - pixel 3: emissivity(rgb) unused(a)
 *   
 * requires uniforms:
 * 
 * - uint IndexRootNode: index into TBONodeBox/TBOIndex containing instance BVH root node.
 */
class deoglGIBVH{
private:
	/** Model. */
	struct sModel{
		deoglRComponentLOD *component;
		deoglRComponent *occlusionMesh;
		int indexNodes;
		int indexFaces;
		int indexVertices;
	};
	
	/** Component. */
	struct sComponent{
		decMatrix matrix;
		int indexModel;
		int indexMatrix;
		int indexInstance;
		int indexMaterial;
	};
	
	
	
	deoglRenderThread &pRenderThread;
	
	float pMaxDetectionRange;
	
	sModel *pModels;
	int pModelCount;
	int pModelSize;
	
	sComponent *pComponents;
	int pComponentCount;
	int pComponentSize;
	
	deoglBVH pBVH;
	deoglBVH::sBuildPrimitive *pPrimitives;
	int pPrimitiveSize;
	deoglBVHNode *pRecalcNodes;
	int pRecalcNodeSize;
	
	int pIndexRootNode;
	
	deoglDynamicTBOFloat32 *pTBONodeBox;
	deoglDynamicTBOUInt16 *pTBOIndex;
	deoglDynamicTBOUInt32 *pTBOInstance;
	deoglDynamicTBOFloat32 *pTBOMatrix;
	deoglDynamicTBOUInt16 *pTBOFace;
	deoglDynamicTBOFloat32 *pTBOVertex;
	deoglDynamicTBOFloat16 *pTBOTexCoord;
	deoglDynamicTBOUInt32 *pTBOMaterial;
	deoglDynamicTBOFloat16 *pTBOMaterial2;
	
	deoglDynamicTBOShared *pSharedTBONodeBox;
	deoglDynamicTBOShared *pSharedTBOIndex;
	deoglDynamicTBOShared *pSharedTBOFace;
	deoglDynamicTBOShared *pSharedTBOVertex;
	deoglDynamicTBOShared *pSharedTBOTexCoord;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination BVH. */
	deoglGIBVH( deoglRenderThread &renderThread );
	
	/** Clean up global illumination BVH. */
	~deoglGIBVH();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Index of bvh component root node or -1 if there are no components. */
	inline int GetIndexRootNode() const{ return pIndexRootNode; }
	
	/** TBO for BVH node boundaries. */
	inline deoglDynamicTBOFloat32 *GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** TBO for BVH node indices. */
	inline deoglDynamicTBOUInt16 *GetTBOIndex() const{ return pTBOIndex; }
	
	/** TBO for instance data. */
	inline deoglDynamicTBOUInt32 *GetTBOInstance() const{ return pTBOInstance; }
	
	/** TBO for instance matrices. */
	inline deoglDynamicTBOFloat32 *GetTBOMatrix() const{ return pTBOMatrix; }
	
	/** TBO for mesh faces. */
	inline deoglDynamicTBOUInt16 *GetTBOFace() const{ return pTBOFace; }
	
	/** TBO for mesh vertices. */
	inline deoglDynamicTBOFloat32 *GetTBOVertex() const{ return pTBOVertex; }
	
	/** TBO for mesh texture coordinates. */
	inline deoglDynamicTBOFloat16 *GetTBOTexCoord() const{ return pTBOTexCoord; }
	
	/** TBO for material parameters. */
	inline deoglDynamicTBOUInt32 *GetTBOMaterial() const{ return pTBOMaterial; }
	
	/** TBO for material float parameters. */
	inline deoglDynamicTBOFloat16 *GetTBOMaterial2() const{ return pTBOMaterial2; }
	
	/** Shared TBOs. */
	inline deoglDynamicTBOShared *GetSharedTBONodeBox() const{ return pSharedTBONodeBox; }
	inline deoglDynamicTBOShared *GetSharedTBOIndex() const{ return pSharedTBOIndex; }
	inline deoglDynamicTBOShared *GetSharedTBOFace() const{ return pSharedTBOFace; }
	inline deoglDynamicTBOShared *GetSharedTBOVertex() const{ return pSharedTBOVertex; }
	inline deoglDynamicTBOShared *GetSharedTBOTexCoord() const{ return pSharedTBOTexCoord; }
	
	
	
	/** Clear BVH. */
	void Clear();
	
	/** Add components. */
	void AddComponents( deoglRenderPlan &plan, const decDVector &position, const deoglGIInstances &instances );
	
	/** Add components. */
	void AddComponents( deoglRenderPlan &plan, const decDVector &position, const deoglCollideList &list );
	
	/** Add component. */
	void AddComponent( deoglRenderPlan &plan, const decMatrix &matrix, deoglRComponentLOD &lod );
	
	/** Add occlusion meshes. */
	void AddOcclusionMeshes( deoglRenderPlan &plan, const decDVector &position, const deoglGIInstances &instances );
	
	/** Add occlusion meshes. */
	void AddOcclusionMeshes( deoglRenderPlan &plan, const decDVector &position, const deoglCollideList &list );
	
	/** Add occlusion mesh. */
	void AddOcclusionMesh( deoglRenderPlan &plan, const decMatrix &matrix, deoglRComponent &component );
	
	/** Build BVH. */
	void BuildBVH();
	
	/** Debug print BVH. */
	void DebugPrint( const decDVector &position );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	sModel &pAddModel();
	sComponent &pAddComponent( int indexModel, int indexMaterial, const decMatrix &matrix );
	void pAddMaterial( const deoglRComponentTexture &texture, deoglRenderTaskTexture *renderTaskTexture );
	void pAddMaterial( const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
		deoglRDynamicSkin *dynamicSkin, deoglRenderTaskTexture *renderTaskTexture,
		const decTexMatrix2 &texCoordMatrix );
	void pAddBVH( const deoglBVH &bvh );
	void pAddLocalBVH( const deoglGIBVHLocal &localBVH );
	void pAddLocalBVHUpdated( const deoglGIBVHLocal &localBVH, const oglVector *positions );
	void pUpdateLocalBVHNodeExtends( const deoglGIBVHLocal &localBVH, const oglVector *positions,
		const deoglBVHNode &node, deoglBVHNode &target );
	void pEnsureRecalcNodeSize( int size );
};

#endif
