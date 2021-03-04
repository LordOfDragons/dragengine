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
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../utils/bvh/deoglBVH.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRComponentLOD;
class deoglRComponentTexture;
class deoglRWorld;
class deoglRSkin;
class deoglSkinTexture;
class deoglSkinState;
class deoglRDynamicSkin;
class deoglRenderTaskTexture;


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
 * - TBOInstance: RG32UI (stride 1 pixel)
 *   stores instance offsets. bvhIndex(R) is the absolute strided index into TBONodeBox
 *   and TBOIndex with the mesh bvh root node. materialIndex(G) is the absolute strided
 *   index into TBOMaterial of the first instance material.
 *   
 * - TBOMatrix: RGBA32F (stride 3 pixels)
 *   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
 *   
 * - TBOFace: RGBA32UI (stride 1 pixel)
 *   stores mesh faces. vertex1(R) vertex2(G) vertex3(B) textureIndex(A). indices into
 *   TBOVertex. textureIndex is relative index into TBOMaterial.
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
 *   - emissivity.intensity:    16 16 16  => 48
 *   - variation:*:             1 1       => 2
 *   
 *   also store the index of the material in the material texture atlas as 14 bits value.
 *   this allows to combine it with variation.* bits to fill up 16 bits. 14 bits allows
 *   for a maximum of 16383 materials. this equals a 128x128 atlas size or 16 pixel material
 *   maps as the worst case
 *   
 *   gamma is limited to the range from 0.4 to 2.2 mapped to 8 bits. this maps pixel value
 *   85 precisely to gamma value 1.
 *   
 *   pixel has a total of 128 bits. this leads to the following possible layout.
 *   - red:
 *     - 8: color.tint.r
 *     - 8: roughness.remap.lower
 *     - 16: emissivity.intensity.r
 *   - green:
 *     - 8: color.tint.g
 *     - 8: roughness.remap.upper
 *     - 16: emissivity.intensity.g
 *   - blue:
 *     - 8: color.tint.b
 *     - 8: roughness.gamma
 *     - 16: emissivity.intensity.b
 *   - alpha:
 *     - 8: color.gamma
 *     - 8: reflectivity.multiplier
 *     - 1: variation.u
 *     - 1: variation.v
 *     - 14: material atlas index
 *   
 *   the layout is chosen like this to allow GLSL to decompose the data using few instructions.
 *   - vec4 colorGamma = vec4(values >> 24)
 *                       * vec4(1.0/255.0, 1.0/255.0, 1.0/255.0, 1.8/255.0)
 *                       + vec4(0, 0, 0, 0.4);
 *   - vec4 roughnessGammaReflMul = vec4((values >> 16) & 0xff)
 *                                  * vec4(1.0/255.0, 1.0/255.0, 1.8/255.0, 1.0/255.0)
 *                                  + vec4(0, 0, 0.4, 0);
 *   - vec3 emissivity = vec3(values.rgb & 0xffff) * vec3(1.0 / 65535.0);
 *   - bvec2 variation = notEqual(values.aa & uvec2(0x8000, 0x4000), uvec2(0));
 *   - int materialIndex = int(values.a & 0x3fff);
 *   
 *   this uses 128 bits.
 *   
 *   possible compacting to gain more free bits can be done like this:
 *   - emissivity.intensity => 16:scale * 565:color (24 bits instead of 32)
 *   - emissivity.intensity => 11_11_10 (16 bit instead of 32)
 *   - color.tint => 565 (16 bits instead of 24)
 *   
 *   maximum saving 24 bits less. this requires though a new layout
 *   
 * requires uniforms:
 * 
 * - uint IndexRootNode: index into TBONodeBox/TBOIndex containing instance BVH root node.
 */
class deoglGIBVH{
private:
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
		int indexMaterial;
	};
	
	
	
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
	deoglDynamicTBOFloat16 pTBOTexCoord;
	deoglDynamicTBOUInt32 pTBOMaterial;
	
	
	
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
	
	/** \brief TBO for mesh texture coordinates. */
	inline const deoglDynamicTBOFloat16 &GetTBOTexCoord() const{ return pTBOTexCoord; }
	
	/** \brief TBO for material parameters. */
	inline const deoglDynamicTBOUInt32 &GetTBOMaterial() const{ return pTBOMaterial; }
	
	
	
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
	sComponent &pAddComponent( int indexModel, int indexMaterial, const decMatrix &matrix );
	void pAddMaterial( const deoglRComponentTexture &texture, deoglRenderTaskTexture *renderTaskTexture );
	void pAddMaterial( const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
		deoglRDynamicSkin *dynamicSkin, deoglRenderTaskTexture *renderTaskTexture );
	void pAddBVH( const deoglBVH &bvh, int rootIndexNodes, int rootIndexFaces );
};

#endif
