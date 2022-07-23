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

#include "deoglOcclusionMesh.h"
#include "deoglROcclusionMesh.h"
#include "deoglDynamicOcclusionMesh.h"
#include "../../component/deoglRComponent.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../model/deoglRModel.h"
#include "../../utils/bvh/deoglBVH.h"
#include "../../utils/bvh/deoglBVHNode.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../vbo/deoglSharedVBO.h"
#include "../../vbo/deoglSharedVBOList.h"
#include "../../vbo/deoglSharedVBOListList.h"
#include "../../vbo/deoglVBOAttribute.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshBone.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshWeight.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshVertex.h>



// Class deoglDynamicOcclusionMesh
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicOcclusionMesh::deoglDynamicOcclusionMesh( deoglRenderThread &renderThread,
deoglROcclusionMesh *occlusionmesh, deoglRComponent *component ) :
pRenderThread( renderThread ),
pBVH( NULL )
{
	if( ! occlusionmesh || ! component ){
		DETHROW( deeInvalidParam );
	}
	
	pOcclusionMesh = occlusionmesh;
	occlusionmesh->AddReference();
	pComponent = component;
	
	pWeights = NULL;
	pWeightCount = 0;
	
	pVertices = NULL;
	
	pVBO = 0;
	pVAO = NULL;
	pVBOData = NULL;
	pVBOVertexCount = 0;
	pVBOVertexSize = 0;
	
	pDirtyOccMesh = true;
	pDirtyVBO = true;
}

deoglDynamicOcclusionMesh::~deoglDynamicOcclusionMesh(){
	pCleanUp();
}



// Management
///////////////

deoglVAO *deoglDynamicOcclusionMesh::GetVAO() const{
	if( pVAO ){
		return pVAO;
		
	}else{
		return pOcclusionMesh->GetVBOBlock()->GetVBO()->GetVAO();
	}
}

void deoglDynamicOcclusionMesh::InvalidateVAO(){
	if( pVAO ){
		delete pVAO;
		pVAO = NULL;
	}
}



void deoglDynamicOcclusionMesh::ComponentStateChanged(){
	pDirtyVBO = true;
	pDirtyOccMesh = true;
}

void deoglDynamicOcclusionMesh::UpdateBoneMappings( const deComponent &component ){
	pBoneMappings.RemoveAll();
	
	if( ! component.GetModel() || ! component.GetOcclusionMesh() ){
		return;
	}
	
	// this mapping goes across some boundaries here. deComponent stores the bone weight matrices
	// in deRig order. the matrices of deoglRComponent though use the model-rig mapping from
	// deComponent. this results in deoglRComponent storing weight matrices in deModel order
	// although fetching them from deComponent which has them in deRig order. hence pBoneMappings
	// has to store the occlusionMesh-model mapping to get the correct result
	// 
	// NOTE this all has one problem. if the model does not contain bones the occlusion mesh
	//      does they can not be mapped. this would require a larger fix since deoglRComponent
	//      needs then to calculate bone weights it does not need for rendering
	const deModel &model = *component.GetModel();
	const deOcclusionMesh &occmesh = *component.GetOcclusionMesh();
	const int occmeshBoneCount = occmesh.GetBoneCount();
	int i;
	
	for( i=0; i<occmeshBoneCount; i++ ){
		pBoneMappings.Add( model.IndexOfBoneNamed( occmesh.GetBoneAt( i ).GetName() ) );
	}
	
	pDirtyVBO = true;
}

void deoglDynamicOcclusionMesh::PrepareForRender(){
	if( ! pDirtyVBO ){
		return;
	}
	
	pBuildArrays();
	pCalculateWeights();
	pTransformVertices();
	pBuildVBO();
	pUpdateVAO();
	
	pDirtyVBO = false;
}

