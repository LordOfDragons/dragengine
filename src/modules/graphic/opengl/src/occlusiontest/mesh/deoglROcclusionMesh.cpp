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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglROcclusionMesh.h"
#include "../../delayedoperation/deoglDelayedDeletion.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../gi/deoglRayTraceField.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../vbo/deoglSharedVBOList.h"
#include "../../vbo/deoglSharedVBOListList.h"
#include "../../vbo/deoglSharedVBO.h"
#include "../../vbo/deoglVBOAttribute.h"
#include "../../utils/bvh/deoglBVH.h"
#include "../../utils/bvh/deoglBVHNode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshWeight.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshVertex.h>



// Class deoglROcclusionMesh
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglROcclusionMesh::deoglROcclusionMesh( deoglRenderThread &renderThread,
const deOcclusionMesh &occlusionmesh ) :
pRenderThread( renderThread ),
pFilename( occlusionmesh.GetFilename() ),
pSharedSPBListUBO( NULL ),
pBVH( NULL ),
pRayTraceField( NULL )
{
	pVBOBlock = NULL;
	
	pWeightsEntries = NULL;
	pWeightsCounts = NULL;
	pWeightsCount = 0;
	
	pVertices = NULL;
	pVertexCount = 0;
	
	pCorners = NULL;
	pCornerCount = 0;
	pSingleSidedFaceCount = 0;
	pDoubleSidedFaceCount = 0;
	
	try{
		pBuildArrays( occlusionmesh );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, OcclusionMesh );
}

deoglROcclusionMesh::~deoglROcclusionMesh(){
	LEAK_CHECK_FREE( pRenderThread, OcclusionMesh );
	pCleanUp();
}



// Management
///////////////

deoglSharedVBOBlock *deoglROcclusionMesh::GetVBOBlock(){
	if( ! pVBOBlock ){
		deoglSharedVBOList &svbolist = pRenderThread.GetBufferObject().
			GetSharedVBOListForType( deoglRTBufferObject::esvbolStaticOcclusionMesh );
		
		if( pVertexCount > svbolist.GetMaxPointCount() ){
			pRenderThread.GetLogger().LogInfoFormat( "OcclusionMesh(%s): Too many points (%i) "
				"to fit into shared VBOs. Using over-sized VBO (performance not optimal).",
				pFilename.GetString(), pVertexCount );
		}
		
		pVBOBlock = svbolist.AddData( pVertexCount, pCornerCount );
		
		pWriteVBOData();
	}
	
	return pVBOBlock;
}

deoglSharedSPBListUBO &deoglROcclusionMesh::GetSharedSPBListUBO(){
	if( ! pSharedSPBListUBO ){
		pSharedSPBListUBO = new deoglSharedSPBListUBO( pRenderThread,
			pRenderThread.GetBufferObject().GetLayoutOccMeshInstanceUBO() );
	}
	return *pSharedSPBListUBO;
}



