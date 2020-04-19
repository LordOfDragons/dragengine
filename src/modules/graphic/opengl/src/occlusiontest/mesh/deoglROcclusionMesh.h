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

#ifndef _DEOGLROCCLUSIONMESH_H_
#define _DEOGLROCCLUSIONMESH_H_

#include "../../deoglBasics.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroupList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deOcclusionMesh;
class deoglRenderThread;
class deoglSharedVBOBlock;
class deoglSharedSPBListUBO;



/**
 * \brief Occlusion Mesh Peer.
 */
class deoglROcclusionMesh : public deObject{
public:
	/** \brief Mesh weight. */
	struct sWeight{
		int bone;
		float weight;
	};
	
	/** \brief Mesh vertex. */
	struct sVertex{
		decVector position;
		int weight;
	};
	
public:
	deoglRenderThread &pRenderThread;
	decString pFilename;
	
	deoglSharedVBOBlock *pVBOBlock;
	
	sWeight *pWeightsEntries;
	int pWeightsEntryCount;
	int *pWeightsCounts;
	int pWeightsCount;
	
	sVertex *pVertices;
	int pVertexCount;
	
	unsigned short *pCorners;
	int pCornerCount;
	int pSingleSidedFaceCount;
	int pDoubleSidedFaceCount;
	
	deoglSharedSPBListUBO *pSharedSPBListUBO;
	deoglSharedSPBRTIGroupList pRTIGroupsSingle;
	deoglSharedSPBRTIGroupList pRTIGroupsDouble;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new render occlusion mesh. */
	deoglROcclusionMesh( deoglRenderThread &renderThread, const deOcclusionMesh &occlusionMesh );
	
	/** \brief Clean up render occlusion mesh. */
	virtual ~deoglROcclusionMesh();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief VBO block. */
	deoglSharedVBOBlock *GetVBOBlock();
	
	
	
	/** \brief Weights entries. */
	inline sWeight *GetWeightsEntries() const{ return pWeightsEntries; }
	
	/** \brief Number of weights entries. */
	inline int GetWeightsEntryCount() const{ return pWeightsEntryCount; }
	
	/** \brief Weights entries count list. */
	inline int *GetWeightsCounts() const{ return pWeightsCounts; }
	
	/** \brief Number of weights. */
	inline int GetWeightsCount() const{ return pWeightsCount; }
	
	
	
	/** \brief Vertices. */
	inline sVertex *GetVertices() const{ return pVertices; }
	
	/** \brief Vertex count. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	
	
	/** \brief Corners. */
	inline unsigned short *GetCorners() const{ return pCorners; }
	
	/** \brief Number of corners. */
	inline int GetCornerCount() const{ return pCornerCount; }
	
	
	
	/**
	 * \brief Single sided face count.
	 * \details Located before the double sided faces.
	 */
	inline int GetSingleSidedFaceCount() const{ return pSingleSidedFaceCount; }
	
	/**
	 * \brief Double sided face count.
	 * \details Located after the double sided faces.
	 */
	inline int GetDoubleSidedFaceCount() const{ return pDoubleSidedFaceCount; }
	
	
	
	/** \brief Shared shader parameter block list using UBO. */
	deoglSharedSPBListUBO &GetSharedSPBListUBO();
	
	/** \brief Render task instance groups for single sided faces. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroupsSingle(){ return pRTIGroupsSingle; }
	
	/** \brief Render task instance groups for double sided fgaces. */
	inline deoglSharedSPBRTIGroupList &GetRTIGroupDouble(){ return pRTIGroupsDouble; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pBuildArrays( const deOcclusionMesh &occlusionMesh );
	void pWriteVBOData();
};

#endif