void deoglDynamicOcclusionMesh::PrepareBVH(){
	if( pBVH ){
		return;
	}
	
	   PrepareForRender(); // make sure vertices are transformed
	
	deoglBVH::sBuildPrimitive *primitives = NULL;
	const int faceCount = pOcclusionMesh->GetSingleSidedFaceCount() + pOcclusionMesh->GetDoubleSidedFaceCount();
	
	if( faceCount > 0 ){
		primitives = new deoglBVH::sBuildPrimitive[ faceCount ];
		const unsigned short *corners = pOcclusionMesh->GetCorners();
		int i;
		
		for( i=0; i<faceCount; i++ ){
			deoglBVH::sBuildPrimitive &primitive = primitives[ i ];
			const decVector &v1 = pVertices[ *(corners++) ];
			const decVector &v2 = pVertices[ *(corners++) ];
			const decVector &v3 = pVertices[ *(corners++) ];
			
			primitive.minExtend = v1.Smallest( v2 ).Smallest( v3 );
			primitive.maxExtend = v1.Largest( v2 ).Largest( v3 );
			primitive.center = ( primitive.minExtend + primitive.maxExtend ) * 0.5f;
		}
	}
	
	try{
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
}



// Private Functions
//////////////////////

void deoglDynamicOcclusionMesh::pCleanUp(){
	if( pOcclusionMesh ){
		pOcclusionMesh->FreeReference();
	}
	
	if( pVBOData ){
		delete [] pVBOData;
	}
	if( pVertices ){
		delete [] pVertices;
	}
	if( pWeights ){
		delete [] pWeights;
	}
	if( pVAO ){
		delete pVAO;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pVBO );
}

void deoglDynamicOcclusionMesh::pBuildArrays(){
	if( ! pDirtyOccMesh ){
		return;
	}
	
	const int weightCount = pOcclusionMesh->GetWeightsEntryCount();
	const int vertexCount = pOcclusionMesh->GetVertexCount();
	oglMatrix3x4 *weights = NULL;
	decVector *vertices = NULL;
	
	try{
		if( weightCount > 0 ){
			weights = new oglMatrix3x4[ weightCount ];
		}
		if( vertexCount > 0 ){
			vertices = new decVector[ vertexCount ];
		}
		
	}catch( const deException & ){
		if( vertices ){
			delete [] vertices;
		}
		if( weights ){
			delete [] weights;
		}
		throw;
	}
	
	if( pWeights ){
		delete [] pWeights;
	}
	pWeights = weights;
	
	if( pVertices ){
		delete [] pVertices;
	}
	pVertices = vertices;
	
	pDirtyOccMesh = false;
}

void deoglDynamicOcclusionMesh::pCalculateWeights(){
	const deoglROcclusionMesh::sWeight *weightsEntries = pOcclusionMesh->GetWeightsEntries();
	const oglMatrix3x4 * const boneMatrices = pComponent->GetBoneMatrices();
	const int * const weightsCounts = pOcclusionMesh->GetWeightsCounts();
	const int weightsCount = pOcclusionMesh->GetWeightsCount();
	float factor;
	int w, e;
	
	//pComponent->UpdateBoneMatrices(); // done by sync
	
	for( w=0; w<weightsCount; w++ ){
		oglMatrix3x4 &weightsMatrix = pWeights[ w ];
		const int entryCount = weightsCounts[ w ];
		
		if( entryCount == 0 || ! boneMatrices ){
			weightsMatrix.a11 = 1.0f;
			weightsMatrix.a12 = 0.0f;
			weightsMatrix.a13 = 0.0f;
			weightsMatrix.a14 = 0.0f;
			weightsMatrix.a21 = 0.0f;
			weightsMatrix.a22 = 1.0f;
			weightsMatrix.a23 = 0.0f;
			weightsMatrix.a24 = 0.0f;
			weightsMatrix.a31 = 0.0f;
			weightsMatrix.a32 = 0.0f;
			weightsMatrix.a33 = 1.0f;
			weightsMatrix.a34 = 0.0f;
			
		}else if( entryCount == 1 ){
			const int modelBoneIndex = pBoneMappings.GetAt( weightsEntries->bone );
			
			if( modelBoneIndex != -1 ){
				const oglMatrix3x4 &boneMatrix = boneMatrices[ modelBoneIndex ];
				weightsMatrix.a11 = boneMatrix.a11;
				weightsMatrix.a12 = boneMatrix.a12;
				weightsMatrix.a13 = boneMatrix.a13;
				weightsMatrix.a14 = boneMatrix.a14;
				weightsMatrix.a21 = boneMatrix.a21;
				weightsMatrix.a22 = boneMatrix.a22;
				weightsMatrix.a23 = boneMatrix.a23;
				weightsMatrix.a24 = boneMatrix.a24;
				weightsMatrix.a31 = boneMatrix.a31;
				weightsMatrix.a32 = boneMatrix.a32;
				weightsMatrix.a33 = boneMatrix.a33;
				weightsMatrix.a34 = boneMatrix.a34;
				
			}else{
				weightsMatrix.a11 = 1.0f;
				weightsMatrix.a12 = 0.0f;
				weightsMatrix.a13 = 0.0f;
				weightsMatrix.a14 = 0.0f;
				weightsMatrix.a21 = 0.0f;
				weightsMatrix.a22 = 1.0f;
				weightsMatrix.a23 = 0.0f;
				weightsMatrix.a24 = 0.0f;
				weightsMatrix.a31 = 0.0f;
				weightsMatrix.a32 = 0.0f;
				weightsMatrix.a33 = 1.0f;
				weightsMatrix.a34 = 0.0f;
			}
			
			weightsEntries++;
			
		}else{
			const int rigBoneIndex = pBoneMappings.GetAt( weightsEntries->bone );
			factor = weightsEntries->weight;
			
			if( rigBoneIndex != -1 ){
				const oglMatrix3x4 &boneMatrix = boneMatrices[ rigBoneIndex ];
				weightsMatrix.a11 = boneMatrix.a11 * factor;
				weightsMatrix.a12 = boneMatrix.a12 * factor;
				weightsMatrix.a13 = boneMatrix.a13 * factor;
				weightsMatrix.a14 = boneMatrix.a14 * factor;
				weightsMatrix.a21 = boneMatrix.a21 * factor;
				weightsMatrix.a22 = boneMatrix.a22 * factor;
				weightsMatrix.a23 = boneMatrix.a23 * factor;
				weightsMatrix.a24 = boneMatrix.a24 * factor;
				weightsMatrix.a31 = boneMatrix.a31 * factor;
				weightsMatrix.a32 = boneMatrix.a32 * factor;
				weightsMatrix.a33 = boneMatrix.a33 * factor;
				weightsMatrix.a34 = boneMatrix.a34 * factor;
				
			}else{
				weightsMatrix.a11 = factor;
				weightsMatrix.a12 = 0.0f;
				weightsMatrix.a13 = 0.0f;
				weightsMatrix.a14 = 0.0f;
				weightsMatrix.a21 = 0.0f;
				weightsMatrix.a22 = factor;
				weightsMatrix.a23 = 0.0f;
				weightsMatrix.a24 = 0.0f;
				weightsMatrix.a31 = 0.0f;
				weightsMatrix.a32 = 0.0f;
				weightsMatrix.a33 = factor;
				weightsMatrix.a34 = 0.0f;
			}
			
			weightsEntries++;
			
			for( e=1; e<entryCount; e++ ){
				const int modelBoneIndex = pBoneMappings.GetAt( weightsEntries->bone );
				factor = weightsEntries->weight;
				
				if( modelBoneIndex != -1 ){
					const oglMatrix3x4 &boneMatrix = boneMatrices[ modelBoneIndex ];
					weightsMatrix.a11 += boneMatrix.a11 * factor;
					weightsMatrix.a12 += boneMatrix.a12 * factor;
					weightsMatrix.a13 += boneMatrix.a13 * factor;
					weightsMatrix.a14 += boneMatrix.a14 * factor;
					weightsMatrix.a21 += boneMatrix.a21 * factor;
					weightsMatrix.a22 += boneMatrix.a22 * factor;
					weightsMatrix.a23 += boneMatrix.a23 * factor;
					weightsMatrix.a24 += boneMatrix.a24 * factor;
					weightsMatrix.a31 += boneMatrix.a31 * factor;
					weightsMatrix.a32 += boneMatrix.a32 * factor;
					weightsMatrix.a33 += boneMatrix.a33 * factor;
					weightsMatrix.a34 += boneMatrix.a34 * factor;
					
				}else{
					weightsMatrix.a11 += factor;
					weightsMatrix.a22 += factor;
					weightsMatrix.a33 += factor;
				}
				
				weightsEntries++;
			}
		}
	}
}

void deoglDynamicOcclusionMesh::pTransformVertices(){
	const deoglROcclusionMesh::sVertex * const vertices = pOcclusionMesh->GetVertices();
	const int vertexCount = pOcclusionMesh->GetVertexCount();
	int v;
	
	for( v=0; v<vertexCount; v++ ){
		const deoglROcclusionMesh::sVertex &orgvertex = vertices[ v ];
		decVector &trapos = pVertices[ v ];
		
		if( orgvertex.weight == -1 ){
			trapos = orgvertex.position;
			
		}else{
			const oglMatrix3x4 &matrix = pWeights[ orgvertex.weight ];
			const decVector &orgpos = orgvertex.position;
			
			trapos.x = matrix.a11 * orgpos.x + matrix.a12 * orgpos.y + matrix.a13 * orgpos.z + matrix.a14;
			trapos.y = matrix.a21 * orgpos.x + matrix.a22 * orgpos.y + matrix.a23 * orgpos.z + matrix.a24;
			trapos.z = matrix.a31 * orgpos.x + matrix.a32 * orgpos.y + matrix.a33 * orgpos.z + matrix.a34;
		}
	}
}

void deoglDynamicOcclusionMesh::pBuildVBO(){
	const int vertexCount = pOcclusionMesh->GetVertexCount();
	deoglVBOp *newArray = NULL;
	int dataSize;
	
	dataSize = VBO_P_STRIDE * vertexCount;
	
	if( ! pVBO ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
	
	if( vertexCount > pVBOVertexSize ){
		newArray = new deoglVBOp[ vertexCount ];
		if( pVBOData ){
			delete [] pVBOData;
		}
		pVBOData = newArray;
		pVBOVertexSize = vertexCount;
	}
	
	pWriteVBOData();
	
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, pVBOData, GL_STREAM_DRAW ) );
	
	pVBOVertexCount = vertexCount;
}