void deoglROcclusionMesh::PrepareBVH(){
	if( pBVH ){
		return;
	}
	
	deoglBVH::sBuildPrimitive *primitives = NULL;
	const int faceCount = pSingleSidedFaceCount + pDoubleSidedFaceCount;
	
	if( faceCount > 0 ){
		primitives = new deoglBVH::sBuildPrimitive[ faceCount ];
		unsigned short *corners = pCorners;
		int i;
		
		for( i=0; i<faceCount; i++ ){
			deoglBVH::sBuildPrimitive &primitive = primitives[ i ];
			const sVertex &v1 = pVertices[ *(corners++) ];
			const sVertex &v2 = pVertices[ *(corners++) ];
			const sVertex &v3 = pVertices[ *(corners++) ];
			
			primitive.minExtend = v1.position.Smallest( v2.position ).Smallest( v3.position );
			primitive.maxExtend = v1.position.Largest( v2.position ).Largest( v3.position );
			primitive.center = ( primitive.minExtend + primitive.maxExtend ) * 0.5f;
		}
	}
	
	try{
		// occlusion meshes usually have lower face counts than models. testing different
		// max depth values resulted in 6-8 showing best performance, above 8 somewhat
		// worse performance and below 6 more worse performance. using 6 max depth now
		// since this seems to be the best overall choice
		pBVH = new deoglBVH;
		pBVH->Build( primitives, faceCount, 6 );
		
	}catch( const deException & ){
		if( pBVH ){
			delete pBVH;
			pBVH = NULL;
		}
		if( primitives ){
			delete [] primitives;
		}
		throw;
	}
	
	if( primitives ){
		delete [] primitives;
	}
	
#if 0
	struct PrintBVH{
		deoglRTLogger &logger;
		const sVertex *vertices;
		const unsigned short *corners;
		PrintBVH(deoglRTLogger &logger, const sVertex *vertices, const unsigned short *corners) :
		logger(logger), vertices(vertices), corners(corners){
		}
		void Print(const decString &prefix, const deoglBVH &bvh, const deoglBVHNode &node) const{
			logger.LogInfoFormat("%sNode: (%g,%g,%g)-(%g,%g,%g)", prefix.GetString(),
				node.GetMinExtend().x, node.GetMinExtend().y, node.GetMinExtend().z,
				node.GetMaxExtend().x, node.GetMaxExtend().y, node.GetMaxExtend().z);
			if(node.GetPrimitiveCount() == 0){
				Print(prefix + "L ", bvh, bvh.GetNodeAt(node.GetFirstIndex()));
				Print(prefix + "R ", bvh, bvh.GetNodeAt(node.GetFirstIndex()+1));
			}else{
				for(int i=0; i<node.GetPrimitiveCount(); i++){
					const int index = bvh.GetPrimitiveAt(node.GetFirstIndex()+i);
					const decVector v1 = vertices[corners[index*3]].position;
					const decVector v2 = vertices[corners[index*3+1]].position;
					const decVector v3 = vertices[corners[index*3+2]].position;
					logger.LogInfoFormat("%sP%03d (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)", prefix.GetString(),
						i, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
				}
			}
		}
	};
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfoFormat("OccMesh BVH: %s", pFilename.GetString());
	if(pBVH->GetRootNode()){
		PrintBVH(logger, pVertices, pCorners).Print("", *pBVH, *pBVH->GetRootNode());
	}
#endif
}



void deoglROcclusionMesh::PrepareRayTraceField(){
// 	if( pRayTraceField || ( pSingleSidedFaceCount == 0 && pDoubleSidedFaceCount == 0 ) ){
// 		return;
// 	}
// 	
// 	PrepareBVH();
// 	if( ! pBVH->GetRootNode() ){
// 		return;
// 	}
// 	const deoglBVHNode &rootNode = *pBVH->GetRootNode();
// 	
// 	pRayTraceField = new deoglRayTraceField( pRenderThread );
// 	pRayTraceField->Init( rootNode.GetMinExtend(), rootNode.GetMaxExtend() );
// 	pRayTraceField->RenderField( *this );
}



// Private Functions
//////////////////////

class deoglROcclusionMeshDeletion : public deoglDelayedDeletion{
public:
	deoglSharedVBOBlock *vboBlock;
	deoglSharedSPBListUBO *sharedSPBListUBO;
	
	deoglROcclusionMeshDeletion() :
	vboBlock( NULL ),
	sharedSPBListUBO( NULL ){
	}
	
	virtual ~deoglROcclusionMeshDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( sharedSPBListUBO ){
			delete sharedSPBListUBO;
		}
		if( vboBlock ){
			vboBlock->GetVBO()->RemoveBlock( vboBlock );
			vboBlock->FreeReference();
		}
	}
};

void deoglROcclusionMesh::pCleanUp(){
	if( pBVH ){
		delete pBVH;
	}
	if( pCorners ){
		delete [] pCorners;
	}
	if( pVertices ){
		delete [] pVertices;
	}
	if( pWeightsCounts ){
		delete [] pWeightsCounts;
	}
	if( pWeightsEntries ){
		delete [] pWeightsEntries;
	}
	
	// delayed deletion of opengl containing objects
	deoglROcclusionMeshDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglROcclusionMeshDeletion;
		delayedDeletion->vboBlock = pVBOBlock;
		delayedDeletion->sharedSPBListUBO = pSharedSPBListUBO;
		pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pRenderThread.GetLogger().LogException( e );
		throw;
	}
}



