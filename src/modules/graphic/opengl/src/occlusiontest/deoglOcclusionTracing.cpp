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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglOcclusionTracing.h"
#include "mesh/deoglROcclusionMesh.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/face/deoglModelFace.h"
#include "../model/texture/deoglModelTexture.h"
#include "../rendering/deoglRenderOcclusion.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionBox.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Struct deoglOcclusionTracing::sOccMesh
///////////////////////////////////////////

void deoglOcclusionTracing::sOccMesh::Clear(){
	occlusionMesh = NULL;
	modelStatic = NULL;
}



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTracing::deoglOcclusionTracing( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pProbeSpacing( 1.0f, 1.0f, 1.0f ),
pProbeSpacingInv( 1.0f / pProbeSpacing.x, 1.0f / pProbeSpacing.y, 1.0f / pProbeSpacing.z ),
pProbeCount( 32, 8, 32 ),
pGridCoordClamp( pProbeCount - decPoint3( 1, 1, 1 ) ),
pProbeOrigin( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) * -0.5f ) ),
pPositionClamp( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) ) ),
pStrideProbeCount( pProbeCount.x * pProbeCount.z ),
pRealProbeCount( pStrideProbeCount * pProbeCount.y ),
pMaxRaysPerProbe( 256 ),
pRaysPerProbe( 64 ),
pMaxUpdateProbeCount( 1024 ), // keep at power of two
pProbesPerLine( 4 ), // 16 for 4k probes
pOcclusionMapSize( 8 ),
pDistanceMapSize( 16 ),
pMaxProbeDistance( 4.0f ),
pDepthSharpness( 50.0f ),
pHysteresis( 0.9f ), // 0.98 (paper)
pNormalBias( 0.25f ),
pEnergyPreservation( 0.85f ),
pMaxDetectionRange( 10.0f ),
pDetectionBox( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) * 0.5f )
	+ decVector( pMaxDetectionRange, pMaxDetectionRange, pMaxDetectionRange ) ),
pOccMeshes( NULL ),
pOccMeshCount( 0 ),
pOccMeshSize( 0 ),
pOccMeshInstances( NULL ),
pOccMeshInstanceCount( 0 ),
pOccMeshInstanceSize( 0 ),
pBVHBuildPrimitives( NULL ),
pBVHBuildPrimitiveSize( 0 ),
pTBONodeBox( renderThread, 4 ),
pTBOIndex( renderThread, 2 ),
pTBOInstance( renderThread, 1 ),
pTBOMatrix( renderThread, 4 ),
pTBOFace( renderThread, 4 ),
pTBOVertex( renderThread, 4 ),
pBVHInstanceRootNode( 0 ),
pSphericalFibonacci( NULL ),
pUBOTracing( NULL ),
pSizeTexOcclusion( pOcclusionMapSize ),
pSizeTexDistance( pDistanceMapSize ),