void deoglDynamicOcclusionMesh::pWriteVBOData(){
	const int vertexCount = pOcclusionMesh->GetVertexCount();
	int i;
	
	for( i=0; i<vertexCount; i++ ){
		pVBOData[ i ].x = ( GLfloat )pVertices[ i ].x;
		pVBOData[ i ].y = ( GLfloat )pVertices[ i ].y;
		pVBOData[ i ].z = ( GLfloat )pVertices[ i ].z;
	}
}

void deoglDynamicOcclusionMesh::pUpdateVAO(){
	if( pVAO || ! pVBO ){
		return;
	}
	
	const deoglSharedVBOBlock &vboBlock = *pOcclusionMesh->GetVBOBlock();
	const deoglVBOLayout &vboLayout = vboBlock.GetVBO()->GetParentList()->GetLayout();
	
	pVAO = new deoglVAO( pRenderThread );
	pVAO->SetIndexType( vboLayout.GetIndexType() );
	OGL_CHECK( pRenderThread, pglBindVertexArray( pVAO->GetVAO() ) );
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
	vboLayout.SetVAOAttributeAt( pRenderThread, 0, 0 ); // pos(0) => vao(0)
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	
	if( vboBlock.GetVBO()->GetIBO() ){
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboBlock.GetVBO()->GetIBO() ) );
	}
	
	OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
	
	pVAO->EnsureRTSVAO();
}
