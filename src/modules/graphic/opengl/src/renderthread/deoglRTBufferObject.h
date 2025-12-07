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

#ifndef _DEOGLRRTBUFFEROBJECT_H_
#define _DEOGLRRTBUFFEROBJECT_H_

#include "../deoglBasics.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroupList.h"

class deoglRenderThread;
class deoglSharedVBOList;
class deoglSharedVBOListList;
class deoglShapeManager;
class deoglSharedSPBList;
class deoglSharedSPBListUBO;


/**
 * Render thread buffer object related.
 */
class deoglRTBufferObject{
public:
	/** Shared VBO Lists. */
	enum eSharedVBOLists{
		/** List for static models. */
		esvbolStaticModel,
		
		/** List for static models with weight index. */
		esvbolStaticModelWeight,
		
		/** List for static models with one additional texture coordinate set. */
		esvbolStaticModelTCS1,
		
		/** List for static models with two additional texture coordinate sets. */
		esvbolStaticModelTCS2,
		
		/** List for simple models with one bone per vertex or a max of 2 weights per vertex. */
		esvbolSimpleModel,
		
		/** List for model position weight indices. */
		esvbolModelPositionWeightIndices,
		
		/** List for model normal and tangent calculation. */
		esvbolModelCalcNormalTangent,
		
		/** List for model write skinned vertices. */
		esvbolModelWriteSkinnedVBO,
		
		/** List for model vertex position sets. */
		esvbolModelVertexPositionSets,
		
		/** List for static occlusion meshes. */
		esvbolStaticOcclusionMesh,
		
		/** List for mathematical shapes. */
		esvbolMathShapes,
		
		/** List for canvas paint. */
		esvbolCanvasPaint
	};
	
	/** Shapes. */
	enum eShapes{
		/** Sphere shape. */
		esSphere,
		
		/** Box shape. */
		esBox,
		
		/** Cylinder shape. */
		esCylinder,
		
		/** Capsule shape. */
		esCapsule
	};
	
	/** Shared SPB Lists. */
	enum eSharedSPBLists{
		/** Skin instances UBO based. */
		esspblSkinInstanceUBO,
		
		/** Skin instances SSBO based. */
		esspblSkinInstanceSSBO,
		
		/** Occlusion Mesh instances UBO based. */
		esspblOccMeshInstanceUBO,
		
		/** Occlusion Mesh instances SSBO based. */
		esspblOccMeshInstanceSSBO,
		
		/** Skin textures UBO based. */
		esspblSkinTextureUBO,
		
		/** Skin textures SSBO based. */
		esspblSkinTextureSSBO
	};
	
private:
	deoglRenderThread &pRenderThread;
	deoglShapeManager *pShapeManager;
	deoglSharedVBOList *pSharedVBOListByType[esvbolCanvasPaint + 1];
	deoglSharedVBOListList *pSharedVBOListList;
	deoglSharedSPBList *pSharedSPBList[esspblSkinTextureSSBO + 1];
	
	int pInstanceArraySizeUBO;
	int pInstanceArraySizeSSBO;
	
	deoglSPBlockUBO::Ref pLayoutSkinInstanceUBO;
	deoglSPBlockSSBO::Ref pLayoutSkinInstanceSSBO;
	deoglSPBlockUBO::Ref pLayoutOccMeshInstanceUBO;
	deoglSPBlockSSBO::Ref pLayoutOccMeshInstanceSSBO;
	deoglSPBlockUBO::Ref pLayoutSkinTextureUBO;
	deoglSPBlockSSBO::Ref pLayoutSkinTextureSSBO;
	
	deoglSharedSPBListUBO *pBillboardSPBListUBO;
	deoglSharedSPBRTIGroupList::Ref pBillboardRTIGroups;
	
	char *pTemporaryVBOData;
	int pTemporaryVBODataSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread buffer object related. */
	deoglRTBufferObject(deoglRenderThread &renderThread);
	
	/** Clean up render thread buffer object related. */
	~deoglRTBufferObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Initialize. Required due to a self-reference in buffer object. */
	void Init();
	
	
	
	/** Shared vbo list list. */
	inline deoglSharedVBOListList &GetSharedVBOListList() const{ return *pSharedVBOListList; }
	
	/** Shared vbo list by type. */
	deoglSharedVBOList &GetSharedVBOListForType(eSharedVBOLists type) const;
	
	/** Shared spb list by type. */
	deoglSharedSPBList &GetSharedSPBList(eSharedSPBLists type) const;
	
	/** Maximum size of SPB Instance array or 0 unlimited. */
	inline int GetInstanceArraySizeUBO() const{ return pInstanceArraySizeUBO; }
	
	/** Maximum size of SPB Instance array or 0 unlimited. */
	inline int GetInstanceArraySizeSSBO() const{ return pInstanceArraySizeSSBO; }
	
	/** Shared shader parameter block layout for skin instances. */
	inline const deoglSPBlockUBO::Ref &GetLayoutSkinInstanceUBO() const{ return pLayoutSkinInstanceUBO; }
	
	/** Shared shader parameter block layout for skin instances. */
	inline const deoglSPBlockSSBO::Ref &GetLayoutSkinInstanceSSBO() const{ return pLayoutSkinInstanceSSBO; }
	
	/** Occlusion mesh shared shader parameter block layout for skin instances. */
	inline const deoglSPBlockUBO::Ref &GetLayoutOccMeshInstanceUBO() const{ return pLayoutOccMeshInstanceUBO; }
	
	/** Occlusion Mesh shared shader parameter block layout for skin instances. */
	inline const deoglSPBlockSSBO::Ref &GetLayoutOccMeshInstanceSSBO() const{ return pLayoutOccMeshInstanceSSBO; }
	
	/** Shared shader parameter block layout for skin textures. */
	inline const deoglSPBlockUBO::Ref &GetLayoutSkinTextureUBO() const{ return pLayoutSkinTextureUBO; }
	
	/** Shared shader parameter block layout for skin textures. */
	inline const deoglSPBlockSSBO::Ref &GetLayoutSkinTextureSSBO() const{ return pLayoutSkinTextureSSBO; }
	
	/** Billboard SPB list UBO. */
	inline deoglSharedSPBListUBO &GetBillboardSPBListUBO() const{ return *pBillboardSPBListUBO; }
	
	/** Billboard RTI Groups. */
	inline deoglSharedSPBRTIGroupList &GetBillboardRTIGroups(){ return pBillboardRTIGroups; }
	
	/** Shape manager. */
	inline deoglShapeManager &GetShapeManager() const{ return *pShapeManager; }
	
	/** Temporary vbo data large enough to contain the requested amount of bytes. */
	char *GetTemporaryVBOData(int size);
	/*@}*/
	
private:
	void pCleanUp();
	void pCleanUpReports();
	
	void pCreateLayoutSkinInstance();
	void pCreateLayoutOccMeshInstance();
	void pCreateLayoutTextureInstance();
	void pCreateLayoutInstanceIndex();
	void pCreateSharedVBOLists();
	void pCreateSharedSPBLists();
	void pCreateShapes();
};

#endif