void deoglROcclusionMesh::pBuildArrays( const deOcclusionMesh &occlusionMesh ){
	const int meshWeightGroupCount = occlusionMesh.GetWeightGroupCount();
	const int meshWeightCount = occlusionMesh.GetWeightCount();
	const int meshVertexCount = occlusionMesh.GetVertexCount();
	const int meshFaceCount = occlusionMesh.GetFaceCount();
	const int meshSingleFaceCount = meshFaceCount - occlusionMesh.GetDoubleSidedFaceCount();
	const deOcclusionMeshWeight * const meshWeights = occlusionMesh.GetWeights();
	const deOcclusionMeshVertex * const meshVertices = occlusionMesh.GetVertices();
	const unsigned short * const meshCorners = occlusionMesh.GetCorners();
	const int * const meshWeightGroups = occlusionMesh.GetWeightGroups();
	const unsigned short * const meshFaces = occlusionMesh.GetFaces();
	int wg, ws, i, j, cornerIndex, meshCornerIndex;
	
	// calculate the number of weight sets
	pWeightsCount = 0;
	for( wg=0; wg<meshWeightGroupCount; wg++ ){
		pWeightsCount += meshWeightGroups[ wg ];
	}
	
	// create weights counts array
	if( pWeightsCount > 0 ){
		pWeightsCounts = new int[ pWeightsCount ];
		ws = 0;
		
		for( wg=0; wg<meshWeightGroupCount; wg++ ){
			for( i=0; i<meshWeightGroups[ wg ]; i++ ){
				pWeightsCounts[ ws++ ] = wg + 1;
			}
		}
	}
	
	// create weights entries array and fill in the weights counts
	pWeightsEntryCount = meshWeightCount;
	
	if( meshWeightCount > 0 ){
		pWeightsEntries = new sWeight[ meshWeightCount ];
		
		for( ws=0; ws<meshWeightCount; ws++ ){
			pWeightsEntries[ ws ].bone = meshWeights[ ws ].GetBone();
			pWeightsEntries[ ws ].weight = meshWeights[ ws ].GetWeight();
		}
	}
	
	// create vertices array
	if( meshVertexCount > 0 ){
		pVertices = new sVertex[ meshVertexCount ];
		pVertexCount = meshVertexCount;
		
		for( i=0; i<meshVertexCount; i++ ){
			pVertices[ i ].position = meshVertices[ i ].GetPosition();
			pVertices[ i ].weight = meshVertices[ i ].GetWeightSet();
		}
	}
	
	// determine the total number of corners required
	pCornerCount = 0;
	
	for( i=0; i<meshFaceCount; i++ ){
		const int faceCornerCount = ( int )meshFaces[ i ];
		
		if( faceCornerCount > 2 ){
			pCornerCount += ( faceCornerCount - 2 ) * 3;
		}
	}
	
	if( pCornerCount > 0 ){
		pCorners = new unsigned short[ pCornerCount ];
	}
	
	// add faces
	pSingleSidedFaceCount = 0;
	pDoubleSidedFaceCount = 0;
	meshCornerIndex = 0;
	cornerIndex = 0;
	
	for( i=0; i<meshFaceCount; i++ ){
		const int faceCornerCount = ( int )meshFaces[ i ];
		
		if( faceCornerCount > 2 ){
			const unsigned short firstFaceCorner = meshCorners[ meshCornerIndex++ ];
			unsigned short thirdFaceCorner = meshCorners[ meshCornerIndex++ ];
			unsigned short secondFaceCorner;
			
			for( j=2; j<faceCornerCount; j++ ){
				secondFaceCorner = thirdFaceCorner;
				thirdFaceCorner = ( int )meshCorners[ meshCornerIndex++ ];
				
				pCorners[ cornerIndex++ ] = thirdFaceCorner;
				pCorners[ cornerIndex++ ] = secondFaceCorner;
				pCorners[ cornerIndex++ ] = firstFaceCorner;
			}
			
			if( i < meshSingleFaceCount ){
				pSingleSidedFaceCount += faceCornerCount - 2;
				
			}else{
				pDoubleSidedFaceCount += faceCornerCount - 2;
			}
			
		}else{
			meshCornerIndex += faceCornerCount;
		}
	}
}

void deoglROcclusionMesh::pWriteVBOData(){
	if( ! pVBOBlock ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglVBOLayout &layout = pVBOBlock->GetVBO()->GetParentList()->GetLayout();
	unsigned char * const data = pVBOBlock->GetData();
	const int stride = layout.GetStride();
	unsigned char *ptrData;
	GLfloat *ptrFloat;
	int i;
	
	// write positions
	const deoglVBOAttribute &attrPos = layout.GetAttributeAt( 0 );
	ptrData = data + attrPos.GetOffset();
	
	for( i=0; i<pVertexCount; i++ ){
		const decVector &position = pVertices[ i ].position;
		
		ptrFloat = ( GLfloat* )ptrData;
		ptrFloat[ 0 ] = ( GLfloat )position.x;
		ptrFloat[ 1 ] = ( GLfloat )position.y;
		ptrFloat[ 2 ] = ( GLfloat )position.z;
		ptrData += stride;
	}
	
	// write indices
	if( pVBOBlock->GetIndexCount() > 0 ){
		switch( layout.GetIndexType() ){
		case deoglVBOLayout::eitUnsignedInt:{
			GLuint * const indexData = ( GLuint* )pVBOBlock->GetIndexData();
			for( i=0; i<pCornerCount; i++ ){
				indexData[ i ] = ( GLuint)pCorners[ i ];
			}
			}break;
			
		case deoglVBOLayout::eitUnsignedShort:{
			GLushort * const indexData = ( GLushort* )pVBOBlock->GetIndexData();
			for( i=0; i<pCornerCount; i++ ){
				indexData[ i ] = ( GLushort )pCorners[ i ];
			}
			}break;
			
		default:
			DETHROW( deeInvalidParam );
		}
	}
}
