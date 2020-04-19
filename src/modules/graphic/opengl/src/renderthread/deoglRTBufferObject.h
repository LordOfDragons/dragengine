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

#ifndef _DEOGLRRTBUFFEROBJECT_H_
#define _DEOGLRRTBUFFEROBJECT_H_

#include "../deoglBasics.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroupList.h"

class deoglRenderThread;
class deoglSharedVBOList;
class deoglSharedVBOListList;
class deoglShapeManager;
class deoglSharedSPBList;
class deoglSPBlockUBO;
class deoglSPBlockSSBO;
class deoglSharedSPBListUBO;


/**
 * \brief Render thread buffer object related.
 */
class deoglRTBufferObject{
public:
	/** \brief Shared VBO Lists. */
	enum eSharedVBOLists{
		/** \brief List for static models. */
		esvbolStaticModel,
		
		/** \brief List for static models with weight index. */
		esvbolStaticModelWeight,
		
		/** \brief List for static models with one additional texture coordinate set. */
		esvbolStaticModelTCS1,
		
		/** \brief List for static models with two additional texture coordinate sets. */
		esvbolStaticModelTCS2,
		
		/** \brief List for simple models with one bone per vertex or a max of 2 weights per vertex. */
		esvbolSimpleModel,
		
		/** \brief List for model position weight indices. */
		esvbolModelPositionWeightIndices,
		
		/** \brief List for model normal and tangent calculation. */
		esvbolModelCalcNormalTangent,
		
		/** \brief List for model write skinned vertices. */
		esvbolModelWriteSkinnedVBO,
		
		/** \brief List for static occlusion meshes. */
		esvbolStaticOcclusionMesh,
		
		/** \brief List for mathematical shapes. */
		esvbolMathShapes,
		
		/** \brief List for canvas paint. */
		esvbolCanvasPaint
	};
	
	/** \brief Shapes. */
	enum eShapes{
		/** \brief Sphere shape. */
		esSphere,
		
		/** \brief Box shape. */
		esBox,
		
		/** \brief Cylinder shape. */
		esCylinder,
		
		/** \brief Capsule shape. */
		esCapsule
	};
	
	/** \brief Shared SPB Lists. */
	enum eSharedSPBLists{
		/** \brief Skin instances UBO based. */
		esspblSkinInstanceUBO,
		
		/** \brief Skin instances SSBO based. */
		esspblSkinInstanceSSBO,
		
		/** \brief Occlusion Mesh instances UBO based. */
		esspblOccMeshInstanceUBO,
		
		/** \brief Occlusion Mesh instances SSBO based. */
		esspblOccMeshInstanceSSBO
	};
	
private:
	deoglRenderThread &pRenderThread;
	deoglShapeManager *pShapeManager;
	deoglSharedVBOList *pSharedVBOListByType[ esvbolCanvasPaint + 1 ];
	deoglSharedVBOListList *pSharedVBOListList;
	deoglSharedSPBList *pSharedSPBList[ esspblOccMeshInstanceSSBO + 1 ];
	
	deoglSPBlockUBO *pLayoutSkinInstanceUBO;
	int pInstanceArraySizeUBO;
	
	deoglSPBlockSSBO *pLayoutSkinInstanceSSBO;
	int pInstanceArraySizeSSBO;
	
	deoglSPBlockUBO *pLayoutOccMeshInstanceUBO;
	int pOccMeshInstanceArraySizeUBO;
	
	deoglSPBlockSSBO *pLayoutOccMeshInstanceSSBO;
	int pOccMeshInstanceArraySizeSSBO;
	
	deoglSharedSPBListUBO *pBillboardSPBListUBO;
	deoglSharedSPBRTIGroupList pBillboardRTIGroups;
	
	char *pTemporaryVBOData;
	int pTemporaryVBODataSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread buffer object related. */
	deoglRTBufferObject( deoglRenderThread &renderThread );
	
	/** \brief Clean up render thread buffer object related. */
	~deoglRTBufferObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize. Required due to a self-reference in buffer object. */
	void Init();
	
	
	
	/** \brief Shared vbo list list. */
	inline deoglSharedVBOListList &GetSharedVBOListList() const{ return *pSharedVBOListList; }
	
	/** \brief Shared vbo list by type. */
	deoglSharedVBOList &GetSharedVBOListForType( eSharedVBOLists type ) const;
	
	/** \brief Shared spb list by type. */
	deoglSharedSPBList &GetSharedSPBList( eSharedSPBLists type ) const;
	
	/** \brief Shared shader parameter block layout for skin instances. */
	inline deoglSPBlockUBO *GetLayoutSkinInstanceUBO() const{ return pLayoutSkinInstanceUBO; }
	
	/** \brief Maximum size of SPB Instance array or 0 unlimited. */
	inline int GetInstanceArraySizeUBO() const{ return pInstanceArraySizeUBO; }
	
	/** \brief Shared shader parameter block layout for skin instances. */
	inline deoglSPBlockSSBO *GetLayoutSkinInstanceSSBO() const{ return pLayoutSkinInstanceSSBO; }
	
	/** \brief Maximum size of SPB Instance array or 0 unlimited. */
	inline int GetInstanceArraySizeSSBO() const{ return pInstanceArraySizeSSBO; }
	
	/** \brief Occlusion mesh shared shader parameter block layout for skin instances. */
	inline deoglSPBlockUBO *GetLayoutOccMeshInstanceUBO() const{ return pLayoutOccMeshInstanceUBO; }
	
	/** \brief Maximum size of Occlusion Mesh SPB Instance array or 0 unlimited. */
	inline int GetOccMeshInstanceArraySizeUBO() const{ return pOccMeshInstanceArraySizeUBO; }
	
	/** \brief Occlusion Mesh shared shader parameter block layout for skin instances. */
	inline deoglSPBlockSSBO *GetLayoutOccMeshInstanceSSBO() const{ return pLayoutOccMeshInstanceSSBO; }
	
	/** \brief Maximum size of Occlusion Mesh SPB Instance array or 0 unlimited. */
	inline int GetOccMeshInstanceArraySizeSSBO() const{ return pOccMeshInstanceArraySizeSSBO; }
	
	/** \brief Shape manager. */
	inline deoglShapeManager &GetShapeManager() const{ return *pShapeManager; }
	
	/** \brief Billboard SPB list UBO. */
	inline deoglSharedSPBListUBO &GetBillboardSPBListUBO() const{ return *pBillboardSPBListUBO; }
	
	/** \brief Billboard RTI Groups. */
	inline deoglSharedSPBRTIGroupList &GetBillboardRTIGroups(){ return pBillboardRTIGroups; }
	
	/** \brief Temporary vbo data large enough to contain the requested amount of bytes. */
	char *GetTemporaryVBOData( int size );
	/*@}*/
	
private:
	void pCleanUp();
	void pCleanUpReports();
	
	void pCreateLayoutSkinInstance();
	void pCreateLayoutOccMeshInstance();
	void pCreateLayoutInstanceIndex();
	void pCreateSharedVBOLists();
	void pCreateSharedSPBLists();
	void pCreateShapes();
};

#endif
