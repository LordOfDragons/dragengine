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

#ifndef _DEOGLROCCLUSIONMESH_H_
#define _DEOGLROCCLUSIONMESH_H_

#include "../../deoglBasics.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroupList.h"
#include "../../vbo/deoglSharedVBOBlock.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deOcclusionMesh;
class deoglRenderThread;
class deoglSharedSPBListUBO;
class deoglBVH;
class deoglRayTraceField;



/**
 * Occlusion Mesh Peer.
 */
class deoglROcclusionMesh : public deObject{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deoglROcclusionMesh>;
	
	
	/** Mesh weight. */
	struct sWeight{
		int bone;
		float weight;
	};
	
	/** Mesh vertex. */
	struct sVertex{
		decVector position;
		int weight;
	};
	
public:
	deoglRenderThread &pRenderThread;
	decString pFilename;
	
	deoglSharedVBOBlock::Ref pVBOBlock;
	
	decTList<sWeight> pWeightsEntries;
	decTList<int> pWeightsCounts;
	decTList<sVertex> pVertices;
	decTList<unsigned short> pCorners;
	int pSingleSidedFaceCount;
	int pDoubleSidedFaceCount;
	
	deoglSharedSPBListUBO *pSharedSPBListUBO;
	deoglSharedSPBRTIGroupList::Ref pRTIGroupsSingle;
	deoglSharedSPBRTIGroupList::Ref pRTIGroupsDouble;
	
	deoglBVH *pBVH;
	deoglRayTraceField *pRayTraceField;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render occlusion mesh. */
	deoglROcclusionMesh(deoglRenderThread &renderThread, const deOcclusionMesh &occlusionMesh);
	
protected:
	/** Clean up render occlusion mesh. */
	~deoglROcclusionMesh() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Filename for debugging. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** VBO block. */
	inline const deoglSharedVBOBlock::Ref &GetVBOBlock() const{ return pVBOBlock; }
	
	/** Prepare VBO block. */
	void PrepareVBOBlock();
	
	
	
	/** Weights entries. */
	inline const decTList<sWeight> &GetWeightsEntries() const{ return pWeightsEntries; }
	
	/** Weights entries count list. */
	inline const decTList<int> &GetWeightsCounts() const{ return pWeightsCounts; }
	
	/** Vertices. */
	inline const decTList<sVertex> &GetVertices() const{ return pVertices; }
	
	/** Corners. */
	inline const decTList<unsigned short> &GetCorners() const{ return pCorners; }
	
	/**
	 * Single sided face count.
	 * \details Located before the double sided faces.
	 */
	inline int GetSingleSidedFaceCount() const{ return pSingleSidedFaceCount; }
	
	/**
	 * Double sided face count.
	 * \details Located after the single sided faces.
	 */
	inline int GetDoubleSidedFaceCount() const{ return pDoubleSidedFaceCount; }
	
	
	
	/** Shared shader parameter block list using UBO. */
	deoglSharedSPBListUBO &GetSharedSPBListUBO();
	
	/** Render task instance groups for single sided faces. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroupsSingle(){ return pRTIGroupsSingle; }
	
	/** Render task instance groups for double sided fgaces. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroupDouble(){ return pRTIGroupsDouble; }
	
	
	
	/** BVH or NULL. */
	inline deoglBVH *GetBVH() const{ return pBVH; }
	
	/** Build BVH if not build yet. */
	void PrepareBVH();
	
	
	
	/** Ray trace field or NULL. */
	inline deoglRayTraceField *GetRayTraceField() const{ return pRayTraceField; }
	
	/** Prepare ray trace field. */
	void PrepareRayTraceField();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pBuildArrays(const deOcclusionMesh &occlusionMesh);
	void pWriteVBOData();
};

#endif