pOcclusionMapScale( 1.0f / ( ( pSizeTexOcclusion + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexOcclusion + 2 ) * pProbeCount.z + 2 ) ),
pDistanceMapScale( 1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.z + 2 ) )
{
	try{
		pSphericalFibonacci = new decVector[ pRaysPerProbe ];
		pUBOTracing = new deoglSPBlockUBO( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglOcclusionTracing::~deoglOcclusionTracing(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING 1

decPoint3 deoglOcclusionTracing::ProbeIndex2GridCoord( int index ) const{
	decPoint3 coord;
	coord.y = index / pStrideProbeCount;
	index -= pStrideProbeCount * coord.y;
	coord.z = index / pProbeCount.x;
	coord.x = index - pProbeCount.x * coord.z;
	return coord;
}

int deoglOcclusionTracing::GridCoord2ProbeIndex( const decPoint3 &coord ) const{
	return pStrideProbeCount * coord.y + pProbeCount.x * coord.z + coord.x;
}

decVector deoglOcclusionTracing::Grid2Local( const decPoint3 &coord ) const{
	return pProbeSpacing.Multiply( decVector( coord ) ) + pProbeOrigin;
}

/*
decPoint3 deoglOcclusionTracing::World2Grid( const decDVector &p ) const{
	return decPoint3( p.x / pProbeSpacing.x + 0.5, p.y / pProbeSpacing.y + 0.5, p.z / pProbeSpacing.z + 0.5 );
}

decDVector deoglOcclusionTracing::Grid2World( const decPoint3 &p ) const{
	return decDVector( pProbeSpacing.x * p.x, pProbeSpacing.y * p.y, pProbeSpacing.z * p.z );
}
*/



deoglSPBlockUBO &deoglOcclusionTracing::GetUBOTracing(){
	deoglSPBlockUBO &ubo = *pUBOTracing;
	
	if( ubo.GetParameterCount() == 0 ){
		// memory consumption:
		// 
		// - 5 * 16 = 80 (fixed size 4-component blocks)
		// - ( maxProbeCount / 4 ) * 16 (index array)
		// - maxProbeCount * 16 (probe position array)
		// - maxRaysPerProbe * 16 (ray direction array)
		// 
		// maxProbeCount is guaranteed to be multiple of 4 so index array can be reduced to
		// - maxProbeCount * 4
		// 
		// maxRaysPerProbe is guaranteed to be at least 1
		// 
		// size = 80 + 16 * ( maxRaysPerProbe + maxProbeCount * 1.25 )
		// 
		// - base size: 80
		// - rays:      1'024 (64), 4096 (256)
		// - probes:    5'120 (256), 20'480 (1024), 40'960 (2048), 81'920 (4096)
		ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ubo.SetParameterCount( eutpRayDirection + 1 );
		ubo.GetParameterAt( eutpSampleImageScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		ubo.GetParameterAt( eutpProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpRaysPerProbe ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpGridProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
		ubo.GetParameterAt( eutpProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpGridProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.GetParameterAt( eutpBVHInstanceRootNode ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpOcclusionMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		ubo.GetParameterAt( eutpDistanceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		ubo.GetParameterAt( eutpOcclusionMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpDistanceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpMaxProbeDistance ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		ubo.GetParameterAt( eutpDepthSharpness ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		ubo.GetParameterAt( eutpFieldOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.GetParameterAt( eutpProbeIndex ).SetAll( deoglSPBParameter::evtInt, 4, 1, pMaxUpdateProbeCount / 4 ); // ivec4
		ubo.GetParameterAt( eutpProbePosition ).SetAll( deoglSPBParameter::evtFloat, 4, 1, pMaxUpdateProbeCount ); // vec4
		ubo.GetParameterAt( eutpRayDirection ).SetAll( deoglSPBParameter::evtFloat, 3, 1, pRaysPerProbe ); // vec3
		ubo.MapToStd140();
		ubo.SetBindingPoint( 0 );
		
		pInitSphericalFibonacci();
	}
	
	return ubo;
}

void deoglOcclusionTracing::PrepareRayTracing( deoglRWorld &world, const decDVector &position ){
	pBVHInstances.Clear();
	pOccMeshInstanceCount = 0;
	pOccMeshCount = 0;
	
	pTBOVertex.Clear();
	pTBOFace.Clear();
	pTBOMatrix.Clear();
	pTBOInstance.Clear();
	pTBOIndex.Clear();
	pTBONodeBox.Clear();
	pBVHInstanceRootNode = -1;
	
	pFindComponents( world, position );
	//pAddOcclusionMeshes( position );
	pAddStaticModels( position );
	pWriteTBOs( position );
}



// Private Functions
//////////////////////

void deoglOcclusionTracing::pCleanUp(){
	if( pBVHBuildPrimitives ){
		delete [] pBVHBuildPrimitives;
	}
	if( pOccMeshInstances ){
		delete [] pOccMeshInstances;
	}
	if( pOccMeshes ){
		delete [] pOccMeshes;
	}
	if( pUBOTracing ){
		pUBOTracing->FreeReference();
	}
	if( pSphericalFibonacci ){
		delete [] pSphericalFibonacci;
	}
}

void deoglOcclusionTracing::pFindComponents( deoglRWorld &world, const decDVector &position ){
	deoglDCollisionBox colbox( position, pDetectionBox );
	pCollideList.Clear();
	pCollideList.AddComponentsColliding( world.GetOctree(), &colbox );
}

void deoglOcclusionTracing::pAddStaticModels( const decDVector &position ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = pCollideList.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglRComponent &component = *pCollideList.GetComponentAt( i )->GetComponent();
		if( ! component.GetModel() || component.GetLODCount() == 0 ){
			continue;
		}
		if( component.GetRenderMode() != deoglRComponent::ermStatic ){
			continue;
		}
		
		const deoglRModel::sExtends &extends = component.GetModel()->GetWeightlessExtends();
		if( ( extends.maximum - extends.minimum ) < decVector( 0.5f, 0.5f, 0.5f ) ){
			continue; // skip small models to improve performance
		}
		
		deoglRComponentLOD &lod = component.GetLODAt( component.GetLODCount() - 1 );
		//component.PrepareRayTraceField();
		pAddStaticModel( ( component.GetMatrix() * matrix ).ToMatrix(), lod );
	}
}

void deoglOcclusionTracing::pAddStaticModel( const decMatrix &matrix, deoglRComponentLOD &lod ){
	// find model
	int indexOccMesh;
	for( indexOccMesh=0; indexOccMesh<pOccMeshCount; indexOccMesh++ ){
		if( pOccMeshes[ indexOccMesh ].modelStatic == &lod ){
			break;
		}
	}
	
	// if model does not exist add it
	if( indexOccMesh == pOccMeshCount ){
		// prepare BVH first just in case something goes wrong
		deoglModelLOD &modelLOD = lod.GetModelLOD();
		modelLOD.PrepareBVH();
		
		if( ! modelLOD.GetBVH()->GetRootNode() ){
			return; // empty model
		}
		
		// add model to list of encountered models
		if( pOccMeshCount == pOccMeshSize ){
			const int newSize = pOccMeshCount + 10;
			sOccMesh * const newArray = new sOccMesh[ newSize ];
			if( pOccMeshes ){
				memcpy( newArray, pOccMeshes, sizeof( sOccMesh ) * pOccMeshCount );
				delete [] pOccMeshes;
			}
			pOccMeshes = newArray;
			pOccMeshSize = newSize;
		}
		
		sOccMesh &occMesh = pOccMeshes[ pOccMeshCount++ ];
		occMesh.Clear();
		occMesh.modelStatic = &lod;
		occMesh.indexVertices = pTBOVertex.GetPixelCount();
		occMesh.indexFaces = pTBOFace.GetPixelCount();
		occMesh.indexNodes = pTBOIndex.GetPixelCount();
		
		// add vertices to TBO in mesh order
		const oglModelPosition * const positions = modelLOD.GetPositions();
		const int positionPoint = modelLOD.GetPositionCount();
		int i;
		
		for( i=0; i<positionPoint; i++ ){
			pTBOVertex.AddVec4( positions[ i ].position, 0.0f );
		}
		
		// add faces to TBOs using primitive mapping from BVH
		const deoglBVH &bvh = *modelLOD.GetBVH();
		const oglModelVertex * const vertices = modelLOD.GetVertices();
		const deoglModelFace * const faces = modelLOD.GetFaces();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		for( i=0; i<primitiveCount; i++ ){
			const deoglModelFace &face = faces[ primitives[ i ] ];
			//const uint32_t doubleSided = modelLOD.GetTextureAt( face.GetTexture() ).GetDoubleSided() ? 0 : 1;
			const uint32_t doubleSided = 1; // always double sided to allow disable probe
			pTBOFace.AddVec4( occMesh.indexVertices + vertices[ face.GetVertex1() ].position,
				occMesh.indexVertices + vertices[ face.GetVertex2() ].position,
				occMesh.indexVertices + vertices[ face.GetVertex3() ].position, doubleSided );
		}
		
		// add BVH to TBOs
		const int nodeCount = bvh.GetNodeCount();
		const deoglBVHNode * const nodes = bvh.GetNodes();
		
		for( i=0; i<nodeCount; i++ ){
			const deoglBVHNode &node = nodes[ i ];
			pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
			pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
			
			if( node.GetPrimitiveCount() == 0 ){
				pTBOIndex.AddVec2( occMesh.indexNodes + node.GetFirstIndex(), 0 );
				
			}else{
				pTBOIndex.AddVec2( occMesh.indexFaces + node.GetFirstIndex(), node.GetPrimitiveCount() );
			}
		}
	}
	
	// add instance to TBO
	if( pOccMeshInstanceCount == pOccMeshInstanceSize ){
		const int newSize = pOccMeshInstanceCount + 10;
		sOccMeshInstance * const newArray = new sOccMeshInstance[ newSize ];
		if( pOccMeshInstances ){
			memcpy( newArray, pOccMeshInstances, sizeof( sOccMeshInstance ) * pOccMeshInstanceCount );
			delete [] pOccMeshInstances;
		}
		pOccMeshInstances = newArray;
		pOccMeshInstanceSize = newSize;
	}
	
	sOccMeshInstance &occMeshInst = pOccMeshInstances[ pOccMeshInstanceCount++ ];
	occMeshInst.indexMesh = indexOccMesh;
	occMeshInst.indexInstance = 0;
	occMeshInst.indexMatrix = 0;
	occMeshInst.matrix = matrix;
}

void deoglOcclusionTracing::pAddOcclusionMeshes( const decDVector &position ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = pCollideList.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglRComponent &component = *pCollideList.GetComponentAt( i )->GetComponent();
		if( component.GetOcclusionMesh() ){
			if( ! component.GetDynamicOcclusionMesh() ){
				component.GetOcclusionMesh()->PrepareRayTraceField();
				if(true) pAddOcclusionMesh( ( component.GetMatrix() * matrix ).ToMatrix(), component.GetOcclusionMesh() );
			}
		}
	}
}

void deoglOcclusionTracing::pAddOcclusionMesh( const decMatrix &matrix, deoglROcclusionMesh *occlusionMesh ){
	// find occlusion mesh
	int indexOccMesh;
	for( indexOccMesh=0; indexOccMesh<pOccMeshCount; indexOccMesh++ ){
		if( pOccMeshes[ indexOccMesh ].occlusionMesh == occlusionMesh ){
			break;
		}
	}
	
	// if occlusion mesh does not exist add it
	if( indexOccMesh == pOccMeshCount ){
		// prepare BVH first just in case something goes wrong
		occlusionMesh->PrepareBVH();
		
		if( ! occlusionMesh->GetBVH()->GetRootNode() ){
			return; // empty occlusion mesh
		}
		
		// add occlusion mesh to list of encountered occlusion meshes
		if( pOccMeshCount == pOccMeshSize ){
			const int newSize = pOccMeshCount + 10;
			sOccMesh * const newArray = new sOccMesh[ newSize ];
			if( pOccMeshes ){
				memcpy( newArray, pOccMeshes, sizeof( sOccMesh ) * pOccMeshCount );
				delete [] pOccMeshes;
			}
			pOccMeshes = newArray;
			pOccMeshSize = newSize;
		}
		
		sOccMesh &occMesh = pOccMeshes[ pOccMeshCount++ ];
		occMesh.Clear();
		occMesh.occlusionMesh = occlusionMesh;
		occMesh.indexVertices = pTBOVertex.GetPixelCount();
		occMesh.indexFaces = pTBOFace.GetPixelCount();
		occMesh.indexNodes = pTBOIndex.GetPixelCount();
		
		// add vertices to TBO in mesh order
		const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh->GetVertices();
		const int vertexCount = occlusionMesh->GetVertexCount();
		int i;
		
		for( i=0; i<vertexCount; i++ ){
			pTBOVertex.AddVec4( vertices[ i ].position, 0.0f );
		}
		
		// add faces to TBOs using primitive mapping from BVH
		const deoglBVH &bvh = *occlusionMesh->GetBVH();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		const int singleSidedFaceCount = occlusionMesh->GetSingleSidedFaceCount();
		const unsigned short * const corners = occlusionMesh->GetCorners();
		
		for( i=0; i<primitiveCount; i++ ){
			const int faceIndex = primitives[ i ];
			const unsigned short * const faceCorners = corners + 3 * faceIndex;
			const uint32_t doubleSided = faceIndex < singleSidedFaceCount ? 0 : 1;
			pTBOFace.AddVec4( occMesh.indexVertices + faceCorners[ 0 ],
				occMesh.indexVertices + faceCorners[ 1 ],
				occMesh.indexVertices + faceCorners[ 2 ], doubleSided );
		}
		
		// add BVH to TBOs
		const int nodeCount = bvh.GetNodeCount();
		const deoglBVHNode * const nodes = bvh.GetNodes();
		
		for( i=0; i<nodeCount; i++ ){
			const deoglBVHNode &node = nodes[ i ];
			pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
			pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
			
			if( node.GetPrimitiveCount() == 0 ){
				pTBOIndex.AddVec2( occMesh.indexNodes + node.GetFirstIndex(), 0 );
				
			}else{
				pTBOIndex.AddVec2( occMesh.indexFaces + node.GetFirstIndex(), node.GetPrimitiveCount() );
			}
		}
	}
	
	// add instance to TBO
	if( pOccMeshInstanceCount == pOccMeshInstanceSize ){
		const int newSize = pOccMeshInstanceCount + 10;
		sOccMeshInstance * const newArray = new sOccMeshInstance[ newSize ];
		if( pOccMeshInstances ){
			memcpy( newArray, pOccMeshInstances, sizeof( sOccMeshInstance ) * pOccMeshInstanceCount );
			delete [] pOccMeshInstances;
		}
		pOccMeshInstances = newArray;
		pOccMeshInstanceSize = newSize;
	}
	
	sOccMeshInstance &occMeshInst = pOccMeshInstances[ pOccMeshInstanceCount++ ];
	occMeshInst.indexMesh = indexOccMesh;
	occMeshInst.indexInstance = 0;
	occMeshInst.indexMatrix = 0;
	occMeshInst.matrix = matrix;
}

#if 0
bool deoglOcclusionTracing::pAddFace( const decVector& v1, const decVector& v2, const decVector& v3, bool doubleSided ){
	const decVector edge1( v2 - v1 );
	const decVector edge2( v3 - v2 );
	const decVector edge3( v1 - v3 );
	const float edge1Len = edge1.Length();
	const float edge2Len = edge2.Length();
	const float edge3Len = edge3.Length();
	if( edge1Len < 0.001f || edge2Len < 0.001f || edge3Len < 0.001f ){
		return false;
	}
	
	const decVector normEdge1( edge1 / edge1Len );
	const decVector normEdge2( edge2 / edge2Len );
	const decVector normEdge3( edge3 / edge3Len );
	
	const decVector faceNormal( normEdge1 % normEdge2 );
	if( faceNormal.IsZero( 0.001f ) ){
		return false;
	}
	
	pTBOPrimitiveData.AddVec3( faceNormal );
	pTBOPrimitiveData.AddFloat( faceNormal * v1 );
	
	const decVector edge1Normal( faceNormal % normEdge1 );
	tbo.AddVec3( edge1Normal );
	tbo.AddFloat( edge1Normal * v1 );
	
	const decVector edge2Normal( faceNormal % normEdge2 );
	tbo.AddVec3( edge2Normal );
	tbo.AddFloat( edge2Normal * v2 );
	
	const decVector edge3Normal( faceNormal % normEdge3 );
	tbo.AddVec3( edge3Normal );
	tbo.AddFloat( edge3Normal * v3 );
	
	return true;
}
#endif

void deoglOcclusionTracing::pWriteTBOs( const decDVector &position ){
	// build BVH from mesh instances. each mesh instance then uses the occlusion mesh bvh
	// with the matrix to do the local tracing. this allows creating occlusion mesh BVH
	// once and then copy it to GPU as required.
	// 
	// NOTE texture formats are restricted so something like RGB32F doesn't even exist.
	//      furthermore 3-component versions exists only with OGL 4.0 onwards
	// 
	// NOTE 16-bit floating point has too little precision causing problems.
	// 
	// requires these TBO to be build:
	// 
	// - TBONodeBox: RGBA32F (stride 2 pixels)
	//   stores bounding box of all bvh nodes. each node uses two pixels: minExtend(0:RGB)
	//   maxExtend(1:RGB). one component has to be wasted in each pixel due to format
	//   restrictions. contains package nodes of all mesh BVH then nodes of instance BVH.
	//   
	// - TBOIndex: RG32UI (stride 1 pixel)
	//   stores bvh node indiced.
	//   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
	//      faces in TBOPrimitiveData (absolute strided index). otherwise points to first child
	//      node in TBONodeBox/TBOIndex (absolute strided index).
	//   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
	//      TBONodeBox/TBOIndex (absolute strided index). for child nodes stays in instance BVH.
	//      for leaf nodes switches to mesh BVH traversal. points into TBOInstance and TBOMatrix.
	// 
	// - TBOInstance: R32UI (stride 1 pixel)
	//   stores instance offsets. bvhIndex(R) is the absolute strided index into TBONodeBox
	//   and TBOIndex with the mesh bvh root node.
	//   
	// - TBOMatrix: RGBA32F (stride 3 pixels)
	//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
	//   
	// - TBOFace: RGBA32UI (stride 1 pixel)
	//   stores mesh faces. vertex1(R) vertex2(G) vertex3(B) doubleSided(A). indices into
	//   TBOVertex. face is doubleSided if doubleSided has value 1 or single sided if value is 0.
	//   
	// - TBOVertex: RGBA32F (stride 1 pixel)
	//   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
	//   
	// requires uniforms:
	// 
	// - uint IndexRootNode: index into TBONodeBox/TBOIndex containing instance BVH root node.
	
	// build instance bvh
	if( pOccMeshInstanceCount > pBVHBuildPrimitiveSize ){
		deoglBVH::sBuildPrimitive * const newArray = new deoglBVH::sBuildPrimitive[ pOccMeshInstanceCount ];
		if( pBVHBuildPrimitives ){
			delete [] pBVHBuildPrimitives;
		}
		pBVHBuildPrimitives = newArray;
		pBVHBuildPrimitiveSize = pOccMeshInstanceCount;
	}
	
	int i;
	for( i=0; i<pOccMeshInstanceCount; i++ ){
		deoglBVH::sBuildPrimitive &primitive = pBVHBuildPrimitives[ i ];
		const sOccMeshInstance &instance = pOccMeshInstances[ i ];
		const sOccMesh &occMesh = pOccMeshes[ instance.indexMesh ];
		const deoglBVHNode *rootNode = NULL;
		if( occMesh.occlusionMesh != NULL ){
			rootNode = occMesh.occlusionMesh->GetBVH()->GetRootNode();
			
		}else if( occMesh.modelStatic != NULL ){
			rootNode = occMesh.modelStatic->GetModelLOD().GetBVH()->GetRootNode();
		}
		
		const decVector &minExtend = rootNode->GetMinExtend();
		const decVector &maxExtend = rootNode->GetMaxExtend();
		const decVector center( ( minExtend + maxExtend ) * 0.5f );
		const decVector halfSize( ( maxExtend - minExtend ) * 0.5f );
		deoglCollisionBox box( instance.matrix * center,
			instance.matrix.GetScale().Multiply( halfSize ), \
			instance.matrix.Normalized().ToQuaternion() );
		deoglCollisionBox enclosing;
		box.GetEnclosingBox( &enclosing );
		
		primitive.center = enclosing.GetCenter();
		primitive.minExtend = primitive.center - enclosing.GetHalfSize();
		primitive.maxExtend = primitive.center + enclosing.GetHalfSize();
	}
	
	pBVHInstances.Build( pBVHBuildPrimitives, pOccMeshInstanceCount, 12 );
	
	// add to TBOs using primitive mapping from BVH
	const int * const primitives = pBVHInstances.GetPrimitives();
	
	for( i=0; i<pOccMeshInstanceCount; i++ ){
		sOccMeshInstance &instance = pOccMeshInstances[ primitives[ i ] ];
		instance.indexMatrix = pTBOMatrix.GetPixelCount() / 3;
		pTBOMatrix.AddMat3x4( instance.matrix.QuickInvert() );
		
		instance.indexInstance = pTBOInstance.GetPixelCount();
		pTBOInstance.AddInt( pOccMeshes[ instance.indexMesh ].indexNodes );
		if(pOccMeshes[instance.indexMesh].modelStatic) printf("instance %d: %s\n", i,
			pOccMeshes[instance.indexMesh].modelStatic->GetComponent().GetModel()->GetFilename().GetString());
	}
	
	// add BVH to TBOs
	const int nodeCount = pBVHInstances.GetNodeCount();
	
	if( nodeCount > 0 ){
		const deoglBVHNode * const nodes = pBVHInstances.GetNodes();
		
		pBVHInstanceRootNode = pTBOIndex.GetPixelCount();
		
		for( i=0; i<nodeCount; i++ ){
			const deoglBVHNode &node = nodes[ i ];
			pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
			pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
			
			if( node.GetPrimitiveCount() == 0 ){
				pTBOIndex.AddVec2( pBVHInstanceRootNode + node.GetFirstIndex(), 0 );
				
			}else{
				pTBOIndex.AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
			}
		}
	}
	
	// update TBOs
	pTBONodeBox.Update();
	pTBOIndex.Update();
	pTBOInstance.Update();
	pTBOMatrix.Update();
	pTBOFace.Update();
	pTBOVertex.Update();
	
	// DEBUG
	//if(pPosition.IsEqualTo(decDVector(-4.619,2.139,34.115),1.0)){
	if(false){
		deoglRTLogger &logger = pRenderThread.GetLogger();
		int i;
		logger.LogInfoFormat("OcclusionTracing: %d OcclusionMeshes", pOccMeshCount);
		for(i=0; i<pOccMeshCount; i++){
			const char * filename = "-";
			if( pOccMeshes[i].occlusionMesh != NULL ){
				filename = pOccMeshes[i].occlusionMesh->GetFilename();
			}else if( pOccMeshes[i].modelStatic != NULL ){
				filename = pOccMeshes[i].modelStatic->GetComponent().GetModel()->GetFilename();
			}
			logger.LogInfoFormat("- %d: indexNodes=%d indexFaces=%d path=%s", i,
				pOccMeshes[i].indexNodes, pOccMeshes[i].indexFaces,filename );
		}
		logger.LogInfoFormat("OcclusionTracing: %d Instances", pOccMeshInstanceCount);
		for(i=0; i<pOccMeshInstanceCount; i++){
			const decDVector p(position + pOccMeshInstances[i].matrix.GetPosition());
			logger.LogInfoFormat("- %d: indexMatrix=%d indexMesh=%d indexInstance=%d position=(%g,%g,%g)",
				i, pOccMeshInstances[i].indexMatrix, pOccMeshInstances[i].indexMesh,
				pOccMeshInstances[i].indexInstance, p.x, p.y, p.z);
		}
		logger.LogInfoFormat("OcclusionTracing: Root Node %d", pBVHInstanceRootNode);
		logger.LogInfo("OcclusionTracing: TBONodeBox");
		pTBONodeBox.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOIndex");
		pTBOIndex.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOInstance");
		pTBOInstance.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOMatrix");
		pTBOMatrix.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOFace");
		pTBOFace.DebugPrint();
		logger.LogInfo("OcclusionTracing: pTBOVertex");
		pTBOVertex.DebugPrint();
		
		struct PrintBVH{
			deoglRTLogger &logger;
			const sOccMeshInstance *instances;
			PrintBVH(deoglRTLogger &logger, const sOccMeshInstance *instances) : logger(logger), instances(instances){
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
						const decVector p(instances[index].matrix.GetPosition());
						const decVector r(instances[index].matrix.Normalized().GetEulerAngles() * RAD2DEG);
						logger.LogInfoFormat("%sP%03d position=(%g,%g,%g) rotation=(%g,%g,%g) mesh=%d",
							prefix.GetString(), i, p.x, p.y, p.z, r.x, r.y, r.z, instances[index].indexMesh);
					}
				}
			}
		};
		logger.LogInfo("OccMesh Instance BVH");
		if(pBVHInstances.GetRootNode()){
			PrintBVH(logger, pOccMeshInstances).Print("", pBVHInstances, *pBVHInstances.GetRootNode());
		}
	}
}

static decVector deoglOcclusionTracing_SphericalFibonacci( float i, float n ){
	const float PHI = sqrtf( 5.0f ) * 0.5f + 0.5f;
	#define madfrac(A, B) ((A)*(B)-floor((A)*(B)))
	const float phi = TWO_PI * madfrac( i, PHI - 1.0f );
	const float cosTheta = 1.0f - ( 2.0f * i + 1.0f ) * ( 1.0f / n );
	const float sinTheta = sqrtf( decMath::clamp( 1.0f - cosTheta * cosTheta, 0.0f, 1.0f ) );
	return decVector( cosf( phi ) * sinTheta, sinf( phi ) * sinTheta, cosTheta );
	#undef madfrac
}

void deoglOcclusionTracing::pInitSphericalFibonacci(){
	int i;
	for( i=0; i<pRaysPerProbe; i++ ){
		pSphericalFibonacci[ i ] = deoglOcclusionTracing_SphericalFibonacci( i, pRaysPerProbe );
	}
}
